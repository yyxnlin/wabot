#ifndef PUMP_H
#define PUMP_H

class Pump {

private:
    int relayPin;

public:
    Pump(int pin);

    void begin();
    void on();
    void off();
};

#endif