#include "UltrasonicSensors.h"
#include "DriveSystem.h"
#include "ColourSensor.h"
#include "Pump.h"

UltrasonicSensors ultrasonic(9, 8, 7, 6, 5, 4);
DriveSystem drive(3, 2, 12, 11, 13, A3);
ColourSensor colour;
Pump pump(10);

void setup()
{
    Serial.begin(9600);

    // ultrasonic.begin();
    // drive.begin();
    colour.begin();
    pump.begin();
}

void loop()
{
    // ultrasonic.update();

    // drive.navigate(ultrasonic.readings);
    bool status = colour.redDetected();
    Serial.println(status);
    if (status)
        pump.on();
    else
        pump.off();
    delay(500);
}