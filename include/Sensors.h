#ifndef SENSORS_H
#define SENSORS_H

#include <avr/io.h>
#include <stdlib.h>
#include "Lcd.h"

#define MAX_HUMINITY 700

typedef struct
{
    char id;           // Identyfikator czujnika
    uint16_t huminity; // Dane czujnika (16-bitowe)
} Sensor;

class Sensors
{
public:
    Sensors();
    void addOrUpdateSensor(uint8_t name, uint16_t huminity);
    void switchSensor();
    ~Sensors();

private:
    Lcd lcd;
    Sensor *sensors;
    uint8_t activeSensorId;
    uint8_t sensorCount;

    void displayActiveSensor();
};

#endif