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

void DriveSystem::moveBackward()
{
    analogWrite(enLeft, speedLeft);
    analogWrite(enRight, speedRight);

    digitalWrite(inLeft1, LOW);
    digitalWrite(inLeft2, HIGH);

    digitalWrite(inRight1, LOW);
    digitalWrite(inRight2, HIGH);
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

void DriveSystem::recordMove(DriveSystem::ActionType action, DriveSystem::Direction dir, int durationMs)
{
    if (moveCount >= maxMoves)
    {
        return;
    }
    moves[moveCount++] = DriveSystem::MovementState{action, dir, durationMs};
}

void DriveSystem::clearMoves()
{
    moveCount = 0;
}

void DriveSystem::executeRecorded(DriveSystem::ActionType action, DriveSystem::Direction dir, int durationMs)
{
    (void)action;
    switch (dir)
    {
    case DriveSystem::DIR_LEFT:
        moveLeft();
        break;
    case DriveSystem::DIR_RIGHT:
        moveRight();
        break;
    case DriveSystem::DIR_FORWARD:
        moveForward();
        break;
    case DriveSystem::DIR_BACKWARD:
        moveBackward();
        break;
    }
    delay(durationMs);
    stop();
    delay(50);
}

void DriveSystem::retraceMoves()
{
    // Reverse each recorded state to return to the original track:
    // left <-> right, forward -> backward.
    while (moveCount > 0)
    {
        DriveSystem::MovementState s = moves[moveCount - 1];
        moveCount--;

        DriveSystem::Direction reverseDir = s.dir;
        if (s.dir == DriveSystem::DIR_LEFT)
            reverseDir = DriveSystem::DIR_RIGHT;
        else if (s.dir == DriveSystem::DIR_RIGHT)
            reverseDir = DriveSystem::DIR_LEFT;
        else if (s.dir == DriveSystem::DIR_FORWARD)
            reverseDir = DriveSystem::DIR_BACKWARD;

        executeRecorded(s.action, reverseDir, s.durationMs);
    }
}

void DriveSystem::avoidObstacle(bool dodgeLeft)
{
    // Record the avoidance sequence so we can retrace it later once clear.
    clearMoves();

    // Turn away from obstacle.
    recordMove(DriveSystem::ACTION_TURN, dodgeLeft ? DriveSystem::DIR_LEFT : DriveSystem::DIR_RIGHT, turnTimeMs);
    executeRecorded(DriveSystem::ACTION_TURN, dodgeLeft ? DriveSystem::DIR_LEFT : DriveSystem::DIR_RIGHT, turnTimeMs);

    // Move forward (go around).
    recordMove(DriveSystem::ACTION_MOVE, DriveSystem::DIR_FORWARD, passObstacleTimeMs);
    executeRecorded(DriveSystem::ACTION_MOVE, DriveSystem::DIR_FORWARD, passObstacleTimeMs);

    // Turn back toward original direction.
    recordMove(DriveSystem::ACTION_TURN, dodgeLeft ? DriveSystem::DIR_RIGHT : DriveSystem::DIR_LEFT, turnTimeMs);
    executeRecorded(DriveSystem::ACTION_TURN, dodgeLeft ? DriveSystem::DIR_RIGHT : DriveSystem::DIR_LEFT, turnTimeMs);

    // Move forward (past obstacle).
    recordMove(DriveSystem::ACTION_MOVE, DriveSystem::DIR_FORWARD, passObstacleTimeMs);
    executeRecorded(DriveSystem::ACTION_MOVE, DriveSystem::DIR_FORWARD, passObstacleTimeMs);

    // Turn toward original path line.
    recordMove(DriveSystem::ACTION_TURN, dodgeLeft ? DriveSystem::DIR_RIGHT : DriveSystem::DIR_LEFT, turnTimeMs);
    executeRecorded(DriveSystem::ACTION_TURN, dodgeLeft ? DriveSystem::DIR_RIGHT : DriveSystem::DIR_LEFT, turnTimeMs);

    // Move forward (back to original path).
    recordMove(DriveSystem::ACTION_MOVE, DriveSystem::DIR_FORWARD, passObstacleTimeMs);
    executeRecorded(DriveSystem::ACTION_MOVE, DriveSystem::DIR_FORWARD, passObstacleTimeMs);

    // Final turn to face original travel direction.
    recordMove(DriveSystem::ACTION_TURN, dodgeLeft ? DriveSystem::DIR_LEFT : DriveSystem::DIR_RIGHT, turnTimeMs);
    executeRecorded(DriveSystem::ACTION_TURN, dodgeLeft ? DriveSystem::DIR_LEFT : DriveSystem::DIR_RIGHT, turnTimeMs);

    // Now wait until sensors say "clear", then retrace in navigate().
    pendingRetrace = true;
}

void DriveSystem::navigate(long readings[])
{
    // Serial.print("Readings: ");
    // Serial.print(readings[0]);
    // Serial.print(" ");
    // Serial.print(readings[1]);
    // Serial.print(" ");
    // Serial.println(readings[2]);

    int left = (readings[0] != -1 && readings[0] < obstacleThreshold);
    int center = (readings[1] != -1 && readings[1] < obstacleThreshold);
    int right = (readings[2] != -1 && readings[2] < obstacleThreshold);

    int state = (left << 2) | (center << 1) | right;

    Serial.print("Drive state: ");
    Serial.println(state);

    switch (state)
    {
    case 0:
        if (pendingRetrace)
        {
            retraceMoves();
            pendingRetrace = false;
        }
        moveForward();
        break; // 0b000
    case 1:
        // 0b001: obstacle on right; detour left and return.
        avoidObstacle(true);
        break; // 0b001
    case 2:
        // 0b010: obstacle in center; prefer left detour.
        avoidObstacle(true);
        break; // 0b010
    case 3:
        // 0b011: obstacles center + right; detour left and return.
        avoidObstacle(true);
        break; // 0b011
    case 4:
        // 0b100: obstacle on left; detour right and return.
        avoidObstacle(false);
        break; // 0b100
    case 5:
        // 0b101: both sides blocked but center open; continue forward.
        moveForward();
        break; // 0b101
    case 6:
        // 0b110: obstacles left + center; detour right and return.
        avoidObstacle(false);
        break; // 0b110
    case 7:
        // 0b111: blocked on all sides; hold position.
        stop();
        break;
    }
}