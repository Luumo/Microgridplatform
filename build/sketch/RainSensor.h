#ifndef RAINSENSOR_H
#define RAINSENSOR_H

#include <arduino.h>

class RainSensor{
    public:
    RainSensor(int digital_pin, int analog_pin);
    int readRain();
    int AnalogSignal();
    private:
    int m_digital_pin;
    int m_analog_pin;
};

#endif //RAINSENSOR_H