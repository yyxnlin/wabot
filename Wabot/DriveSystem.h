#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

class DriveSystem
{

public:
    enum ActionType 
    {
        ACTION_TURN,
        ACTION_MOVE
    };

    enum Direction
    {
        DIR_LEFT,
        DIR_RIGHT,
        DIR_FORWARD,
        DIR_BACKWARD
    };

    struct MovementState
    {
        ActionType action;
        Direction dir;
        int durationMs;
    };

    int enLeft, inLeft1, inLeft2;
    int enRight, inRight1, inRight2;
    int speedLeft;
    int speedRight;
    const int obstacleThreshold = 10;   // Distance (cm) for obstacle detection.
    const int turnTimeMs = 450;         // Turn duration (ms) for each left/right turn in the sequence.
    const int passObstacleTimeMs = 900; // Forward duration (ms) for each straight segment in the sequence.

    static const int maxMoves = 40;
    MovementState moves[maxMoves];
    int moveCount = 0;
    bool pendingRetrace = false;

    void recordMove(ActionType action, Direction dir, int durationMs);
    void clearMoves();
    void executeRecorded(ActionType action, Direction dir, int durationMs);
    void retraceMoves();
    void avoidObstacle(bool dodgeLeft);

public:
    DriveSystem(int enR, int r1, int r2, int enL, int l1, int l2);

    void begin();
    void setSpeed(int s);

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    // void moveHardLeft();
    // void moveHardRight();
    void stop();

    void navigate(long readings[]);
};

#endif