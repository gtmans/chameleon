# chameleon
funbot with TCS3472 color sensor and neopixel ledstrip

I am trying to transfer this project from Arduino Nano to ATTiny85. At first I encountered som problems compiling the project because the "Adafruit_TCS34725.h" is using the arduino wire library, but finally succeeded compiling it successfull by using ATTinycore with options Board: ATTiny/25/45/85(no bootloader) and Programmer: Arduino as ISP. The next problem is that the TCS3472 somehow does not initialise. I use i2c connections that I used in other projects: SCL pin4 and SDA pin0. Tried using TinyWireM.h and wire.h which both compiles OK.

![chameleon](https://github.com/gtmans/chameleon/blob/main/chameleon-breadboard.jpg)

I tried this:

wrote a i2c scanner (see *4.5.ino) that displays the found address binairy on the led strip but it finds nothing
checked the ATTiny is running at 16MHz : Yes
Wire.setClock() to set the I2C clock speed to 100kHz and 10kHz, which should be compatible with the ATTiny85.
two pullups of 10k to sda and scl
3.3v and 5v
compiled succesfully with wire.h or TinyWireM.h
