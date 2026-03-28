#ifndef PUMP_H
#define PUMP_H

// Controls the water pump via a relay. The relay is active HIGH, so the pump turns on when the pin is HIGH and off when LOW.
class Pump
{

private:
    int relayPin;

public:
    Pump(int pin);

    void begin();
    void on();
    void off();
};

#endif