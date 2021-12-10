/* main.cpp
 * Jonas
 */
 
#include <avr/wdt.h>
#include <Ethernet.h>
#include <Controllino.h>
#include <scpiparser.h>
#include <gauge.hpp>
#include <valve.hpp>

#include "scpi_commands.hpp"


// Create vacuum components
gauge turbo;
gauge transfer_chamber;
gauge sample_chamber;

// gate valves (VAT)
blockable_valve valve1(turbo, transfer_chamber);
obstructable_valve valve2(transfer_chamber, sample_chamber);

// angle valves (Pfeiffer)
angle_valve valve3(valve1, valve2);
angle_valve valve4(valve1, valve2);


// Define ethernet interface
byte mac[] = { 0x42, 0x23, 0x00, 0xC0, 0xFE, 0xFE };
EthernetServer server = EthernetServer(23);


// Declare buffers for SCPI communication
uint8_t RESPONSE_LEN;
char COMMAND_BUFFER[BUFFER_LEN];
char SEND_BUFFER[BUFFER_LEN];
struct scpi_parser_context ctx;


// Flag used for periodic polling of readback values
volatile uint8_t timer_flag = 0;
volatile uint8_t reset_flag = 0;
volatile uint8_t disconnect_flag = 0;
volatile uint8_t change_flag = 0;

void setup(void){
    // Initialize ethernet controller
    //Ethernet.begin(mac, ip, gateway, subnet);
    //DHCP
    Ethernet.begin(mac);
    
    // Clear watchdog timer
    MCUSR = 0;
    
    // setup timer for sensor polling via ISR
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCNT1 = 0x00;
    OCR1A = 780;                        // set compare unit to 780 (results in 10Hz timer with a prescaler of 1024)
    TCCR1B |= _BV(WGM12);               // CTC mode
    TCCR1B |= _BV(CS12) | _BV(CS10);    // set prescaler to 1024
    TIMSK1 |= _BV(OCIE1B);              // enable compare match interrupt
    
    
    // Initialize and connect the vacuum components
    // Relay output of the TPG Controller(s)
    //main_chamber.connect(CONTROLLINO_A10);
    transfer_chamber.connect(CONTROLLINO_A11);
    turbo.connect(CONTROLLINO_A12);
    sample_chamber.connect(CONTROLLINO_A13);
    //pre_pump.connect(CONTROLLINO_A14);
    
    // Vacuum valves
    valve3.connect_third_valve(&valve4);
    valve4.connect_third_valve(&valve3);

    valve1.connect(CONTROLLINO_D0, CONTROLLINO_A0, CONTROLLINO_A1);
    valve2.connect(CONTROLLINO_D1, CONTROLLINO_A2, CONTROLLINO_A3, CONTROLLINO_A8);
    valve3.connect(CONTROLLINO_D2, CONTROLLINO_A4, CONTROLLINO_A5);
    valve4.connect(CONTROLLINO_D3, CONTROLLINO_A6, CONTROLLINO_A7);
    
    
    //Register System SCPI commands
    scpi_init(&ctx);
    scpi_register_command(ctx.command_tree, SCPI_CL_SAMELEVEL, "*IDN?", 5, "*IDN?", 5, identify);
    scpi_register_command(ctx.command_tree, SCPI_CL_SAMELEVEL, "*RST", 4, "*RST", 4, reset);
    scpi_register_command(ctx.command_tree, SCPI_CL_SAMELEVEL, "*END", 4, "*END", 4, disconnect);
    
    // Generate Valve 1 command tree
    struct scpi_command* v1;
    v1 = scpi_register_command(ctx.command_tree, SCPI_CL_CHILD, "VALVE1", 6, "V1", 2, NULL);
    scpi_register_command(v1, SCPI_CL_CHILD, "OPEN", 4, "O", 1, valve1_open);
    scpi_register_command(v1, SCPI_CL_CHILD, "CLOSE", 5, "C", 1, valve1_close);
    scpi_register_command(v1, SCPI_CL_CHILD, "STATE?", 6, "ST?", 3, valve1_getstate);
    
    // Generate Valve 2 command tree
    struct scpi_command* v2;
    v2 = scpi_register_command(ctx.command_tree, SCPI_CL_CHILD, "VALVE2", 6, "V2", 2, NULL);
    scpi_register_command(v2, SCPI_CL_CHILD, "OPEN", 4, "O", 1, valve2_open);
    scpi_register_command(v2, SCPI_CL_CHILD, "CLOSE", 5, "C", 1, valve2_close);
    scpi_register_command(v2, SCPI_CL_CHILD, "STATE?", 6, "ST?", 3, valve2_getstate);
    
    // Generate Valve 3 command tree
    struct scpi_command* v3;
    v3 = scpi_register_command(ctx.command_tree, SCPI_CL_CHILD, "VALVE3", 6, "V3", 2, NULL);
    scpi_register_command(v3, SCPI_CL_CHILD, "OPEN", 4, "O", 1, valve3_open);
    scpi_register_command(v3, SCPI_CL_CHILD, "CLOSE", 5, "C", 1, valve3_close);
    scpi_register_command(v3, SCPI_CL_CHILD, "STATE?", 6, "ST?", 3, valve3_getstate);
    
    // Generate Valve 4 command tree
    struct scpi_command* v4;
    v4 = scpi_register_command(ctx.command_tree, SCPI_CL_CHILD, "VALVE4", 6, "V4", 2, NULL);
    scpi_register_command(v4, SCPI_CL_CHILD, "OPEN", 4, "O", 1, valve4_open);
    scpi_register_command(v4, SCPI_CL_CHILD, "CLOSE", 5, "C", 1, valve4_close);
    scpi_register_command(v4, SCPI_CL_CHILD, "STATE?", 6, "ST?", 3, valve4_getstate);
    
    
    // Start server and enable interrupts
    server.begin();
    sei();
}


// Raise timer_flag
ISR(TIMER1_COMPB_vect){
    timer_flag = 1;
}


// Helper function to read variable length command and put it in a buffer
unsigned int get_command(EthernetClient* client, char* Buffer, uint8_t MaxLen){
    unsigned int c;
    uint8_t idx = 0;
    
    while(client->available() && idx < MaxLen - 1){
        c = client->read();
        
        if ((unsigned char)c == '\n'){
            break;
        }
        
        else{
            Buffer[idx] = (unsigned char)c;
            idx++;
        }
    }
    //Buffer[idx] = '\0';
    return idx;
}


void loop(){
        
    if(timer_flag == 1){
        // Flash indicating that timer is running
        digitalWrite(CONTROLLINO_D8, HIGH);
        
        // Update state of the gauges
        //main_chamber.update_state();
        transfer_chamber.update_state();
        turbo.update_state();
        sample_chamber.update_state();
        //pre_pump.update_state();
        
        // update states of the valves
        valve1.update_state();
        valve2.update_state();
        valve3.update_state();
        valve4.update_state();
        
        delay(20);
        digitalWrite(CONTROLLINO_D8, LOW);
        delay(20);
        timer_flag = 0;
    }
        
    EthernetClient client = server.available();
    if (client) {
        int len = get_command(&client, COMMAND_BUFFER, BUFFER_LEN);
        scpi_execute_command(&ctx, COMMAND_BUFFER, len);
        
        if (RESPONSE_LEN > 0){
            client.write(SEND_BUFFER, RESPONSE_LEN);
            RESPONSE_LEN = 0;
        }
        
        if (disconnect_flag == 1){
            client.stop();
            disconnect_flag = 0;
        }
        
        if (reset_flag == 1){
            client.stop();
            wdt_enable(WDTO_15MS);
        }
    }
}
