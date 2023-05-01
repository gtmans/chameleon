// using  ATTinycore (which supports Wire.h)
// Board: ATTiny/25/45/85(no bootloader)
// Programmer: Arduino as ISP
// ATTiny85 running at 16MHz(PLL) and burned bootloader 
  
//#define  Tiny         1           // if defined compile for ATTiny85 io. Arduino
#define  Xiao           1           // if defined compile for Seeduino Xiao
//#define  Use_TinyWire 1           // if defined uses TiniWireM.h io. Wire.h 
//#define  Use_Serial

bool debug = false;
int  diff  = 48;

#ifdef   Use_TinyWire
  #include <TinyWireM.h>          // I2C Master lib for ATTinys which use USI
//#include <USI_TWI_Master.h>
#else
  #include <Wire.h>
#endif

#ifdef   Tiny
  #include <avr/power.h>          // Required for 16 MHz Adafruit Trinket
  #define LED_PIN    3            // PB3 pin to which the NeoPixels are attached  
  #define TST_LED    4            // PB4 pin to which the led is attached  
#else      
  #define TST_LED    1            // D1 pin to which the led is attached  
  #define LED_PIN    6            // pin to which the NeoPixels are attached
#endif
//#define   LED_COUNT  8            // number of pixels attached to NEO pixelstrip
#define   LED_COUNT  28+13

#include <Adafruit_NeoPixel.h>
#include "Adafruit_TCS34725.h"
//#include "Adafruit_TCS34725_Hshopvn.h"


Adafruit_NeoPixel strip (LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);//https://learn.adafruit.com/adafruit-color-sensors/library-reference
//Adafruit_TCS34725_Hshopvn tcs = Adafruit_TCS34725_Hshopvn(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);//https://learn.adafruit.com/adafruit-color-sensors/library-reference
  
int minr,ming,minb,maxr,maxg,maxb;
int myred,mygre,myblu;
    
void setup() {

  #ifndef  Tiny 
    #ifdef Use_Serial
      Serial.begin              (115200);
      while                     (!Serial);
      Serial.println            ("Color View Test!");
    #endif
  #else
    pinMode                   (TST_LED, OUTPUT);
    digitalWrite              (TST_LED, HIGH);
    delay                     (1000);
    digitalWrite              (TST_LED, LOW);
    delay                     (1000);
    digitalWrite              (TST_LED, HIGH);
    delay                     (1000);
    digitalWrite              (TST_LED, LOW);    
  #endif

  strip.begin();              // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();               // Turn OFF all pixels ASAP
  strip.setBrightness(5);     // Set BRIGHTNESS to about 1/5 (max = 255)

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
//  USI_TWI_Master::init();
//  TinyWireM::USI_TWI_Master twi;
    #ifndef   Use_TinyWire
//    Wire.setClock(100000);  // set I2C clock speed to 100kHz
//    Wire.setClock(10000);   // set I2C clock speed to 100kHz
    #endif                    // TinyWireM.setClock() doesnot exist
    setcolor(0,255,0);        // green: processor is running at 16mhz
  #else
    setcolor(255,0,0);        // red:   processor is not running at 16mhz or not ATTiny
  #endif

  if (debug){
    delay(1000);
    displayAddresses();         // scan i2c bus for devices and diplay found address binairy on 8 pixel ledstrip 
  }

  if (tcs.begin()) {
  #ifndef  Tiny
    #ifdef Use_Serial    
      Serial.println           ("Found sensor");
    #else
//    setcolor                 (0,255,0);//green: show green leds found sensor
    #endif
  #else
//    setcolor                   (0,0,255);//blue: show blue leds found sensor
  #endif
  } else {
  #ifndef  Tiny    
    #ifdef Use_Serial
        Serial.println         ("No TCS34725 found ... check your connections");
    #else
        setcolor               (255,0,0);//red: no sensor found
    #endif
  #else
        setcolor               (255,0,0);//red: no sensor found
  #endif
  while (1);                  // halt!
  }

  rainbow(10);             // Flowing rainbow cycle along the whole strip

//  setcolor(0,255,0);          //green: show everything is OK
//  delay                       (1000);

  minr=255;
  ming=minr;
  minb=minr;
}

void loop() {
  float red, green, blue;    
  delay(60);  // takes 50ms to read
  tcs.getRGB     (&red, &green, &blue);
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
  #ifdef Use_Serial  
  Serial.print("R:");   Serial.print(String(int(red))  +"("+String(minr)+"-"+String(maxr)+")"); 
  Serial.print("\tG:"); Serial.print(String(int(green))+"("+String(ming)+"-"+String(maxg)+")"); 
  Serial.print("\tB:"); Serial.print(String(int(blue)) +"("+String(minb)+"-"+String(maxb)+")");
  Serial.print("\t");
  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\t");
  Serial.print(String(myred)+"/"+String(mygre)+"/"+String(myblu));
  Serial.print("\n");
  #endif
#endif

//setcolor  (myred,mygre,myblu);
  setcolor2 (myred,mygre,myblu);
  delay     (100);
}

void setcolor(int r,int g,int b){
  strip.clear ();
  for         (int n=0;n<LED_COUNT;n++){
    strip.setPixelColor(n, strip.Color(r,g,b));
//  strip.show();
//  delay   (50);
  }
  strip.show();
}

void setcolor2(int r,int g,int b){//more variation of colors
  strip.clear ();
  int r2,g2,b2;
  for         (int n=0;n<LED_COUNT;n++){
    if (r>diff){r2=r-diff+random(diff);}else{r2=random(diff);}
    if (g>diff){g2=g-diff+random(diff);}else{g2=random(diff);}
    if (b>diff){b2=b-diff+random(diff);}else{b2=random(diff);}
    strip.setPixelColor(n,r2,g2,b2);
    Serial.println     ("diff:"+String(diff)+" pixel:"+String(n)+" r2:"+String(r2)+" g2:"+String(g2)+" b2:"+String(b2));
  }
  strip.show();
}

void displayAddresses() {
  int nDevices = 0;

  #ifndef  Tiny
  #ifdef Use_Serial  
    Serial.println("Scanning...");
  #endif
  #endif
  
  #ifdef   Use_TinyWire
  //const uint32_t TWI_SPEED = 100000UL; // desired clock speed
  //TinyWireM.begin(TWI_SPEED); // set I2C clock frequency to 100 kHz does not work
    TinyWireM.begin(); 
  #else
    Wire.begin();
  #endif
  
  for (byte address = 1; address < 127; ++address) {
    byte saveA=address;
//  byte binaryAddress[8]={"00000000"};
    byte binaryAddress[8]={0};
    byte index = 0;  
    // Converteer het I2C-adres naar binaire notatie
    while (saveA > 0) {
      binaryAddress[index++] = saveA % 2;
      saveA /= 2;
    }
    strip.clear ();
    // Schrijf het binaire adres naar de ledstrip
    for (int i = 0; i < 8; i++) {
      if (binaryAddress[i] == 1) {
        strip.setPixelColor(i, 255);
      } else {
        strip.setPixelColor(i, 0);
      }
    }
    strip.show();
    #ifndef  Tiny
    #ifdef Use_Serial
      Serial.println(address);
    #endif
    #endif
        
    #ifdef  Use_TinyWire
      TinyWireM.beginTransmission(address);
      byte error = TinyWireM.endTransmission();
    #else
      Wire.beginTransmission(address);
      byte error = Wire.endTransmission();
    #endif
    
    if (error == 0) {
      #ifndef  Tiny
      #ifdef Use_Serial
        Serial.print("I2C device found at address 0x");
        if (address < 16) {
          Serial.print("0");
        }
        Serial.print  (address, HEX);
        Serial.print  ("HEX ");
        Serial.print  (address, BIN);
        Serial.println("BIN");
      #endif
      #endif
      ++nDevices;
      delay(5000); 
     
    } else if (error == 4) {
      #ifndef  Tiny
      #ifdef Use_Serial
        Serial.print("Unknown error at address 0x");
        if (address < 16) {
          Serial.print("0");
        }
        Serial.println(address, HEX);
      #endif        
      #else
        setcolor(255,255,0);//REDGREEN
        delay   (5000);
      #endif
      }
      delay(50);
  }
    if (nDevices == 0) {
    #ifndef  Tiny
    #ifdef Use_Serial
      Serial.println("No I2C devices found\n");
    #endif
    #else
      setcolor(0,255,255);//BLUEGREEN
      delay   (5000);
    #endif
    }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 1*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
