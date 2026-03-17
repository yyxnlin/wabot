#include "DriveSystem.h"
#include <Arduino.h>

DriveSystem::DriveSystem(int enL,int l1,int l2,int enR,int r1,int r2)
{
    enLeft = enL;
    inLeft1 = l1;
    inLeft2 = l2;

    enRight = enR;
    inRight1 = r1;
    inRight2 = r2;
}

void DriveSystem::begin()
{
    pinMode(enLeft, OUTPUT);
    pinMode(inLeft1, OUTPUT);
    pinMode(inLeft2, OUTPUT);

    pinMode(enRight, OUTPUT);
    pinMode(inRight1, OUTPUT);
    pinMode(inRight2, OUTPUT);

    stop();
}

void DriveSystem::setSpeed(int s)
{
    speed = s;
}

void DriveSystem::moveForward()
{
    analogWrite(enLeft, speed);
    analogWrite(enRight, speed);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::moveLeft()
{
    analogWrite(enLeft, speed - 15);
    analogWrite(enRight, speed + 15);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::moveRight()
{
    analogWrite(enLeft, speed + 50);
    analogWrite(enRight, 0);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::moveHardLeft()
{
    analogWrite(enLeft, speed - 30);
    analogWrite(enRight, speed + 30);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::moveHardRight()
{
    analogWrite(enLeft, speed + 30);
    analogWrite(enRight, speed - 30);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::stop()
{
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);

    digitalWrite(inLeft1, LOW);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, LOW);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::navigate(long readings[])
{

    int threshold = 10;

    int left = (readings[0] != -1 && readings[0] < threshold);
    int center = (readings[1] != -1 && readings[1] < threshold);
    int right = (readings[2] != -1 && readings[2] < threshold);

    int state = (left << 2) | (center << 1) | right;

    Serial.print("Drive state: ");
    Serial.println(state);

    switch(state)
    {
        case 0: moveForward(); break;
        case 1: moveLeft(); break;
        case 2: moveHardLeft(); break;
        case 3: moveHardLeft(); break;
        case 4: moveRight(); break;
        case 5: moveForward(); break;
        case 6: moveHardRight(); break;
        case 7: stop(); break;
    }
}