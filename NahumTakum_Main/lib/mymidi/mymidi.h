// mymidi.h
#ifndef MYMIDI_H
#define MYMIDI_H

#include "Arduino.h"

class MyMIDI {
  public:
    MyMIDI();
    void sendMIDI(unsigned char command, unsigned char note, unsigned char val);
    void getPotentiometerInputs();

  private:
    // Define control pins for the 16:1 MUX
    static const int S0 = 13;
    static const int S1 = 27;
    static const int S2 = 25;
    static const int S3 = 4;
    static const int SIG = 33; // Analog input pin to read from (change as needed)

    // Define the number of inputs on the MUX
    static const int NUM_INPUTS = 16;

    // Array to store previous input values
    int prevValues[NUM_INPUTS];
};

#endif
