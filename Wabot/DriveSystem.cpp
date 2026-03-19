#include "DriveSystem.h"
#include <Arduino.h>

DriveSystem::DriveSystem(int enR,int r1,int r2,int enL,int l1,int l2)
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
    analogWrite(enLeft, 0);
    analogWrite(enRight, speed - 20);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::moveRight()
{
    analogWrite(enLeft, speed - 20);
    analogWrite(enRight, 0);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::moveHardLeft()
{
    analogWrite(enLeft, 0);
    analogWrite(enRight, speed - 20);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::moveHardRight()
{
    analogWrite(enLeft, speed - 20);
    analogWrite(enRight, 0);

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

    int threshold = 30;

    // Serial.print("Readings: ");
    // Serial.print(readings[0]);
    // Serial.print(" ");
    // Serial.print(readings[1]);
    // Serial.print(" ");
    // Serial.println(readings[2]);

    int left = (readings[0] != -1 && readings[0] < threshold);
    int center = (readings[1] != -1 && readings[1] < threshold);
    int right = (readings[2] != -1 && readings[2] < threshold);

    int state = (left << 2) | (center << 1) | right;

    Serial.print("Drive state: ");
    Serial.println(state);

    switch(state)
    {
        case 0: moveForward(); break;
        case 1: stop(); delay(500); moveLeft(); delay(500); stop(); break;
        case 2: stop(); delay(500); moveHardLeft(); delay(500); stop(); break;
        case 3: stop(); delay(500); moveHardLeft(); delay(500); stop(); break;
        case 4: stop(); delay(500); moveRight(); delay(500); stop(); break;
        case 5: stop(); delay(500); moveForward(); delay(500); stop(); break;
        case 6: stop(); delay(500); moveHardRight(); delay(500); stop(); break;
        case 7: stop(); break;
    }
}