#include "ColourSensor.h"
#include <Arduino.h>

void ColourSensor::begin()
{
    if(!sensor.begin())
    {
        Serial.println("Sensor not found");
        while(1);
    }

    sensor.setDrvCurrent(1);
    sensor.drvOn();
    sensor.setIntegrationTime(50);
}

bool ColourSensor::redDetected()
{
    sensor.startMeasurement();

    while(!sensor.dataReady())
        delay(5);

    float violet = sensor.readCalibratedViolet();
    float blue = sensor.readCalibratedBlue();
    float green = sensor.readCalibratedGreen();
    float yellow = sensor.readCalibratedYellow();
    float orange = sensor.readCalibratedOrange();
    float red = sensor.readCalibratedRed();

    if(red >= violet && red >= blue && red >= green && red >= yellow &&
       orange >= violet && orange >= blue && orange >= green && orange >= yellow)
    {
        Serial.println("RED DETECTED");
        return true;
    }

    return false;
}