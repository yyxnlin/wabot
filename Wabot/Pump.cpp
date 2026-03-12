#include "Pump.h"
#include <Arduino.h>

Pump::Pump(int pin)
{
    relayPin = pin;
}

void Pump::begin()
{
    pinMode(relayPin, OUTPUT);
    off();
}

void Pump::on()
{
    digitalWrite(relayPin, HIGH);
    Serial.println("Pump ON");
}

void Pump::off()
{
    digitalWrite(relayPin, LOW);
    Serial.println("Pump OFF");
}