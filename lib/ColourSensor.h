#ifndef COLOURSENSOR_H
#define COLOURSENSOR_H

#include <Adafruit_AS726x.h>

struct ColourReadings {
    float violet;
    float blue;
    float green;
    float yellow;
    float orange;
    float red;
};

class ColourSensor {

private:
    Adafruit_AS726x sensor;

public:
    void begin();
    bool redDetected();
    ColourReadings readAllColours();
};

#endif