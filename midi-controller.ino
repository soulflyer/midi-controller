#include "MIDIUSB.h"

const int potPin = 2;
int val[] = {0, 0, 0, 0, 0, 0};
int oldVal[] = {0, 0, 0, 0, 0, 0};
int controller[] = {10, 11, 12, 13, 14, 15};
/* The channel stored here starts at 0, which represents midi channel 1 */
int channel = 10;

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  Serial.begin(9600);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

/**
   Adjust the value from the control to be in the correct range for a midi controller
*/
int scaledValue(int minVal, int maxVal, int value) {
  float ratio = maxVal / 127;
  float returnVal = value / ratio;
  return returnVal;
}

void readPin(int pin){
  int adjusted = 0;
  oldVal[pin] =val[pin];
  adjusted = scaledValue(0, 1024, analogRead(pin));
  val[pin] = adjusted;
  if (oldVal[pin] != val[pin]) {
    controlChange(channel, controller[pin], adjusted);
    MidiUSB.flush();
  }
}

void loop() {
  readPin(2);
  /* Probably won't need the delay when reading lots of pins?*/
  delay(20);
}
