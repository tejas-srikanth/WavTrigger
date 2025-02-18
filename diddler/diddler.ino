#include <Metro.h>
#include <AltSoftSerial.h>
#include <wavTrigger.h>

const int interval = 6000;

Metro gSeqMetro(interval);        // Sequencer state machine interval timer
#define LED 13                // our LED

Metro gLedMetro(500);         // LED blink interval timer

byte gLedState = 0;  
int  gSeqState = 0;           // Main program sequencer state
int  gRateOffset = 0;         // WAV Trigger sample-rate offset
int state = -1;

const int track1 = 6;
const int track2 = 7;
const int track3 = 8;

const int button1 = 6;  // Button #1 connected to digital pin 6
const int button2 = 5;  // Button #2 connected to digital pin 5
const int button3 = 4;  // Button #3 connected to digital pin 4
const int button4 = 3;  // Button #4 connected to digital pin 3
const int button5 = 2;  // Button #5 connected to digital pin 2

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

  bool b1 = digitalRead(button1) == LOW;
  bool b2 = digitalRead(button2) == LOW;
  bool b3 = digitalRead(button3) == LOW;
  bool b4 = digitalRead(button4) == LOW;
  bool b5 = digitalRead(button5) == LOW;
  
  if (b1 || b2 || b3 || b4 || b5) { // dem buttons don't work tho
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  // Successively mute and unmute

  if (gSeqMetro.check() == 1) { // switches state every 6 s
   
    switch(state) {
      case -1:
        wTrig.masterGain(0);           
        
        wTrig.trackPlayPoly(track1);  // Loop Track 6
        wTrig.trackLoop(track1, 1);
        
        wTrig.trackPlayPoly(track2);  // Loop Track 7
        wTrig.trackLoop(track2, 2);
        
        wTrig.trackPlayPoly(track3);  // Loop Track 8 
        wTrig.trackLoop(track3, 1);
        
        state = 0; // update state
        break;

      case 0:
        wTrig.trackGain(track1, -40);  // Mute Track 6
        state = 1; // update state
        break;
      case 1:
        wTrig.trackGain(track2, -40);  // Mute Track 7
        state = 2; // update state
        break;
      case 2:
        wTrig.trackGain(track3, -40); // Mute Track 8
        state = 3; // update state
        break;
      case 3:
        wTrig.trackGain(track1, 0); // Unmute track 6
        wTrig.trackGain(track2, 0); // Unmute track 7
        wTrig.trackGain(track3, 0); // Unmute track 8
        state = 0; // update state
        break;
    }
  } 
}