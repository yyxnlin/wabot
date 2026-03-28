// Include the header file for the UltrasonicSensors class, and the Arduino library for pin control, serial output, and delay functions.
#include "UltrasonicSensors.h"
#include "DriveSystem.h"
#include <Arduino.h>

// Constructor for the UltrasonicSensors class, takes the trig and echo pin numbers for the left, center, and right sensors as arguments and stores them in member variables.
UltrasonicSensors::UltrasonicSensors(int tl,int el,int tc,int ec,int tr,int er)
{
    trigLeft = tl;
    echoLeft = el;

    trigCenter = tc;
    echoCenter = ec;

    trigRight = tr;
    echoRight = er;
}

// Initializes the ultrasonic sensors by setting the trig pins as OUTPUT and the echo pins as INPUT.
void UltrasonicSensors::begin()
{
    pinMode(trigLeft, OUTPUT);
    pinMode(echoLeft, INPUT);

    pinMode(trigCenter, OUTPUT);
    pinMode(echoCenter, INPUT);

    pinMode(trigRight, OUTPUT);
    pinMode(echoRight, INPUT);
}

// Finds the median of three long integers using a simple sorting algorithm. This is used to filter out outliers from the ultrasonic sensor readings by taking three samples and returning the middle value.
long UltrasonicSensors::median(long a, long b, long c)
{
    long temp;

    if (a > b) {
        temp = a;
        a = b;
        b = temp;
    }

    if (b > c) {
        temp = b;
        b = c;
        c = temp;
    }

    if (a > b) {
        temp = a;
        a = b;
        b = temp;
    }

    return b;
}

// Read the distance from a single ultrasonic sensor by sending a trigger pulse and measuring the duration of the echo pulse. The function returns the distance in centimeters, or -1 if the reading is invalid (e.g., no echo received within the timeout period).
long UltrasonicSensors::readSensor(int trig, int echo)
{
    digitalWrite(trig, LOW); // Ensure the trigger pin is low before starting the measurement.
    delayMicroseconds(2);

    digitalWrite(trig, HIGH); // Send a 10 microsecond pulse to the trigger pin to start the measurement.
    delayMicroseconds(10);
    digitalWrite(trig, LOW); // Set the trigger pin back to low after the pulse.

    long duration = pulseIn(echo, HIGH, 10000); // Wait for the echo pin to go HIGH and measure the duration of the pulse, with a timeout of 10 milliseconds (10000 microseconds).

    // If the duration is 0, it means no echo was received within the timeout period, so return -1 to indicate an invalid reading.
    if(duration == 0)
        return -1;

    return duration * 0.0343 / 2; // Convert the duration to distance in centimeters (speed of sound is approximately 343 m/s, which is 0.0343 cm/µs, and divide by 2 for the round trip).
}

// Reads the distance from each ultrasonic sensor by taking three samples from each and storing the median value in the readings array.
void UltrasonicSensors::update()
{
    // LEFT sensor (3 samples)
    long a1 = readSensor(trigLeft, echoLeft);
    long b1 = readSensor(trigLeft, echoLeft);
    long c1 = readSensor(trigLeft, echoLeft);
    readings[0] = median(a1, b1, c1); // Store the median of the three samples for the left sensor in readings[0].

    // delay(250); // allow echoes to clear

    // CENTER sensor (3 samples)
    long a2 = readSensor(trigCenter, echoCenter);
    long b2 = readSensor(trigCenter, echoCenter);
    long c2 = readSensor(trigCenter, echoCenter);
    readings[1] = median(a2, b2, c2); // Store the median of the three samples for the center sensor in readings[1].

    // delay(250);

    // RIGHT sensor (3 samples)
    long a3 = readSensor(trigRight, echoRight);
    long b3 = readSensor(trigRight, echoRight);
    long c3 = readSensor(trigRight, echoRight);
    readings[2] = median(a3, b3, c3); // Store the median of the three samples for the right sensor in readings[2].

    // delay(250);
}