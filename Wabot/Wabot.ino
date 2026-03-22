#include "../lib/UltrasonicSensors.h"
#include "../lib/DriveSystem.h"
#include "../lib/ColourSensor.h"
#include "../lib/Pump.h"


// trig = 8 left, echo = 9 left
// trig = 7 center, echo = 6 center
// trig = 4 right, echo = 5 right
UltrasonicSensors ultrasonic(4, 5, 7, 6, 8, 9);
DriveSystem drive(3, 2, 12, 11, 13, A3);
ColourSensor colour;
Pump pump(10);

void setup()
{
    Serial.begin(9600);

    ultrasonic.begin();
    drive.begin();
    drive.setSpeed(200);
    colour.begin();
    pump.begin();
}

void loop()
    {
    ultrasonic.update();
    drive.navigate(ultrasonic.readings);

    bool status = colour.redDetected();
    Serial.println(status);
    if (status){
        drive.stop();
        pump.on();
        
        while(true){
            
        }
    }
    else{
        pump.off();
    }
    delay(500);
}