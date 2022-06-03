/* sample_holder.cpp
 * Jonas
 */
#include "Arduino.h"
#include "sample_holder.hpp"

sample_holder::sample_holder(){
    return;
}

void sample_holder::connect(int pin){
    _readback_pin = pin;
    pinMode(_readback_pin, INPUT);
    return;
}

void sample_holder::update_state(){
    state = digitalRead(_readback_pin);
    return;
}
