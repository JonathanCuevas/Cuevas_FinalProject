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

boolean on = false;

boolean stepState[4][8] = {
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH },
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH },
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH },
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH },
};

void setup() {
  AudioMemory(12);
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(0.4);
  waveform1.frequency(262);

  pinMode(slideswitchPin, INPUT);
  pinMode(slideswitchPin2, INPUT);
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


  checkButton();

}

void checkButton() {
  lastButtonState1 = buttonState1;
  buttonState1 = digitalRead(buttonPin1);
  
  if(buttonState1 == HIGH && lastButtonState1 == LOW) {
    if(on == false) {
      on = true;
    } else if(on == true) {
      on = false;
    }
  }

    if(on == true) {
    liveSequence();
    digitalWrite(ledSeqPins[0], HIGH);
    digitalWrite(ledSeqPins[1], LOW);
  } else if(on == false) {
    on = false;
    setSequence1();
    digitalWrite(ledSeqPins[1], HIGH);
    digitalWrite(ledSeqPins[0], LOW);
  }
}



void liveSequence() {


//void sequence() 
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


//void sequenceBackward() 
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


//void setLeds() 
  for (int i = 0; i < totalLeds; i++) {

    if (i == currentStep) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }


//void pitch() 
  for (int i = 0; i < totalPots; i++) {

    if (i == currentStep) {
      potVals[i] = analogRead(potPins[i]);
      mappedPotVals[i] = map(potVals[i], 0, 1023, 0, 12);
      waveform1.frequency(minPitch * pow(2, mappedPotVals[i] / 12.0));
    }
  }


//void pitchBoundaries() 
  if (digitalRead(slideswitchPin2) == HIGH) {
    minPitch = 262;
    maxPitch = 523;
  } else {
    minPitch = 523;
    maxPitch = 1047;
  }
}

void setSequence1() {

//  void sequence() 
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


//void sequenceBackward() 
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


//void setLeds() 
  for (int i = 0; i < totalLeds; i++) {

    if (i == currentStep) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }


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



