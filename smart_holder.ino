

/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.

 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch

 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).

 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND

 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include <Wire.h>
#include "Adafruit_DRV2605.h"


#include "HX711.h"
#include <Adafruit_NeoPixel.h>

#define DOUT  2  //D4
#define CLK   0 //D3
#define PIN 12 // D6

Adafruit_DRV2605 drv;
HX711 scale(DOUT, CLK);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(26, PIN, NEO_GRB + NEO_KHZ800);

float temp_weight = 0;
int counter = 0;
const int ledPin1 = 12;
const int ledPin2 = 13;
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
 
const int buzzerPin = 16 ; //D0

uint8_t LED_weight = 0;
uint8_t alarm_status = 0;
uint16_t count;


float calibration_factor = -3070; //-7050 worked for my 440lb max scale setup

void alarm_tone(){
   for (int t=0 ; t < 3 ; t++){
  tone(buzzerPin,2300);
  delay(100);
  tone(buzzerPin,2300);
  delay(100);    
  tone(buzzerPin,2300);
  delay(100);
  noTone(buzzerPin);
  delay(70);
  drv.setWaveform(0, 2);  // play effect 
  drv.setWaveform(1, 0);       // end waveform
  // play the effect!
  drv.go();
}
}
void firstSection()
{
 for(int i = 0 ; i < 3 ; i++){
  tone(buzzerPin,1300);
  delay(30); 
  tone(buzzerPin,1600);
  delay(30);
  tone(buzzerPin,1900);
  delay(30);
  noTone(buzzerPin);
  delay(70);
 }
 pinMode(buzzerPin,INPUT);
 //digitalWrite(buzzerPin,HIGH);
}
void setup() {
  pinMode(PIN,OUTPUT);
  Serial.begin(9600);
  
   strip.begin();
   strip.show(); // Initialize all pixels to 'off'
   drv.begin();
   drv.selectLibrary(1);
   drv.setMode(DRV2605_MODE_INTTRIG); 
  
   scale.set_scale(calibration_factor); //Adjust to this calibration factor
   delay(1000);
   scale.tare(); //Reset the scale to 0
  
   colorWipe(strip.Color(0, 50, 0), 30);
   firstSection();
   colorWipe(strip.Color(0, 0, 0), 30);
   
  drv.setWaveform(0, 58);  // play effect 
  drv.setWaveform(1, 0);       // end waveform
  // play the effect!
  drv.go();
  // wait a bit
  //delay(2000);
}

void loop() {
  
  temp_weight = scale.get_units(1);
  if (temp_weight < 0) temp_weight = 0;
  LED_weight = map(temp_weight,1,300,1,26);
  //Serial.println(LED_weight);
  Weight(strip.Color(0, 0, LED_weight+(10*(LED_weight/2))), 0);  
  if (LED_weight > 10) alarm_status = 1;
  if (LED_weight < 5 && alarm_status == 1) count++;
  if (count > 100) { alarm_tone();
                      count = 0;
  }
  
}


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void Weight(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<26; i++) {
    strip.setPixelColor(i, c);
  }
//  for(uint16_t j=LED_weight ; j < 26 ; j++){
//    strip.setPixelColor(j, strip.Color(0,0,0));
//  }
    strip.show();
    delay(wait);
  }

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
    digitalWrite(ledPin1, HIGH);
    delay(duration);
    digitalWrite(ledPin1, LOW);
  }else
  {
    digitalWrite(ledPin2, HIGH);
    delay(duration);
    digitalWrite(ledPin2, LOW);
  }
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(50);
 
  //Increment counter
  counter++;
}

