#ifndef CURRENTSENSOR_H
#define CURRENTSENSOR_H
class CurrentSensor{
    public:
    CurrentSensor(int pin);
    void calcAdcVoltage();          // helper-function, converts to mV 
    void getAdcValue(int m_pin);     // helper-function, to get raw input value
    double calcCurrentValue();      // returns Ampere
    double currentValue = 0;

    private:
    int m_pin;
    int m_sensitivity     = 66;     //66mV/A
    int m_offsetVoltage   = 2500;    // VIOT 2.5V when no load on ammeter
    int m_adcValue        = 0;
    double m_adcVoltage   = 0;
};  

#endif // CURRENTSENSOR_H
