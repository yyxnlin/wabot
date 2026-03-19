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


long UltrasonicSensors::median(long a, long b, long c)
{
    long temp;

    if (a > b) {
        temp = a;
        a = b;
        b = temp;
    }

    if (b > c) {
        temp = b;
        b = c;
        c = temp;
    }

    if (a > b) {
        temp = a;
        a = b;
        b = temp;
    }

    return b;
}

long UltrasonicSensors::readSensor(int trig, int echo)
{
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH, 10000);

    if(duration == 0)
        return -1;

    return duration * 0.0343 / 2;
}

void UltrasonicSensors::update()
{
    // LEFT sensor (3 samples)
    long a1 = readSensor(trigLeft, echoLeft);
    long b1 = readSensor(trigLeft, echoLeft);
    long c1 = readSensor(trigLeft, echoLeft);
    readings[0] = median(a1, b1, c1);

    // delay(250); // allow echoes to clear

    // CENTER sensor (3 samples)
    long a2 = readSensor(trigCenter, echoCenter);
    long b2 = readSensor(trigCenter, echoCenter);
    long c2 = readSensor(trigCenter, echoCenter);
    readings[1] = median(a2, b2, c2);

    // delay(250);

    // RIGHT sensor (3 samples)
    long a3 = readSensor(trigRight, echoRight);
    long b3 = readSensor(trigRight, echoRight);
    long c3 = readSensor(trigRight, echoRight);
    readings[2] = median(a3, b3, c3);

    // delay(250);
}