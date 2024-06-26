#ifndef MYMPU_H
#define MYMPU_H

#include "Arduino.h"
#include <Wire.h>

class MyMPU {
public:
    MyMPU();
    void begin();
    void update();
    int getYaw();
    int getPitch();
    int getRoll();
    int getSuperpositionValue();
    int getZAngleToGround();
    int getYaw2();
    int getYaw120();
private:

    float yaw;
    float pitch;
    float roll;
    
};

#endif
