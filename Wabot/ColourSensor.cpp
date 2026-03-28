// Include the header file for the ColourSensor class, and the Arduino library for serial output and delay functions.
#include "ColourSensor.h"
#include <Arduino.h>

// Constructor for the ColourSensor class, initializes the Adafruit_AS726x sensor object.
void ColourSensor::begin()
{
    // Check if the sensor is connected and can be initialized. If not, print an error message and halt the program in an infinite loop.
    if (!sensor.begin())
    {
        Serial.println("Sensor not found");
        while (1);
    }

    sensor.setDrvCurrent(1);       // Set the drive current to 1mA.
    sensor.drvOn();                // Turn on the sensor's internal LED driver.
    sensor.setIntegrationTime(50); // Set the integration time to 50ms for faster readings.
}

// Checks if red is detected by the colour sensor. Read the calibrated values for all six colour channels (violet, blue, green, yellow, orange, red). The function returns true if the red and orange channels are both dominant compared to the other four channels, indicating a strong presence of red. Otherwise, it returns false.
bool ColourSensor::redDetected()
{
    sensor.startMeasurement(); // Start a new measurement cycle on the sensor.

    while (!sensor.dataReady())
        delay(5);

    float violet = sensor.readCalibratedViolet();
    float blue = sensor.readCalibratedBlue();
    float green = sensor.readCalibratedGreen();
    float yellow = sensor.readCalibratedYellow();
    float orange = sensor.readCalibratedOrange();
    float red = sensor.readCalibratedRed();

    // Red and orange more dominant than the other 4 channels.
    if (red >= violet && red >= blue && red >= green && red >= yellow &&
        orange >= violet && orange >= blue && orange >= green && orange >= yellow)
    {
        Serial.println("RED DETECTED"); // Print a message to the serial monitor for debugging purposes when red is detected.
        return true;
    }

    return false;
}