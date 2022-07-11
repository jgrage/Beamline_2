/* valve.cpp
 * Jonas
 */
#include "Arduino.h"
#include "gauge.hpp"
#include "valve.hpp"

/* -----------------------
 * Generic valve functions
 * -----------------------
 */
void valve::connect(int open_pin, int encoder_closed_pin, int encoder_open_pin){
    
    _open_pin = open_pin;
    _encoder_closed_pin = encoder_closed_pin;
    _encoder_open_pin = encoder_open_pin;
    
    pinMode(_encoder_closed_pin, INPUT);
    pinMode(_encoder_open_pin, INPUT);
    pinMode(_open_pin, OUTPUT);
    return;
}

void valve::open(){
    if (state == CLOSED_BLOCKED){
        digitalWrite(_open_pin, LOW);
    }
    else{
        digitalWrite(_open_pin, HIGH);
    }
    return;
}

void valve::close(){
    if (state == OPEN_BLOCKED){
        digitalWrite(_open_pin, HIGH);
    }
    else{
        digitalWrite(_open_pin, LOW);
    }
    return;
}

/* -------------------------
 * blockable valve functions
 * -------------------------
 */
blockable_valve::blockable_valve(gauge& left_section, gauge& right_section) : left(left_section), right(right_section){
    return;
}

void blockable_valve::update_state(){
    int closed = digitalRead(_encoder_closed_pin);
    int open = digitalRead(_encoder_open_pin);
    
    
    if ((closed == HIGH) && (open == LOW)){
        // Valve is closed. Check if blocked because of difference in pressures
        if (left.state == right.state){
            state = CLOSED;
        }
        
        else{
            state = CLOSED_BLOCKED;
        }
        
    }
    else if ((closed == LOW) && (open == HIGH)){
        // Valve is currently open
        state = OPEN;
        
        /* !!! NOT YET TESTED !!!
         * Close the valve automatically if a leak causes a difference
         * in pressure. Depending on the distance of the pressure
         * sensors and the setpoints this should trigger on medium
         * sized leaks in the system */
        if (left.state != right.state){
            close();
        }
    }
    
    else{
        state = MOVING;
    }
    return;
}

/* ----------------------------
 * Obstructable valve functions
 * ----------------------------
 */
obstructable_valve::obstructable_valve(gauge& left_section, gauge& right_section) : left(left_section), right(right_section){
    return;
}

void obstructable_valve::connect(int open_pin, int encoder_closed_pin, int encoder_open_pin, int lock_pin){
    
    _open_pin = open_pin;
    _encoder_closed_pin = encoder_closed_pin;
    _encoder_open_pin = encoder_open_pin;
    _lock_pin = lock_pin;
    
    pinMode(_encoder_closed_pin, INPUT);
    pinMode(_encoder_open_pin, INPUT);
    pinMode(_open_pin, OUTPUT);
    pinMode(_lock_pin, INPUT);
    return;
}

void obstructable_valve::update_state(){
    int closed = digitalRead(_encoder_closed_pin);
    int open = digitalRead(_encoder_open_pin);
    // assuming active low
    int locked = digitalRead(_lock_pin);
    
    
    if ((closed == HIGH) && (open == LOW)){
        // Valve is closed. Check if blocked because of difference in pressures
        if (left.state == right.state){
            state = CLOSED;
        }
        
        else{
            state = CLOSED_BLOCKED;
        }
        
    }
    else if ((closed == LOW) && (open == HIGH)){
        /* Valve is open. An obstructible valve will not close
         * automatically on changing pressure to prevent 
         * damage to the equipment */
        if (locked == HIGH){
            state = OPEN;
        }
        // check if blocked
        else{
            state = OPEN_BLOCKED;
        }
    }
    
    else{
        state = MOVING;
    }
    return;
}
