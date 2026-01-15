#include "MIDIUSB.h"

const int potPin = 2;
int val = 0;
int oldVal = 0;
int adjusted = 0;

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

void loop() {
  oldVal = val;
  val = analogRead(potPin);
  if (oldVal != val) {
    adjusted = scaledValue(0, 1024, val);
    Serial.println("val:      " + String(val));
    Serial.println("adjusted: " + String(adjusted));
    controlChange(0, 10, adjusted);
    MidiUSB.flush();
  }
  delay(500);
}

/* void loop() { */
/*   Serial.println("Sending note on"); */
/*   noteOn(0, 48, 64);   // Channel 0, middle C, normal velocity */
/*   MidiUSB.flush(); */
/*   delay(500); */
/*   Serial.println("Sending note off"); */
/*   noteOff(0, 48, 64);  // Channel 0, middle C, normal velocity */
/*   MidiUSB.flush(); */
/*   delay(1500); */

/*   // controlChange(0, 10, 65); // Set the value of controller 10 on channel 0 to 65 */
/* } */
