#include "DriveSystem.h"
#include <Arduino.h>

// Motor wiring (matches Wabot.ino): enR=3, r1=2, r2=12 | enL=11, l1=13, l2=A3
// Constructor takes these pin numbers as arguments and stores them in member variables for later use in controlling the motors.
DriveSystem::DriveSystem(int enR, int r1, int r2, int enL, int l1, int l2)
{
    enLeft = enL;
    inLeft1 = l1;
    inLeft2 = l2;

    enRight = enR;
    inRight1 = r1;
    inRight2 = r2;
}

// Initializes the motor control pins by setting the enable pins as OUTPUT and the direction pins as OUTPUT, and then stops the motors to ensure the robot starts in a stationary state.
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

// Sets the speed for the left and right motors based on the input speed value (0-255). The left motor speed is slightly reduced to help the robot drive straight, and both speeds are constrained to the valid range of 0-255.
void DriveSystem::setSpeed(int s)
{
    // Slight asymmetry to help the chassis drive straight (tune if it drifts).
    speedLeft = s - 15;
    speedRight = (int)(s * 1.2);

    speedLeft = constrain(speedLeft, 0, 255); // Ensure the speed values are within the valid range for PWM (0-255).
    speedRight = constrain(speedRight, 0, 255); // Constrain the right motor speed as well.
}

// Function to move the robot forward. Both motors are set to the current speed, and the direction pins are set to drive forward (inLeft1 HIGH, inLeft2 LOW for left motor; inRight1 HIGH, inRight2 LOW for right motor).
void DriveSystem::moveForward()
{
    analogWrite(enLeft, speedLeft);
    analogWrite(enRight, speedRight);

    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

// Function to pivot the robot to the left by turning off the left motor and driving the right motor forward. The direction pins are set to make sure both wheels are in turning forward direction.
void DriveSystem::moveLeft()
{
    // Pivot-style turn: left wheel off, right wheel drives.
    analogWrite(enLeft, 0);
    analogWrite(enRight, speedRight);

    // The left motor is off, but set the direction pins to a consistent state (forward) in case of any brief unintended movement.
    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    // The right motor drives forward.
    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

// Function to pivot the robot to the right by turning off the right motor and driving the left motor forward. The direction pins are set to make sure both wheels are in turning forward direction.
void DriveSystem::moveRight()
{
    // Pivot-style turn: right wheel off, left wheel drives.
    analogWrite(enLeft, speedLeft);
    analogWrite(enRight, 0);

    // The left motor drives forward.
    digitalWrite(inLeft1, HIGH);
    digitalWrite(inLeft2, LOW);

    // The right motor is off, but set the direction pins to a consistent state (forward) in case of any brief unintended movement.
    digitalWrite(inRight1, HIGH);
    digitalWrite(inRight2, LOW);
}

// Function to stop the robot by setting both motors' speeds to 0 and setting all direction pins to LOW to ensure the motors are not driven in any direction.
void DriveSystem::stop()
{
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);

    digitalWrite(inLeft1, LOW);
    digitalWrite(inLeft2, LOW);

    digitalWrite(inRight1, LOW);
    digitalWrite(inRight2, LOW);
    delay(50); // Short delay to ensure the motors have time to stop before any new commands are issued.

}

// Function to execute an obstacle avoidance maneuver based on the location of the detected obstacle. The whereObstacle parameter indicates the position of the obstacle (1 = left, 2 = right, 3 = left-ish center, 4 = center/right-ish center), and the function performs a series of movements to try to navigate around the obstacle while maintaining forward progress.
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

    // Print the location of the detected obstacle for debugging purposes.
    Serial.print("whereobstacle: ");
    Serial.println(whereObstacle);

    // --- Obstacle on the left: steer right, then forward, then left to re-center, then forward again to clear, then a small right turn to straighten. ---
    if (whereObstacle == 1)
    {
        stop();
        delay(150);
        moveRight(); // Turn right to avoid the obstacle on the left.
        delay(turnTimeMs);
        stop();
        moveForward(); // Move forward to clear the obstacle after the initial right turn.
        delay(passObstacleTimeMs);
        stop();
        moveLeft(); // Turn left to try to re-center after passing the obstacle.
        delay(turnTimeMs);
        stop();
        moveForward(); // Move forward again to ensure the robot straightens back on track.
        delay(passObstacleTimeMs);
        stop();
        moveLeft(); // Additional left turn to ensure the robot is back on the initial path.
        delay(turnTimeMs);
        stop();
        moveForward(); // Final forward move to clear the area after re-centering.
        delay(passObstacleTimeMs);
        stop();
        moveRight(); // Small right turn to straighten the robot's path after the detour.
        delay(275);
        stop();
    }

    // --- Obstacle on the right: mirror of case 1 ---
    else if (whereObstacle == 2)
    {
        stop();
        delay(50);
        moveLeft(); // Turn left to avoid the obstacle on the right.
        delay(turnTimeMs);
        stop();
        delay(50);
        moveForward(); // Move forward to clear the obstacle after the initial left turn.
        delay(passObstacleTimeMs);
        stop();
        delay(50);
        moveRight(); // Turn right to try to re-center after passing the obstacle.
        delay(turnTimeMs);
        stop();
        delay(50);
        moveForward(); // Move forward again to ensure the robot straightens back on track.
        delay(passObstacleTimeMs);
        stop();
        delay(50);
        moveRight(); // Additional right turn to ensure the robot is back on the initial path.
        delay(turnTimeMs);
        stop();
        delay(50);
        moveForward(); // Final forward move to clear the area after re-centering.
        delay(passObstacleTimeMs);
        stop();
        delay(50);
        moveLeft(); // Small left turn to straighten the robot's path after the detour.
        delay(turnTimeMs);
        stop();
        delay(50);
    }

    // --- Center-heavy with possibly Left Obstacles: longer turn and forward durations ---
    else if (whereObstacle == 3)
    {
        stop();
        delay(50);
        moveRight(); // Turn right to avoid the obstacle that is primarily in the center but also slightly on the left.
        delay(extraTurnTimeMs); // Use extra turn time to ensure clearance of the center obstacle.
        stop();
        delay(50);
        moveForward(); // Move forward to clear the obstacle after the initial right turn.
        delay(extraPassObstacleTimeMs); // Use extra forward time to ensure the robot has enough time to clear the center obstacle.
        stop();
        delay(50);
        moveLeft(); // Turn left to try to re-center after passing the obstacle.
        delay(extraTurnTimeMs); // Use extra turn time to ensure the robot is properly re-centered after the longer detour.
        stop();
        delay(50);
        moveForward(); // Move forward again to ensure the robot straightens back on track after the longer detour.
        delay(extraPassObstacleTimeMs); // Use extra forward time to ensure the robot has enough time to straigthen back on path.
        stop();
        delay(50);
        moveLeft(); // Additional left turn to ensure the robot is back on the initial path.
        delay(extraTurnTimeMs);
        stop();
        delay(50);
        moveForward(); // Final forward move to clear the area after re-centering.
        delay(extraPassObstacleTimeMs);
        stop();
        delay(50);
        moveRight(); // Small right turn to straighten the robot's path after the detour.
        delay(extraTurnTimeMs);
        stop();
        delay(50);
    }
    // --- Center and right-heavy: mirror of 3 but with turns to left to avoid obstacles . ---
    else if (whereObstacle == 4)
    {
        stop();
        delay(50);
        moveLeft(); // Turn left to avoid the obstacle that is primarily in the center but also slightly on the right.m
        delay(extraTurnTimeMs); // Use extra turn time to ensure clearance of the center obstacle.
        stop();
        delay(50);
        moveForward(); // Move forward to clear the obstacle after the initial left turn.
        delay(extraPassObstacleTimeMs); // Use extra forward time to ensure the robot has enough time to clear the center obstacle.
        stop();
        delay(50);
        moveRight(); // Turn right to try to re-center after passing the obstacle.
        delay(extraTurnTimeMs); // Use extra turn time to ensure the robot is properly re-centered after the longer detour.
        stop();
        delay(50);
        moveForward(); // Move forward again to ensure the robot straightens back on track after the longer detour.
        delay(extraPassObstacleTimeMs); // Use extra forward time to ensure the robot has enough time to straigthen back on path.
        stop();
        delay(50);
        moveRight(); // Additional right turn to ensure the robot is back on the initial path.
        delay(extraTurnTimeMs);
        stop();
        delay(50);
        moveForward(); // Final forward move to clear the area after re-centering.
        delay(extraPassObstacleTimeMs);
        stop();
        delay(50);
        moveLeft(); // Small left turn to straighten the robot's path after the detour.
        delay(extraTurnTimeMs);
        stop();
        delay(50);
    }

    moveForward();
}

// Function to navigate based on ultrasonic sensor readings. It checks the left, center, and right sensor readings against the obstacle threshold to determine if there is an obstacle in each direction, and then uses a switch statement to decide how to navigate (e.g., move forward, turn left, turn right, or stop) based on the combination of obstacles detected.
void DriveSystem::navigate(long readings[])
{
    // Booleans: True (1) if an obstacle is detected in that direction (reading is valid and below threshold).
    // If a reading is -1 (invalid), treat it as "no obstacle" to avoid overreacting to sensor errors.
    int left = (readings[0] != -1 && readings[0] < obstacleThreshold);
    int center = (readings[1] != -1 && readings[1] < obstacleThreshold);
    int right = (readings[2] != -1 && readings[2] < obstacleThreshold);

    // 3-bit pattern: bits [left][center][right] — used as switch index 0..7. For example, if left and center detect obstacles but right is clear, the pattern would be 110 in binary, which is 6 in decimal.
    int state = (left << 2) | (center << 1) | right;

    // Print the state of obstacle detection and resulting drive state for debugging purposes. This helps to understand how the robot is interpreting the sensor readings and which navigation decision it is making.
    Serial.print("Drive state: ");
    Serial.println(state);


    // Decide navigation action based on the state variable which describes the combination of obstacles detected. The cases are designed to handle different scenarios of obstacle presence, with specific maneuvers for each case to try to navigate around the obstacles while maintaining forward progress when possible.
    // States are from 0-7 with bits representing (left, center, right) obstacle presence. For example, state 5 (0b101) means obstacles on left and right but not center, which might indicate a narrow gap that the robot can still pass through by moving forward.
    switch (state)
    {
    case 0: // 000 clear
        moveForward(); // No obstacles detected, move forward.
        break;
    case 1: // 001 right only
        avoidObstacle(2); // Dodge to the left if there's an obstacle on the right.
        break;
    case 2: // 010 center only
        avoidObstacle(4); // Treat center-only as the most severe case, same detour as 3 with longer timings to avoid the center.
        break;
    case 3: // 011 center + right
        avoidObstacle(4); // Same detour as 2 (center-only), since the center obstacle is the critical factor.
        break;
    case 4: // 100 left only
        avoidObstacle(1); // Dodge to the right if there's an obstacle on the left.
        break;
    case 5: // 101 left + right (narrow gap)
        moveForward(); // Obstacles on both sides but not center, try to move forward through the gap.
        break;
    case 6: // 110 left + center
        avoidObstacle(3); // Similar to center-only but with an obstacle on the left as well, so turn right instead use the longer detour timings to ensure clearance.
        break;
    case 7: // 111 all blocked
        moveRight(); // Try to turn right first to see if we can find a way around the obstacles. Use the longest detour timings in avoidObstacle to try to navigate around the cluster of obstacles.
        delay(extraTurnTimeMs);
        stop();
        moveForward(); // After the initial turn, try moving forward to see if we can get past the obstacles.
        delay(extraPassObstacleTimeMs);
        stop();
        avoidObstacle(3); // Treat all-blocked after turning right as the left + center obstacle and complete the same detour as case 6.
        break;
    }
}
