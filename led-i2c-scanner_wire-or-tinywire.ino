// --------------------------------------
// i2c_scanner ATTiny85 led debug blinks 5 times when i2c device found
// using  ATTinycore (which supports Wire.h)
// Board: ATTiny/25/45/85(no bootloader)
// Programmer: Arduino as ISP
// ATTiny85 running at 16MHz(PLL) and burned bootloader for 16MHz
// --------------------------------------

// check this https://github.com/107-systems/107-Arduino-TCS3472/blob/main/examples/TCS3472-Basic/TCS3472-Basic.ino

//#define  Use_TinyWire 1       // if defined uses TiniWireM.h io. Wire.h 

#ifdef   Use_TinyWire
  #include <TinyWireM.h>        // I2C Master lib for ATTinys which use USI
  #include <avr/power.h>        // Required for 16 MHz Adafruit Trinket
#else
  #include <Wire.h>
#endif
#include "Adafruit_TCS34725.h"
#define   TST_LED    4          // PB4 pin to which the led is attached  

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);//https://learn.adafruit.com/adafruit-color-sensors/library-reference

void setup() {
  pinMode                       (TST_LED, OUTPUT);
  for (int i=0;i<3;i++)         {digitalWrite(TST_LED, HIGH);delay(500);digitalWrite(TST_LED, LOW);delay(500);}
                                delay(1000); // blink 3 times and wait a second
  #ifdef   Use_TinyWire
    TinyWireM.begin();
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
      clock_prescale_set(clock_div_1);
    #endif
  #else
    Wire.begin();
    Wire.setClock(100000);      // set I2C clock speed to 100kHz
//  Wire.setClock(10000);       // set I2C clock speed to 10kHz
  #endif

  scani2c();                    // (3 initial+) 5 blinks=found device 10 blinks=error 15 blinks=no deviced found
 
  if (tcs.begin()) {            // turn led on when sensor initiates
    digitalWrite(TST_LED, HIGH);// led on  means found a device  on i2c
  } else {
    digitalWrite(TST_LED, LOW);// led off means no device found on i2c
  }
}

void loop() {}

void scani2c() {
  int nDevices=0;
  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device did acknowledge to the address.
    #ifdef   Use_TinyWire
      TinyWireM.beginTransmission(address);
      byte error = TinyWireM.endTransmission();
    #else
      Wire.beginTransmission(address);
      byte error = Wire.endTransmission();
    #endif
    
    if (error == 0) {
      // I2C device found at address 0x
      for (int i=0;i<5;i++)   {digitalWrite(TST_LED, HIGH);delay(500);digitalWrite(TST_LED, LOW);delay(500);}
                              delay(1000); // blink 5 times and wait a second      
      nDevices++;
    } else if (error == 4) {
      for (int i=0;i<10;i++)  {digitalWrite(TST_LED, HIGH);delay(500);digitalWrite(TST_LED, LOW);delay(500);}
                              delay(1000); // blink 10 times and wait a second
    }
  }
  if (nDevices == 0) {
      for (int i=0;i<15;i++)  {digitalWrite(TST_LED, HIGH);delay(500);digitalWrite(TST_LED, LOW);delay(500);}
                              delay(1000); // blink 15 times and wait a second
  }
}
