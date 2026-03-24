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

void DriveSystem::stop()
{
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);

    digitalWrite(inLeft1, LOW);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, LOW);
    digitalWrite(inRight2, LOW);
}

void DriveSystem::avoidObstacle(int whereObstacle)
{
    // 1 = left
    // 2 = right
    // 3 = left-ish center
    // 4 = right-ish center or super center

    // Full detour with forward-only segments (no reverse).
    if (whereObstacle == 2) // obstacle in the right
    {
        moveLeft();
        delay(turnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(passObstacleTimeMs);
        stop();
        delay(50);
        moveRight();
        delay(turnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(passObstacleTimeMs);
        stop();
        delay(50);
        moveRight();
        delay(turnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(passObstacleTimeMs);
        stop();
        delay(50);
        moveLeft();
        delay(turnTimeMs);
        stop();
        delay(50);
    }
    else if (whereObstacle == 1) // obstacle in the left
    {
        moveRight();
        delay(turnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(passObstacleTimeMs);
        stop();
        delay(50);
        moveLeft();
        delay(turnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(passObstacleTimeMs);
        stop();
        delay(50);
        moveLeft();
        delay(turnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(passObstacleTimeMs);
        stop();
        delay(50);
        moveRight();
        delay(turnTimeMs);
        stop();
        delay(50);
    }
    else if (whereObstacle == 3) // obstacle in left ish center
    {
        moveRight();
        delay(extraTurnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(extraPassObstacleTimeMs);
        stop();
        delay(50);
        moveLeft();
        delay(extraTurnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(extraPassObstacleTimeMs);
        stop();
        delay(50);
        moveLeft();
        delay(extraTurnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(extraPassObstacleTimeMs);
        stop();
        delay(50);
        moveRight();
        delay(extraTurnTimeMs);
        stop();
        delay(50);
    }
    else // obstacle in right ish center or center
    {
        moveRight();
        delay(extraTurnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(extraPassObstacleTimeMs);
        stop();
        delay(50);
        moveLeft();
        delay(extraTurnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(extraPassObstacleTimeMs);
        stop();
        delay(50);
        moveLeft();
        delay(extraTurnTimeMs);
        stop();
        delay(50);
        moveForward();
        delay(extraPassObstacleTimeMs);
        stop();
        delay(50);
        moveRight();
        delay(extraTurnTimeMs);
        stop();
        delay(50);
    }
    moveForward();
}

void DriveSystem::navigate(long readings[])
{
    // 1 = left
    // 2 = right
    // 3 = left-ish center
    // 4 = right-ish center or super center
    int left = (readings[0] != -1 && readings[0] < obstacleThreshold);
    int center = (readings[1] != -1 && readings[1] < obstacleThreshold);
    int right = (readings[2] != -1 && readings[2] < obstacleThreshold);

    int state = (left << 2) | (center << 1) | right;

    Serial.print("Drive state: ");
    Serial.println(state);

    switch (state)
    {
    case 0:
        moveForward();
        break;
    case 1: // 0 0 1 obstacle in the right
        avoidObstacle(2);
        break;
    case 2: // 0 1 0 obtacle in dead center
        avoidObstacle(4);
        break;
    case 3: // 0 1 1 obstacle in right-ish center 
        avoidObstacle(4);
        break;
    case 4: // 1 0 0 obstacle in left
        avoidObstacle(1);
        break;
    case 5: // 1 0 1 
        moveForward();
        break;
    case 6: // 1 1 0 left ish center
        avoidObstacle(3);
        break;
    case 7:
        stop();
        break;
    }
}
