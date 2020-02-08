#ifndef RTDSENSOR_H
#define RTDSENSOR_H
#include <arduino.h>

#define SERIESRESISTOR 5025.0     //Resistor value of series resistor in voltage divider

class RtdSensor{
    public:
    RtdSensor(int pin);
    float readTemperature();
    // void SerialPrint();


    private:
    int m_pin;
    float m_ADC;
    float m_resistance;
    float R0 = 1000.0;            // nominal value, resistance at 0*C for PT-1000
    float A = 3.9083E-3;        // Constants for pt-1000
    float B = -5.775E-7;        //Constant
    float C = -4.183*10E-12;    //constant
    
    //helper functions
    float readInputSignal(int m_pin);
    float getResistance();
    
};
#endif //RTDSENSOR_H