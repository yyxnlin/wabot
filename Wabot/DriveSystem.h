#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

class DriveSystem
{

private:
    int enLeft, inLeft1, inLeft2;
    int enRight, inRight1, inRight2;
    int speedLeft;
    int speedRight;
    const int obstacleThreshold = 20;   // Distance (cm) for obstacle detection.
    const int turnTimeMs = 450;         // Turn duration (ms) for each left/right turn in the sequence.
    const int passObstacleTimeMs = 900; // Forward duration (ms) for each straight segment in the sequence.
    const int extraTurnTimeMs = 900;
    const int extraPassObstacleTimeMs = 1600;


    void avoidObstacle(int whereObstacle);

public:
    DriveSystem(int enR, int r1, int r2, int enL, int l1, int l2);

    void begin();
    void setSpeed(int s);

    void moveForward();
    void moveLeft();
    void moveRight();
    void stop();

    void navigate(long readings[]);
};

#endif
