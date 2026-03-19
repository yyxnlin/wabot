#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

class DriveSystem {

private:
    int enLeft, inLeft1, inLeft2;
    int enRight, inRight1, inRight2;

    int speed = 200;

public:
    DriveSystem(int enR, int r1, int r2, int enL, int l1, int l2);

    void begin();
    void setSpeed(int s);

    void moveForward();
    void moveLeft();
    void moveRight();
    void moveHardLeft();
    void moveHardRight();
    void stop();

    void navigate(long readings[]);
};

#endif