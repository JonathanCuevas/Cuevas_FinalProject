#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=222,227
AudioOutputAnalog        dac1;           //xy=446,226
AudioConnection          patchCord1(waveform1, dac1);
// GUItool: end automatically generated code


int ledPins[8] = { 23, 21, 19, 17, 15, 13, 39, 37 };
int totalLeds = 8;

int ledSeqPins[4] = { 6, 8, 10, 12 };
int totalSeqLeds = 4;

int currentStep = 0;
unsigned long lastStepTime = 0;

int tempo = 0;

int currentChannel = 0;

int buttonPin1 = A12;
bool lastButtonState1 = LOW;
bool buttonState1 = LOW;

int buttonPin2 = A13;
bool lastButtonState2 = LOW;
bool buttonState2 = LOW;

int slideswitchPin = 29;
int slideswitchPin2 = 30;
int slideswitchPin3 = 28;

int potPins[8] = { A8, A6, A4, A2, A0, A22, A19, A17 };
int totalPots = 8;

int potVals[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int totalPotVals = 8;

int mappedPotVals[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int totalMappedPotVals = 8;

int potStepPin = A16;
int potStepVal = 0;
int mappedPotStepVal = 0;

int minPitch = 0;
int maxPitch = 0;

int sequences[4] = { 1, 2, 3, 4 };
int totalSequences = 4;


boolean on = false;


void setup() {
  AudioMemory(12);
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(0.4);
  waveform1.frequency(262);

  pinMode(slideswitchPin, INPUT);
  pinMode(slideswitchPin2, INPUT);
  pinMode(slideswitchPin3, INPUT);
  pinMode(potStepPin, INPUT);

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  for (int i = 0; i < totalLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  for (int i = 0; i < totalSeqLeds; i++) {
    pinMode(ledSeqPins[i], OUTPUT);
  }
}

void loop() {
  if (digitalRead(slideswitchPin3) == HIGH) { // This is responsible for toggling between the live sequence and the first set sequence and their respective LED indicators
    checkButton1();
    digitalWrite(ledSeqPins[2], LOW);
    digitalWrite(ledSeqPins[3], LOW);
  } else { // This is responsible for toggling between the third and fourth set sequences and their respective LED indicators
    checkButton2();
    digitalWrite(ledSeqPins[0], LOW);
    digitalWrite(ledSeqPins[1], LOW);
  }
}


void checkButton1() { // If the switch is high, pressing this button will switch between the live sequence and the first set sequence
  lastButtonState1 = buttonState1;
  buttonState1 = digitalRead(buttonPin1);

  if (buttonState1 == HIGH && lastButtonState1 == LOW) {
    if (on == false) {
      on = true;
    } else if (on == true) {
      on = false;
    }
  }


  if (on == false) {
    liveSequence();
    digitalWrite(ledSeqPins[0], HIGH);
    digitalWrite(ledSeqPins[1], LOW);
  } else if (on == true) {
    setSequence1();
    digitalWrite(ledSeqPins[1], HIGH);
    digitalWrite(ledSeqPins[0], LOW);
  }
}

void checkButton2() { // If the switch is low, pressing this button will switch between the third and fourth set sequences
  lastButtonState2 = buttonState2;
  buttonState2 = digitalRead(buttonPin2);

  if (buttonState2 == HIGH && lastButtonState2 == LOW) {
    if (on == false) {
      on = true;
    } else if (on == true) {
      on = false;
    }
  }


  if (on == false) {
    setSequence2();
    digitalWrite(ledSeqPins[2], HIGH);
    digitalWrite(ledSeqPins[3], LOW);
  } else if (on == true) {
    setSequence3();
    digitalWrite(ledSeqPins[3], HIGH);
    digitalWrite(ledSeqPins[2], LOW);
  }
}

void sequence() { // This is the basis of the entire step sequencer when going forward
  potStepVal = analogRead(potStepPin);
  mappedPotStepVal = map(potStepVal, 0, 1023, 0, 7);

  tempo = analogRead(A14);

  if (digitalRead(slideswitchPin) == LOW) {

    if (millis() > lastStepTime + tempo) {
      currentStep = currentStep + 1;



      if (currentStep > mappedPotStepVal) {
        currentStep = 0;
      }


      lastStepTime = millis();
    }
  }
}


void sequenceBackward() { // This is the basis of the entire step sequencer when going backward
  potStepVal = analogRead(potStepPin);
  mappedPotStepVal = map(potStepVal, 0, 1023, 0, 7);

  tempo = analogRead(A14);

  if (digitalRead(slideswitchPin) == HIGH) {

    if (millis() > lastStepTime + tempo) {
      currentStep = currentStep - 1;

      if (currentStep < mappedPotStepVal) {
        currentStep = 7;
      }

      lastStepTime = millis();
    }
  }
}


void setLeds() { // This controls the LEDs of the step sequencer
  for (int i = 0; i < totalLeds; i++) {

    if (i == currentStep) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}


void liveSequence() { // This is the live sequence
  sequence();
  sequenceBackward();
  setLeds();

  // This controls the quantized pitches of the potentiometers
  for (int i = 0; i < totalPots; i++) {

    if (i == currentStep) {
      potVals[i] = analogRead(potPins[i]);
      mappedPotVals[i] = map(potVals[i], 0, 1023, 0, 12);
      waveform1.frequency(minPitch * pow(2, mappedPotVals[i] / 12.0));
    }
  }


  //This controls the octave of the pitches
  if (digitalRead(slideswitchPin2) == HIGH) {
    minPitch = 252;
    maxPitch = 523;
  } else {
    minPitch = 523;
    maxPitch = 1047;
  }
}





void setSequence1() { // This is the first set sequence
  sequence();
  sequenceBackward();
  setLeds();

  for (int i = 0; i < totalLeds; i++) {

    if (i == currentStep) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }

    if (currentStep == 0) {
      waveform1.frequency(311);
    }

    if (currentStep == 1) {
      waveform1.frequency(466);
    }

    if (currentStep == 2) {
      waveform1.frequency(622);
    }

    if (currentStep == 3) {
      waveform1.frequency(466);
    }

    if (currentStep == 4) {
      waveform1.frequency(784);
    }

    if (currentStep == 5) {
      waveform1.frequency(466);
    }

    if (currentStep == 6) {
      waveform1.frequency(622);
    }

    if (currentStep == 7) {
      waveform1.frequency(784);
    }


  }
}


void setSequence2() { // This is the second set sequence
  sequence();
  sequenceBackward();
  setLeds();

  for (int i = 0; i < totalLeds; i++) {

    if (i == currentStep) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }

    if (currentStep == 0) {
      waveform1.frequency(262);
    }

    if (currentStep == 1) {
      waveform1.frequency(330);
    }

    if (currentStep == 2) {
      waveform1.frequency(392);
    }

    if (currentStep == 3) {
      waveform1.frequency(524);
    }

    if (currentStep == 4) {
      waveform1.frequency(660);
    }

    if (currentStep == 5) {
      waveform1.frequency(392);
    }

    if (currentStep == 6) {
      waveform1.frequency(524);
    }

    if (currentStep == 7) {
      waveform1.frequency(660);
    }


  }
}


void setSequence3() { // This is the third set sequence
  sequence();
  sequenceBackward();
  setLeds();

  for (int i = 0; i < totalLeds; i++) {

    if (i == currentStep) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }

    if (currentStep == 0) {
      waveform1.frequency(262);
    }

    if (currentStep == 1) {
      waveform1.frequency(294);
    }

    if (currentStep == 2) {
      waveform1.frequency(440);
    }

    if (currentStep == 3) {
      waveform1.frequency(588);
    }

    if (currentStep == 4) {
      waveform1.frequency(699);
    }

    if (currentStep == 5) {
      waveform1.frequency(440);
    }

    if (currentStep == 6) {
      waveform1.frequency(588);
    }

    if (currentStep == 7) {
      waveform1.frequency(699);
    }


  }
}

