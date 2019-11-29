#include <Arduino.h>

#ifndef DIGITALPIN_H
#define DIGITALPIN_H

class DigitalPin{
    public:
    DigitalPin(int pin);
    void begin();
    void on();
    void off();
    bool status();

    private:
    int _pin;
    bool _status;
};

#endif //DIGITALPIN_H
