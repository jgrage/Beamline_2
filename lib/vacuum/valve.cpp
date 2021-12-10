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
        // Closed valve. Check if blocked because of difference in pressures
        if (left.state == right.state){
            state = CLOSED;
        }
        
        else{
            state = CLOSED_BLOCKED;
        }
        
    }
    else if ((closed == LOW) && (open == HIGH)){
        // Open valve
        state = OPEN;
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
        // Closed valve. Check if blocked because of difference in pressures
        if (left.state == right.state){
            state = CLOSED;
        }
        
        else{
            state = CLOSED_BLOCKED;
        }
        
    }
    else if ((closed == LOW) && (open == HIGH)){
        // Open valve
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

/* ------------------
 * Closed dummy valve
 * ------------------
 */
angle_valve::angle_valve(valve& first_valve, valve& second_valve) : first(first_valve), second(second_valve){
    return;
}

void angle_valve::connect_third_valve(valve *third_valve){
    third = third_valve;
    return;
}

void angle_valve::update_state(){
    int closed = digitalRead(_encoder_closed_pin);
    int open = digitalRead(_encoder_open_pin);    
    
    if ((closed == HIGH) && (open == LOW)){
        // Closed valve. Check if blocked because of the state of the other valves on the chamber
        
        if ( ((first.state == CLOSED_BLOCKED) || (first.state == CLOSED)) && ((second.state == CLOSED_BLOCKED) || (second.state == CLOSED)) && ((third->state == CLOSED_BLOCKED) || (third->state == CLOSED)) ){
            state = CLOSED;
        }
        /*
        if ( (third->state == CLOSED_BLOCKED) ){
            state = CLOSED;
        }
        */
        else{
            state = CLOSED_BLOCKED;
        }
        
    }
    else if ((closed == LOW) && (open == HIGH)){
        state = OPEN;
    }
    
    else{
        state = MOVING;
    }
    return;
}