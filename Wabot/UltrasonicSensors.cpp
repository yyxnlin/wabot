#include "UltrasonicSensors.h"
#include <Arduino.h>

UltrasonicSensors::UltrasonicSensors(int tl,int el,int tc,int ec,int tr,int er)
{
    trigLeft = tl;
    echoLeft = el;

    trigCenter = tc;
    echoCenter = ec;

    trigRight = tr;
    echoRight = er;
}

void UltrasonicSensors::begin()
{
    pinMode(trigLeft, OUTPUT);
    pinMode(echoLeft, INPUT);

    pinMode(trigCenter, OUTPUT);
    pinMode(echoCenter, INPUT);

    pinMode(trigRight, OUTPUT);
    pinMode(echoRight, INPUT);
}

long UltrasonicSensors::readSensor(int trig, int echo)
{
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH, 30000);

    if(duration == 0)
        return -1;

    return duration * 0.0343 / 2;
}

void UltrasonicSensors::update()
{
    readings[0] = readSensor(trigLeft, echoLeft);
    delay(60);

    readings[1] = readSensor(trigCenter, echoCenter);
    delay(60);

    readings[2] = readSensor(trigRight, echoRight);
    delay(60);
}