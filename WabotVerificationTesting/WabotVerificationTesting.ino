#include <Arduino.h>
#include "UltrasonicSensors.h"
#include "ColourSensor.h"

UltrasonicSensors ultrasonic(4, 5, 7, 6, 8, 9);
ColourSensor colour;

unsigned long previousMillis = 0;
const unsigned long interval = 1000; // 1 Hz

// Print ultrasonic + colour readings at 1 Hz
void testSensorReadings1Hz() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Update sensors
        ultrasonic.update();
        ColourReadings readings = colour.readAllColours();

        // Print readings
        Serial.print("Time: "); Serial.print(currentMillis); Serial.print(" ms | ");
        Serial.print("Ultrasonic Left: "); Serial.print(ultrasonic.readings[0]); Serial.print(" cm | ");
        Serial.print("Center: "); Serial.print(ultrasonic.readings[1]); Serial.print(" cm | ");
        Serial.print("Right: "); Serial.print(ultrasonic.readings[2]); Serial.println(" cm | ");

        Serial.print("V: "); Serial.print(readings.violet);
        Serial.print(" B: "); Serial.print(readings.blue);
        Serial.print(" G: "); Serial.print(readings.green);
        Serial.print(" Y: "); Serial.print(readings.yellow);
        Serial.print(" O: "); Serial.print(readings.orange);
        Serial.print(" R: "); Serial.println(readings.red);
    }
}

// Check if red is detected
void testRedDetection() {
    if (colour.redDetected()) {
        Serial.println("TEST: Red detected!");
    } else {
        Serial.println("TEST: Red not detected.");
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("=== Wabot Verification Tests ===");

    ultrasonic.begin();
    colour.begin();
}

void loop() {
    testSensorReadings1Hz(); // prints sensor readings at 1 Hz
    // testRedDetection();      // checks red detected continuously
}