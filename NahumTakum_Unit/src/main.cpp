#include <Arduino.h>
#include "mympu.h"
#include "espnowslave.h"

#define MPU_SAMPLE_INTERVAL 20 // Sampling interval for MPU in milliseconds

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

  if (currentMillis - lastMPUSampleTime >= MPU_SAMPLE_INTERVAL) {

    lastMPUSampleTime = currentMillis;

    int yaw = mympu.getYaw();

    int raw_pitch = mympu.getSuperpositionValue();

    mympu.update();

    //Serial.print("Yaw: "); Serial.print(yaw);
    //Serial.print(" Pitch: "); Serial.println(raw_pitch);

    espnowslave.sendData(raw_pitch, yaw);

  }

}