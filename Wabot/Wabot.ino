#include "UltrasonicSensors.h"
#include "DriveSystem.h"
#include "ColorSensor.h"
#include "Pump.h"

UltrasonicSensors ultrasonic(9,8,7,6,5,4);
DriveSystem drive(3,2,12,11,13,A3);
ColorSensor color;
Pump pump(10);

void setup()
{
    Serial.begin(9600);

    ultrasonic.begin();
    drive.begin();
    color.begin();
    pump.begin();
}

void loop()
{
    ultrasonic.update();

    drive.navigate(ultrasonic.readings);

    if(color.redDetected())
        pump.on();
    else
        pump.off();
}