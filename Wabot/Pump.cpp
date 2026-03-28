// Include the header file for the Pump class, and the Arduino library for pin control and serial output.
#include "Pump.h"
#include <Arduino.h>

// Constructor for the Pump class, takes the control pin number as an argument and stores it in the relayPin member variable.
Pump::Pump(int pin)
{
    relayPin = pin;
}

// Initializes the pump by setting the relayPin as an OUTPUT and ensuring the pump is turned off at starting.
void Pump::begin()
{
    pinMode(relayPin, OUTPUT);
    off();
}

// Turns the pump on by setting the relayPin HIGH, and prints a message to the serial monitor for debugging purposes.
void Pump::on()
{
    digitalWrite(relayPin, HIGH);
    Serial.println("Pump ON");
}

// Turns the pump off by setting the relayPin LOW, and prints a message to the serial monitor for debugging purposes.
void Pump::off()
{
    digitalWrite(relayPin, LOW);
    Serial.println("Pump OFF");
}