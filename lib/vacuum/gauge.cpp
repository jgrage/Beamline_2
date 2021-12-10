/* gauge.cpp
 * Jonas
 */
#include "Arduino.h"
#include "gauge.hpp"

gauge::gauge(){
    return;
}

void gauge::connect(int pin){
    _readback_pin = pin;
    pinMode(_readback_pin, INPUT);
    return;
}

void gauge::update_state(){
    state = digitalRead(_readback_pin);
    return;
}
