#include "UltrasonicSensors.h"
#include "DriveSystem.h"
#include "ColourSensor.h"
#include "Pump.h"

// --- Pin map (ultrasonic): trig/echo per channel ---
// Left:   trig 8, echo 9
// Center: trig 7, echo 6
// Right:  trig 4, echo 5
// Constructor order: (right trig, right echo, center trig, center echo, left trig, left echo)
UltrasonicSensors ultrasonic(4, 5, 7, 6, 8, 9);

// Drive: (enR, r1, r2, enL, l1, l2)
DriveSystem drive(3, 2, 12, 11, 13, A3);

ColourSensor colour;
Pump pump(10);

static const unsigned long mainLoopDelayMs = 500;

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

    if (status)
    {
        // Stop driving; run pump until power cycle (infinite loop by design).
        drive.stop();
        pump.on();
        while (true)
        {
        }
    }
    else
    {
        pump.off();
    }

    delay(mainLoopDelayMs);
}
