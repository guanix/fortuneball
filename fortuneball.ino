#include "FastLED.h"
#include <SoftwareSerial.h>

#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 50
CRGB leds[NUM_LEDS_PER_STRIP];
const int ledStripPin = 17;

int crystalBallState = 0;
int crystalBallMax = 8;

//holders for infromation you're going to pass to shifting function
byte data;
byte dataArray[10];


/*============ MOTION SENSOR VARIABLES ====================*/
// motion sensor variables
int pirPin = 3;    //the digital pin connected to the PIR sensor's output

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  


/*=============== Crystal Ball sensor ====================*/
const int crystalBallPin = 1; //analog pin 0
int curCrystalBallVal = 0;
int crystalBallThreshold = 1;
int crystalBallSteady = 0;

// etc
int tempCounter = 0;

void setup() {
  // set crystal ball led controller pin
  pinMode(ledStripPin, OUTPUT);
  FastLED.addLeds<NEOPIXEL, 17>(leds, NUM_LEDS_PER_STRIP); //led strip pin needs to be hardcoded or build errors
  delay(1000);

  blinkLedStrip(8,8);

  delay(5000);

  blinkLedStrip(0,8);
  delay(5000);

  //calibrate IR sensor in crystal ball
  stabalizeCrystalBallSensor();
  
}

void stabalizeCrystalBallSensor() {
//  Serial.println("Calibrating Crystal ball sensor");
  int tempCrystalBallReading = 0;
  for(int i=0;i<10;i++) {
    tempCrystalBallReading += analogRead(crystalBallPin);  
    delay(100);
  }
  crystalBallSteady = tempCrystalBallReading/10;
}

void blinkLedStrip(int tgtLevel, int tgtMax) {
  //lights up a a percentage of the led strips calculated by tgtLevel/ tgtMax
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {

    if(i%tgtMax< tgtLevel) { 
      //leds[0][i] = CRGB(random(0, 255),random(0, 255),random(0, 255));
      leds[i] = CRGB::White;
    } else {
      leds[i] = CRGB::Black;
    }
 
  }
  FastLED.show();
//  Serial.println();
}

void read_crystal_ball_sensor() {
  int cb_read_avg = 0;
  for(int i=0;i<10;i++) {
    cb_read_avg += analogRead(crystalBallPin);
    delay(10);
    
  }
  curCrystalBallVal =  cb_read_avg/10;
  
  if(abs(curCrystalBallVal-crystalBallSteady) > crystalBallThreshold) {
    
    crystalBallState += 1;
    crystalBallState = min(crystalBallState,crystalBallMax);
  } else {

    crystalBallState -= 1;
    crystalBallState = max(crystalBallState,0);
  }

  blinkLedStrip(crystalBallState,crystalBallMax);
  if (Serial) {
    Serial.write(crystalBallState);
  }
}


void loop() {
  delay(100);               // wait for a second

  read_crystal_ball_sensor();
}
