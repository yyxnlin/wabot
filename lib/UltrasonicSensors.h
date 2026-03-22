#ifndef ULTRASONICSENSORS_H
#define ULTRASONICSENSORS_H

class UltrasonicSensors {

private:
    int trigLeft, echoLeft;
    int trigCenter, echoCenter;
    int trigRight, echoRight;

    long readSensor(int trig, int echo);

public:
    long readings[3];

    UltrasonicSensors(int tl, int el, int tc, int ec, int tr, int er);

    void begin();
    void update();
    long median(long a, long b, long c);
};

#endif