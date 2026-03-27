#include "DriveSystem.h"
#include <Arduino.h>

// Motor wiring (matches Wabot.ino): enR=3, r1=2, r2=12 | enL=11, l1=13, l2=A3

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
    // Slight asymmetry to help the chassis drive straight (tune if it drifts).
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
    // Pivot-style turn: left wheel off, right wheel drives.
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
    delay(50);

}

void DriveSystem::avoidObstacle(int whereObstacle)
{
    
    /*
     * whereObstacle selects which detour template to run (same physical sequence shape,
     * different timings / mirror). Forward-only segments (no reverse driving).
     *
     * 1 = obstacle on left    -> dodge to the right (standard timings)
     * 2 = obstacle on right   -> dodge to the left  (standard timings)
     * 3 = left-ish center     -> longer turns/straights, same shape as 4
     * 4 = center / right-ish  -> same as 3 (extra timings)
     */


    Serial.print("whereobstacle: ");
    Serial.println(whereObstacle);
    // --- Obstacle on the left: step right, forward, weave, end aligned forward ---
    if (whereObstacle == 1)
    {
        stop();
        delay(150);
        moveRight();
        delay(turnTimeMs);
        stop();
        moveForward();
        delay(passObstacleTimeMs);
        stop();
        moveLeft();
        delay(turnTimeMs);
        stop();
        moveForward();
        delay(passObstacleTimeMs);
        stop();
        moveLeft();
        delay(turnTimeMs);
        stop();
        moveForward();
        delay(passObstacleTimeMs);
        stop();
        moveRight();
        delay(275);
        stop();
    }

    // --- Obstacle on the right: mirror of case 1 ---
    else if (whereObstacle == 2)
    {
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

    // --- Center-heavy: longer turn and forward durations (same path family as final else) ---
    else if (whereObstacle == 3)
    {
        stop();
        delay(50);
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
    else if (whereObstacle == 4)
    {
        // whereObstacle == 4 (or any other code): same geometry as 3, extra timings
        stop();
        delay(50);
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
    // Booleans: 1 if that sensor sees something closer than obstacleThreshold.
    int left = (readings[0] != -1 && readings[0] < obstacleThreshold);
    int center = (readings[1] != -1 && readings[1] < obstacleThreshold);
    int right = (readings[2] != -1 && readings[2] < obstacleThreshold);

    // 3-bit pattern: bits [left][center][right] — used as switch index 0..7
    int state = (left << 2) | (center << 1) | right;

    Serial.print("Drive state: ");
    Serial.println(state);

    // state = 0..7: bits (left, center, right) as LCR e.g. state 6 = 0b110 = left+center
    switch (state)
    {
    case 0: // 000 clear
        moveForward();
        break;
    case 1: // 001 right only
        avoidObstacle(2);
        break;
    case 2: // 010 center only
        avoidObstacle(4);
        break;
    case 3: // 011 center + right
        avoidObstacle(4);
        break;
    case 4: // 100 left only
        avoidObstacle(1);
        break;
    case 5: // 101 left + right (narrow gap)
        moveForward();
        break;
    case 6: // 110 left + center
        avoidObstacle(3);
        break;
    case 7: // 111 all blocked
        stop();
        break;
    }
}
