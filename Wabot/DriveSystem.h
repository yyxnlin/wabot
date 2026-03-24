#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

/**
 * Differential-drive motor control + timed obstacle-avoidance sequences.
 * Pin wiring is documented in Wabot.ino next to the DriveSystem constructor.
 */
class DriveSystem
{
private:
    int enLeft, inLeft1, inLeft2;
    int enRight, inRight1, inRight2;
    int speedLeft;
    int speedRight;

    // Ultrasonic readings below this distance (cm) count as "obstacle present".
    const int obstacleThreshold = 20;
    // Standard detour timing (used when obstacle is clearly on one side).
    const int turnTimeMs = 450;
    const int passObstacleTimeMs = 900;
    // Longer moves when the center region is involved (see avoidObstacle codes 3–4).
    const int extraTurnTimeMs = 900;
    const int extraPassObstacleTimeMs = 1600;

    // whereObstacle: 1 = left, 2 = right, 3 = left-ish center, 4 = center / right-ish center
    void avoidObstacle(int whereObstacle);

public:
    DriveSystem(int enR, int r1, int r2, int enL, int l1, int l2);

    void begin();
    void setSpeed(int s);

    void moveForward();
    void moveLeft();
    void moveRight();
    void stop();

    /** readings[0]=left, [1]=center, [2]=right; distances in cm or -1 if invalid. */
    void navigate(long readings[]);
};

#endif
