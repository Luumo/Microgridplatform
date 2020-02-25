#ifndef CURRENTSENSOR_H
#define CURRENTSENSOR_H

class CurrentSensor{
    public:
    CurrentSensor(int pin, float sensitivity);
    float readCurrent();      // returns Ampere
    float currentValue = 0.000;

    private:
    int m_pin;
    float m_sensitivity;     //66mV/A for 30A sensor, 100mV/A for 20A sensor 
    float m_offsetVoltage   = 2500.0;    // VIOT 2.5V when no load on ammeter
    float m_adcValue        = 0.0;
    float m_adcVoltage      = 0.0;
    void calcAdcVoltage();          // helper-function, converts to mV 
    void getAdcValue(int m_pin);     // helper-function, to get raw input value
};  

#endif // CURRENTSENSOR_H
