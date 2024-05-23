#include <Arduino.h>
#include "espnowmaster.h"
#include "mymidi.h"
#include <FastLED.h>

#define MAP_VALUES

#define LED_PIN 26
#define NUM_LEDS 5
#define SETUP_MAPPING_BUTTON_PIN 12
#define PLAY_BUTTON_PIN 14

CRGB leds[NUM_LEDS];

ESPNowMaster espNowMaster;
MyMIDI myMIDI;

bool setupButtonPressed = false;
bool playButtonPressed = false;

bool mappingPressed = false;

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

  if (setupButtonPressed) {
    myMIDI.getPotentiometerInputs();
  }
  else if (playButtonPressed) {
    sendDataToComputer();
  }

}

void sendDataToComputer() {
  int mapped_raw_pitch;
  int mapped_yaw;
  
  for (int i = 1; i <= NUM_SLAVES; i++) {
    SensorData data = espNowMaster.getSensorData(i);

    if (mappingPressed) {
      mapped_raw_pitch = map(data.raw_pitch, 1, 126, 20, 40);
      mapped_yaw = map(data.yaw, 1, 126, 20, 40);
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
  if (digitalRead(SETUP_MAPPING_BUTTON_PIN) == LOW) {
    //Serial.println("Setup button pressed");
    setupButtonPressed = true;
    playButtonPressed = false;
    turnOnLed(0);
    delay(500);
  }
  else if (digitalRead(PLAY_BUTTON_PIN) == LOW) {
    //Serial.println("Play button pressed");
    setupButtonPressed = false;
    playButtonPressed = true;
    turnOnLed(1);
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
