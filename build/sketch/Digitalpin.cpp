#include "Digitalpin.h"

DigitalPin::DigitalPin(int pin){ //constructor
    _pin = pin;
}

void DigitalPin::begin(){ // set the pin to output
    pinMode(_pin, OUTPUT);
}

void DigitalPin::on(){
    digitalWrite(_pin, HIGH);
    _status = 1;
}

void DigitalPin::off(){
    digitalWrite(_pin, LOW);
    _status = 0;
}

bool DigitalPin::status(){
    return _status;
}