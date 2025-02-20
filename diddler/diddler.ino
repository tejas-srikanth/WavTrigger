#include <AltSoftSerial.h>
#include <wavTrigger.h>

const int interval = 1000;
const int muteVol = -70;
const int unmuteVol = 0;

#define LED 13                // our LED

byte gLedState = 0;  
int  gSeqState = 0;           // Main program sequencer state
int  gRateOffset = 0;         // WAV Trigger sample-rate offset
int state = -1;
int currState = -1;


const int track1 = 35;
const int track2 = 36;
const int track3 = 37;
const int track4 = 38;
const int track5 = 39;

const int tracks[] = {track1, track2, track3, track4, track5};

const int button1 = 6;  // Button #1 connected to digital pin 6
const int button2 = 5;  // Button #2 connected to digital pin 5
const int button3 = 4;  // Button #3 connected to digital pin 4
const int button4 = 3;  // Button #4 connected to digital pin 3
const int button5 = 2;  // Button #5 connected to digital pin 2

const int buttons[] = {button1, button2, button3, button4, button5};

wavTrigger wTrig;

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);

  Serial.begin(9600);  // WAV Trigger baud rate
  wTrig.start();
  delay(2000);  // Wait for WAV Trigger to initialize

  wTrig.stopAllTracks();
  wTrig.samplerateOffset(0);
  wTrig.trackPlayPoly(6, true);  // Loop Track 6
  
  pinMode(LED,OUTPUT);

}

void loop() {

    // currState is a 3-bit integer
    // the ith bit from the right represents
    // whether the corresponding track is turned on
    // EX: currState=6=(110)b
    // track1 is off (last bit is 0)
    // track2 is on (2nd last bit is 1)
    // track3 is on (3rd last bit is 1)
  switch(currState){

    // entry state for state machine
    case -1: {
      Serial.println(currState);
      wTrig.masterGain(0);
      // play all tracks initially
      for (int i = 0; i < 5; ++i){
        wTrig.trackPlayPoly(tracks[i]);
        wTrig.trackLoop(tracks[i], 1);
        wTrig.trackGain(tracks[i], muteVol);
      }
      // begin the main loop
      currState = 0;
      break;
    }

    // main loop
    default: {
      for (int i = 0; i < 5; ++i){
        int currButtonStatus = digitalRead(buttons[i]); // can use rand() % 2 to simulate button press
        int currBitValue = currState & (1 << i); // gets the ith bit from the right

        // we check if user has just pressed the button
        if (currButtonStatus && !currBitValue){
          wTrig.trackGain(tracks[i], unmuteVol); // play track
          currState ^= (1 << i); // toggle ith bit from right
        } 
        
        // check if user has just let go of the button
        else if (!currButtonStatus && currBitValue){
          wTrig.trackGain(tracks[i], muteVol); // mute track
          currState ^= (1 << i); // toggle ith bit from right
        }
      } 
      Serial.println(currState, BIN);
      break;
    }
  } 
}