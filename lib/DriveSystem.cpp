#include "DriveSystem.h"
#include <Arduino.h>

// enR: 3
// r1: 2
// r2: 12
// enL: 11
// l1: 13
// l2: A3

DriveSystem::DriveSystem(int enR, int r1, int r2, int enL, int l1, int l2)
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
    speedLeft = s - 15;
    speedRight = (int)(s * 1.2);

    speedLeft = constrain(speedLeft, 0, 255);
    speedRight = constrain(speedRight, 0, 255);
}

void DriveSystem::moveForward()
{
    analogWrite(enLeft, speedLeft);
    analogWrite(enRight, speedRight);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::moveLeft()
{
    analogWrite(enLeft, 0);
    analogWrite(enRight, speedRight);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::moveRight()
{
    analogWrite(enLeft, speedLeft);
    analogWrite(enRight, 0);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

// void DriveSystem::moveHardLeft()
// {
//     analogWrite(enLeft, 0);
//     analogWrite(enRight, speedRight);

//     digitalWrite(inLeft1, HIGH);
//     digitalWrite(inLeft2, LOW);

//     digitalWrite(inRight1, HIGH);
//     digitalWrite(inRight2, LOW);
// }

// void DriveSystem::moveHardRight()
// {
//     analogWrite(enLeft, speedLeft);
//     analogWrite(enRight, 0);

//     digitalWrite(inLeft1, HIGH);
//     digitalWrite(inLeft2, LOW);

//     digitalWrite(inRight1, HIGH);
//     digitalWrite(inRight2, LOW);
// }

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

    switch (state)
    {
    case 0:
        // 0b000: no obstacles; optionally undo the previous dodge, then continue forward.
        if (lastTurn == 1)
        {
            // Previous dodge was left, so apply a right correction.
            moveForward();
            delay(800);
            moveRight();
            delay(500);
            moveForward();
            delay(800);
            lastTurn = 0;
        }
        else if (lastTurn == 2)
        {
            // Previous dodge was right, so apply a left correction.
            moveForward();
            delay(800);
            moveLeft();
            delay(500);
            moveForward();
            delay(800);
            lastTurn = 0;
        }
        moveForward();
        break; // 0b000
    case 1:
        // 0b001: obstacle on right; dodge left.
        lastTurn = 1;
        stop();
        delay(500);
        moveLeft();
        delay(500);
        stop();
        break; // 0b001
    case 2:
        // 0b010: obstacle in center; dodge left.
        lastTurn = 1;
        stop();
        delay(500);
        moveLeft();
        delay(500);
        stop();
        break; // 0b010
    case 3:
        // 0b011: obstacles center + right; dodge left.
        lastTurn = 1;
        stop();
        delay(500);
        moveLeft();
        delay(500);
        stop();
        break; // 0b011
    case 4:
        // 0b100: obstacle on left; dodge right.
        lastTurn = 2;
        stop();
        delay(500);
        moveRight();
        delay(500);
        stop();
        break; // 0b100
    case 5:
        // 0b101: obstacles on both sides but center open; inch forward.
        stop();
        delay(500);
        moveForward();
        delay(500);
        stop();
        break; // 0b101
    case 6:
        // 0b110: obstacles left + center; dodge right.
        lastTurn = 2;
        stop();
        delay(500);
        moveRight();
        delay(500);
        stop();
        break; // 0b110
    case 7:
        // 0b111: blocked on all sides; hold position.
        stop();
        break;
    }
}