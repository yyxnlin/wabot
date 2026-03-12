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
    digitalWrite(relayPin, LOW);
}

void Pump::off()
{
    digitalWrite(relayPin, HIGH);
}