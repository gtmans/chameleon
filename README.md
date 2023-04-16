# chameleon
funbot with TCS3472 color sensor and neopixel ledstrip

I am trying to transfer this project from Arduino Nano to ATTiny85. At first I encountered som problems compiling the project because the "Adafruit_TCS34725.h" is using the arduino wire library
but finally succeeded by using  ATTinycore Board: ATTiny/25/45/85(no bootloader) and Programmer: Arduino as ISP.

![chameleon](https://github.com/gtmans/chameleon/blob/main/chameleon-breadboard.jpg)
