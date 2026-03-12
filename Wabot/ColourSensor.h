#ifndef COLOURSENSOR_H
#define COLOURSENSOR_H

#include <Adafruit_AS726x.h>

class ColourSensor {

private:
    Adafruit_AS726x sensor;

public:
    void begin();
    bool redDetected();
};

#endif