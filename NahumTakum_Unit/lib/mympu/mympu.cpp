#include "mympu.h"
#include "Arduino.h"
#include <Wire.h>
#include <MPU6050_light.h>

#define MPU_SDA_PIN 21
#define MPU_SCL_PIN 22

MPU6050 mpu(Wire);

MyMPU::MyMPU() {
    yaw = 0.0;
    pitch = 0.0;
    roll = 0.0;
}

void MyMPU::begin() {
    Wire.begin();
    mpu.begin();
    mpu.calcOffsets();
}

void MyMPU::update() {
    mpu.update();
}

int MyMPU::getYaw() {
    float angle = mpu.getAngleZ();
    angle = fmod(angle, 720.0); // Wrap angle within [-360, 360]

    if (angle > 360.0)
        angle -= 720.0;
    else if (angle < -360.0)
        angle += 720.0;

    float mappedValue;
    if (angle >= 0)
        mappedValue = map(angle, 0, 360, 1, 126);
    else
        mappedValue = map(angle, -360, 0, 126, 1);

    int constrainedValue = constrain(static_cast<int>(mappedValue), 1, 126);

    return constrainedValue;
    
}

int MyMPU::getRoll() {
    float angleY =  mpu.getAngleY();
    return static_cast<int>(angleY);
}

int MyMPU::getPitch() {
    float angleX = mpu.getAngleX();
    return static_cast<int>(angleX);
}

int MyMPU::getSuperpositionValue() {
    // Get pitch and raw values
    float pitchValue = mpu.getAngleX();
    float rawValue = mpu.getAngleY();

    // Map pitch and raw values to range [1, 126]
    float mappedPitch = map(abs(pitchValue), 0, 100, 1, 126);
    float mappedRaw = map(abs(rawValue), 0, 90, 1, 126);

    // Calculate the superposition value
    float maxMappedValue = max(mappedPitch, mappedRaw);

    int superpositionValue = min(static_cast<int>(maxMappedValue), 126);

    return superpositionValue;
}