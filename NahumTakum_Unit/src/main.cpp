#include <Arduino.h>
#include "mympu.h"
#include "espnowslave.h"

#define SAMPLE_INTERVAL 20

MyMPU mympu;
ESPNowSlave espnowslave;

unsigned long lastMPUSampleTime = 0;

void setup() {

  Serial.begin(115200);
  Serial.println("Starting Slave device");

  mympu.begin();

  espnowslave.init();

}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - lastMPUSampleTime >= SAMPLE_INTERVAL) {

    lastMPUSampleTime = currentMillis;

    //int yaw = mympu.getYaw();
    int yaw90 = mympu.getYaw2();
    //int yaw120 = mympu.getYaw120();

    int angleToGround = mympu.getZAngleToGround();

    mympu.update();

    // Serial.print(" Yaw: "); Serial.print(yaw);
    // Serial.print(" Yaw90: "); Serial.print(yaw90);
    // Serial.print(" Yaw120: "); Serial.print(yaw120);
    // Serial.print("Angle to ground: "); Serial.println(angleToGround);

    espnowslave.sendData(angleToGround, yaw90);

  }

}