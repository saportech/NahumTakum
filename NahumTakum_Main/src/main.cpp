#include <Arduino.h>
#include "espnowmaster.h"
#include "mymidi.h"
#include <FastLED.h>

//#define MAP_VALUES

#define LED_PIN 26
#define NUM_LEDS 5
#define SETUP_MAPPING_BUTTON_PIN 12
#define PLAY_BUTTON_PIN 14

CRGB leds[NUM_LEDS];

ESPNowMaster espNowMaster;
MyMIDI myMIDI;

bool setupButtonPressed = false;
bool playButtonPressed = true;

bool mappingPressed = true;

bool someButtonPressed = false;

void getButtonInputs();
void sendDataToComputer();
void setupLeds();
void turnOnLed(int ledIndex);

void setup() {

  Serial.begin(115200);
  Serial.println("Starting Master device");

  espNowMaster.init();

  pinMode(SETUP_MAPPING_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PLAY_BUTTON_PIN, INPUT_PULLUP);

  setupLeds();

}

void loop() {

  getButtonInputs();

  if (someButtonPressed) {
    myMIDI.getPotentiometerInputs();
  }
  else {
    sendDataToComputer();
  }

}

void sendDataToComputer() {
  int mapped_raw_pitch;
  int mapped_yaw;
  int specialAngleValue = 100;

  for (int i = 1; i <= NUM_SLAVES; i++) {
    SensorData data = espNowMaster.getSensorData(i);
    
    if (mappingPressed) {
      if (data.raw_pitch >= 1 && data.raw_pitch <= specialAngleValue)
          mapped_raw_pitch = map(data.raw_pitch, 1, specialAngleValue, 1, 126);
      else
          mapped_raw_pitch = 126;
    
    mapped_yaw = data.yaw;
    
    }
    else {
      mapped_raw_pitch = data.raw_pitch;
      mapped_yaw = data.yaw;
    }

    // Send mapped values via MIDI
    myMIDI.sendMIDI(176, i * 2 - 1, mapped_raw_pitch);
    myMIDI.sendMIDI(176, i * 2, mapped_yaw);
  }
}

void turnOnLed(int ledIndex) {

  leds[ledIndex] = CRGB::Green;

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i != ledIndex) {
      leds[i] = CRGB::Black;
    }
  }

  FastLED.show();

}

void setupLeds() {

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void getButtonInputs() {
  #ifndef MAP_VALUES
  if (digitalRead(SETUP_MAPPING_BUTTON_PIN) == LOW || digitalRead(PLAY_BUTTON_PIN) == LOW) {
    if (someButtonPressed) {
      someButtonPressed = false;
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
    }
    else {
      someButtonPressed = true;
      turnOnLed(1);
    }
    delay(500);
  }
  #else//MAPPING
  if (digitalRead(SETUP_MAPPING_BUTTON_PIN) == LOW) {
    //Serial.println("Setup button pressed");
    if (mappingPressed) {
      mappingPressed = false;
      leds[2] = CRGB::Black;
      FastLED.show();
    }
    else {
      mappingPressed = true;
      leds[2] = CRGB::Blue;
      FastLED.show();
    }
    delay(500);
  }
  else if (digitalRead(PLAY_BUTTON_PIN) == LOW) {
    //Serial.println("Play button pressed");
    setupButtonPressed = false;
    playButtonPressed = true;
    leds[1] = CRGB::Green;
    FastLED.show();
    delay(500);
  }
  #endif
}
