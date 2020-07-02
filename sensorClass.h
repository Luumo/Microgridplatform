#ifndef SENSORCLASS_H
#define SENSORCLASS_H
#include <arduino.h>

#define SAMPLE_INTERVAL 10

class Sensor{
    public:
    Sensor(int pin); // constructor

    float readPin();
    float readPinVoltage();
    
    int m_pin;
    private:

};

class TestSensor: public Sensor{
    public:

}


#endif SENSORCLASS_H
