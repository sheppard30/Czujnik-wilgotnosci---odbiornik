#include "Sensors.h"
#include "Uart.h"

Sensors::Sensors()
{
    sensors = nullptr;
    sensorCount = 0;
    activeSensorId = 0;
}

void Sensors::addOrUpdateSensor(uint8_t id, uint16_t huminity)
{
    // Sprawdź, czy czujnik już istnieje
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        if (sensors[i].id == id)
        {
            // Zaktualizuj dane istniejącego czujnika
            sensors[i].huminity = huminity;
            return;
        }
    }

    // Jeśli nie istnieje, dodaj nowy czujnik
    sensorCount++;                                                      // Zwiększamy liczbę czujników
    sensors = (Sensor *)realloc(sensors, sensorCount * sizeof(Sensor)); // Powiększamy tablicę

    if (sensors != nullptr)
    {
        sensors[sensorCount - 1].id = id;             // Ustawienie identyfikatora
        sensors[sensorCount - 1].huminity = huminity; // Ustawienie danych czujnika
    }

    activeSensorId = sensorCount - 1;
    displayActiveSensor();
}

void Sensors::switchSensor()
{
    if (sensorCount == 0)
    {
        return;
    }

    if (sensorCount == activeSensorId + 1)
    {
        activeSensorId = 0;
        displayActiveSensor();
        return;
    }

    if (sensorCount > activeSensorId + 1)
    {
        activeSensorId++;
        displayActiveSensor();
        return;
    }
}

void Sensors::displayActiveSensor()
{
    if (sensorCount == 0 || activeSensorId >= sensorCount)
    {
        return;
    }

    char sensorLabel[] = "Czujnik: ";
    char huminityLabel[] = "Wilgotnosc: ";

    Sensor sensor = sensors[activeSensorId];

    char id = sensor.id;
    uint8_t huminity = sensor.huminity;

    lcd.reset();
    lcd.print(sensorLabel);
    lcd.print(id);
    lcd.goToRow(1);
    lcd.print(huminityLabel);
    lcd.print(huminity);
    lcd.print('%');
}

Sensors::~Sensors()
{
    if (sensors != nullptr)
    {
        free(sensors); // Zwalniamy pamięć po czujnikach
    }
}