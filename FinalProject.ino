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

int slideswitchPin = 33;

int potPins[8] = { 22, 20, 18, 16, 14, 38, 36 };
int totalPots = 8;

int potVals[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int totalPotVals = 8;

int mappedPotVals[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int totalMappedPotVals = 8;

int minPitch = 0;
int maxPitch = 0;

boolean stepState[8] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };

void setup() {
  AudioMemory(12);
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(0.4);
  waveform1.frequency(262);

  pinMode(slideswitchPin, INPUT);

  for (int i = 0; i < totalLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  for (int i = 0; i < totalSeqLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  pitch();
  sequence();
  setLeds();
  currentSequence();
  currentSequenceBackward();
  currentSequenceLeds();
}

void sequence() {

  tempo = analogRead(A14);

  if (millis() > lastStepTime + tempo) {
    currentStep = currentStep + 1;

    if (currentStep > 7) {
      currentStep = 0;
    }

    //      if (stepState[currentStep] == HIGH) {
    //        usbMIDI.sendNoteOn(mappedPotVals[i], 127, 1);
    //      }

    lastStepTime = millis();
  }
}


void setLeds() {
  for (int i = 0; i < totalLeds; i++) {

    if (i == currentStep) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void currentSequence() {
  lastButtonState1 = buttonState1;
  buttonState1 = digitalRead(buttonPin1);

  if (buttonState1 == HIGH && lastButtonState1 == LOW) {
    currentChannel = currentChannel + 1;
    if (currentChannel > 3) {
      currentChannel = 0;
    }
  }
}

void currentSequenceBackward() {
  lastButtonState2 = buttonState2;
  buttonState2 = digitalRead(buttonPin2);

  if (buttonState2 == HIGH && lastButtonState2 == LOW) {
    currentChannel = currentChannel - 1;
    if (currentChannel < 0) {
      currentChannel = 3;
    }
  }
}

void currentSequenceLeds() {
  for (int i = 0; i < totalSeqLeds; i++) {
    if (currentChannel == i) {
      digitalWrite(ledSeqPins[i], HIGH);
    } else {
      digitalWrite(ledSeqPins[i], LOW);
    }
  }
}

void pitch() {
  if (digitalRead(slideswitchPin) == HIGH) {
    minPitch = 262;
    maxPitch = 523;
  } else {
    minPitch = 523;
    maxPitch = 1047;
  }
}

void arpeggiator() {
  for (int i = 0; i < totalPots; i++) {
    potVals[i] = analogRead(potPins[i]);
    mappedPotVals[i] = map(potVals[i], 0, 1023, 0, 12);
    waveform1.frequency(minPitch * pow(2, mappedPotVals[i] / 12.0));
  }
}

