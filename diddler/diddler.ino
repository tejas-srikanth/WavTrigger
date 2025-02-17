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

  // switch(state) {
  //   case -1:
  //     wTrig.masterGain(0);           
  //     wTrig.trackPlayPoly(6, true);  // Loop Track 6
  //     wTrig.trackPlayPoly(7, true);  // Loop Track 7
  //     wTrig.trackPlayPoly(8, true);  // Loop Track 8 
  //   case 0:
  //     wTrig.trackGain(6, -40);  // Mute Track 6
  //     break;
  //   case 1:
  //     wTrig.trackGain(7, -40);  // Mute Track 7
  //     break;
  //   case 2:
  //     wTrig.trackGain(8, -40); // Mute Track 8
  //     break;
  //   case 3:
  //     wTrig.trackGain(6, 0); // Unmute track 6
  //     wTrig.trackGain(7, 0); // Unmute track 7
  //     wTrig.trackGain(8, 0); // Unmute track 8
  //     state = -1;
  //     break;
  // } 
  //state++ ;
  

  if (gSeqMetro.check() == 1) {

    // turn LED on off
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

    wTrig.masterGain(-8);                 // Lower main volume
    wTrig.trackPlayPoly(6);               // Play first note
    delay(1000);
    wTrig.trackPlayPoly(7);               // Play second note
    delay(1000);
    wTrig.trackPlayPoly(8);               // Play third note
    delay(1000);
    wTrig.trackFade(6, -50, 5000, 1);     // Fade Track 6 to -50dB and stop
    wTrig.trackFade(7, -50, 5000, 1);     // Fade Track 7 to -50dB and stop
    wTrig.trackFade(8, -50, 5000, 1);     // Fade Track 8 to -50dB and stop
  }
}