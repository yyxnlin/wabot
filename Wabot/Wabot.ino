// Include the important hearder files for the various components of the robot.
#include "UltrasonicSensors.h" // Ultrasonic sensor class, for distance measurement.
#include "DriveSystem.h"       // Drive system class, for controlling the motors and robot movement.
#include "ColourSensor.h"      // Colour sensor class, for detecting red.
#include "Pump.h"              // Pump class, for controlling the water pump.

// --- Pin map (ultrasonic): trig/echo per channel ---
// Left:   trig 8, echo 9
// Center: trig 7, echo 6
// Right:  trig 4, echo 5
// Constructor order: (right trig, right echo, center trig, center echo, left trig, left echo)
UltrasonicSensors ultrasonic(4, 5, 7, 6, 8, 9);

// --- Pin Map (Drive System): (enableRight, right1, right2, enableLeft, left1, left2) ---
DriveSystem drive(3, 2, 12, 11, 13, A3);

// --- Pins (Colour Sensor): (SCL= A5, SDA=A4) *built into Adafruit_AS726x.h* ---
ColourSensor colour;

// --- Pin Map (Pump): (control pin) ---
Pump pump(10);

// Delay between each loop iteration, in milliseconds. Adjust as needed for performance vs responsiveness.
static const unsigned long mainLoopDelayMs = 500;

// Setup function, runs once at startup. Initializes all the components and sets initial conditions.
void setup()
{
    Serial.begin(9600);

    ultrasonic.begin();  // Initialize ultrasonic sensors.
    drive.begin();       // Initialize drive system.
    drive.setSpeed(200); // Set initial speed (0-255) for the drive system.
    colour.begin();      // Initialize colour sensor.
    pump.begin();        // Initialize pump (sets pin mode and turns it off).
}

// Main loop function, runs repeatedly after setup. Handles sensor readings, navigation, and pump control based on colour detection.
void loop()
{
    ultrasonic.update();                 // Update ultrasonic sensor readings (stores results in ultrasonic.readings[]).
    drive.navigate(ultrasonic.readings); // Navigate based on ultrasonic sensor readings (obstacle avoidance).

    bool status = colour.redDetected(); // Check if red is detected by the colour sensor, and store the result in a boolean variable.
    Serial.println(status);             // Print status of red detection for debugging.

    // If red is detected, execute the following code to stop the robot, run the pump, and wait until red is no longer detected before stopping the pump and halting the robot. If it's not detected, ensure the pump is off.
    if (status)
    {
        drive.stop();
        pump.on();
        while (colour.redDetected())
        {
            delay(10);
        }
        delay(100);
        pump.off();
        while (true)
        {
            drive.stop();
        }
    }
    else
    {
        pump.off();
    }

    delay(mainLoopDelayMs); // Delay before the next loop iteration to control the update rate of the robot's sensors and actions.
}
