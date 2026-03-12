#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include <Adafruit_AS726x.h>

class ColorSensor {

private:
    Adafruit_AS726x sensor;

public:
    void begin();
    bool redDetected();
};

#endif