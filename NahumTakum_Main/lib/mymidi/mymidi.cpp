// mymidi.cpp
#include "mymidi.h"

MyMIDI::MyMIDI() {
    // Initialize prevValues array
    for (int i = 0; i < NUM_INPUTS; i++) {
        prevValues[i] = -1; // Initialize to invalid values
    }
}

void MyMIDI::getPotentiometerInputs() {
    // Initialize control pins
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);

    // Loop to read inputs
    for (int i = 0; i < 12; i++) {
        digitalWrite(S0, i & 0x01);
        digitalWrite(S1, i & 0x02);
        digitalWrite(S2, i & 0x04);
        digitalWrite(S3, i & 0x08);
        delay(10); // Delay to stabilize the signal
        int currentValue = analogRead(SIG);

        // Compare current value with previous value for the same input
        if (abs(currentValue - prevValues[i]) >= 100) {
            // Map the current value to a range between 1 and 127
            int mappedValue = map(currentValue, 0, 4095, 1, 127);

            //Serial.print("Input ");
            //Serial.print(i);
            //Serial.print(": ");
            //Serial.println(mappedValue);
            prevValues[i] = currentValue; // Update prevValues for this input
            sendMIDI(176, i, mappedValue); // Send MIDI message
        }
    }
}

void MyMIDI::pairMidiToAbelton() {
    for (int i = 1; i <= 10; i++) {
        sendMIDI(176, i, 40);
        delay(1000);
        sendMIDI(176, i, 40);
        delay(1000);
        sendMIDI(176, i, 40);
        delay(1000);
        sendMIDI(176, i, 40);
        delay(1000);
        sendMIDI(176, i, 40);
        delay(1000);
        sendMIDI(176, i, 40);
        delay(1000);
        sendMIDI(176, i, 40);
        delay(1000);
        sendMIDI(176, i, 40);
        delay(1000);
    }
}

void MyMIDI::sendMIDI(unsigned char command, unsigned char note, unsigned char val) {
    Serial.write(command);
    Serial.write(note);
    Serial.write(val);
}
