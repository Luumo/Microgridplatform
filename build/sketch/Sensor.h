#ifndef SENSOR_H
#define SENSOR_H
#include <arduino.h>


class Sensor{
    public:
    Sensor(char sensorname);
    char sensorname;
    char sensortype;
    char id;
    char prefix; 

    private:
    char m_sensorname;
};

#endif //SENSOR_H
