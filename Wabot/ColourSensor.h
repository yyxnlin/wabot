#ifndef COLOURSENSOR_H
#define COLOURSENSOR_H

#include <Adafruit_AS726x.h>

// Controls the AS726x colour sensor. Use the calibrated colour readings to determine if red is detected. The red detection logic considers both the red and orange channels, and requires them to be the dominant colours compared to violet, blue, green, and yellow for a positive red detection.
class ColourSensor
{

private:
    Adafruit_AS726x sensor;

public:
    void begin();
    bool redDetected();
};

#endif