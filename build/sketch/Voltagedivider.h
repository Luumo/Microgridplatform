#ifndef VOLTAGEDIVIDER_H
#define VOLTAGEDIVIDER_H
#include <arduino.h>

class VoltageDivider{
    public:
    VoltageDivider(int pin, float R1, float R2);
    float readVoltage();
    float m_voltage;

    private:
    int m_pin;
    float m_R1;
    float m_R2;
    int m_inputvalue = 0;
    //helper function
    void readInputSignal(int m_pin);

};
#endif //VOLTAGEDIVIDER_H