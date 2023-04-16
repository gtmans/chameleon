// using  ATTinycore
// Board: ATTiny/25/45/85(no bootloader)
// Programmer: Arduino as ISP

#include <Adafruit_NeoPixel.h>
#include "Adafruit_TCS34725.h"
#define  Tiny 1                   // compile for ATTiny85

#ifdef  Tiny
  #include <avr/power.h>          // Required for 16 MHz Adafruit Trinket
  #include <TinyWireM.h>          // I2C Master lib for ATTinys which use USI
  #define LED_PIN    3            // PB3 pin to which the NeoPixels are attached  
#else
  #include <Wire.h>
  #define LED_PIN    6            // pin to which the NeoPixels are attached
#endif
#define   LED_COUNT  8            // number of pixels attached to Attiny85

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
  
int minr,ming,minb,maxr,maxg,maxb;
int myred,mygre,myblu;
    
void setup() {

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(5);  // Set BRIGHTNESS to about 1/5 (max = 255)

  #ifndef  Tiny
    Serial.begin    (115200);
    while           (!Serial);
    Serial.println  ("Color View Test!");
  #else
    setcolor(0,255,0);        //green: show leds are working 
  #endif

  if (tcs.begin()) {
  #ifndef  Tiny    
    Serial.println  ("Found sensor");
  #else
    setcolor        (0,0,255);//blue: show blue leds found sensor
    delay           (1000);
  #endif
  } else {
  #ifndef  Tiny    
    Serial.println  ("No TCS34725 found ... check your connections");
  #else
    setcolor        (255,0,0);//red: show red leds no sensor found
  #endif
  while (1);                // halt!
  }

  minr=255;
  ming=minr;
  minb=minr;

}

void loop() {
  float red, green, blue;    
  delay(60);  // takes 50ms to read
  tcs.getRGB  (&red, &green, &blue);
  if (red>maxr)  {maxr=red;}
  if (red<minr)  {minr=red;}
  if (green>maxg){maxg=green;}
  if (green<ming){ming=green;}
  if (blue>maxb) {maxb=blue;}
  if (blue<minb) {minb=blue;}
  
  myred=map(int(red),  minr,maxr,0,255);
  mygre=map(int(green),ming,maxg,0,255);
  myblu=map(int(blue), minb,maxb,0,255);

#ifndef  Tiny
  Serial.print("R:");   Serial.print(String(int(red))  +"("+String(minr)+"-"+String(maxr)+")"); 
  Serial.print("\tG:"); Serial.print(String(int(green))+"("+String(ming)+"-"+String(maxg)+")"); 
  Serial.print("\tB:"); Serial.print(String(int(blue)) +"("+String(minb)+"-"+String(maxb)+")");
  Serial.print("\t");
  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\t");
  Serial.print(String(myred)+"/"+String(mygre)+"/"+String(myblu));
  Serial.print("\n");
#endif

  setcolor(myred,mygre,myblu);
  delay (100);
}

void setcolor(int r,int g,int b){
  strip.clear ();
  for         (int n=0;n<LED_COUNT;n++){
    strip.setPixelColor(n, strip.Color(r,g,b));
    strip.show();
    delay   (250);
  }
}
