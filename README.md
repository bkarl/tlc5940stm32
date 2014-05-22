tlc5940stm32
============

A simple library to use TIs TLC5940 with an STM32 in grayscale mode.

You will need the the ST Standard Peripheral Library to use this. 
The Code was developed and tested with Em::Blocks (http://www.emblocks.org/web/).

The code also needs a delay(x in µS) function. I added one in the main.c file.

To start just fill a TLC_InitStruct and hand it to the library in the void initTLC(TLC_InitStruct* tlc) function.
You also need to start a PWM timer. There is an example for TIM2 with the PWM pin PA1.

In the file tlc.c change the timer function to your desired timer (in this case TIM2 is used).

Don't forget to active the GPIO and SPI peripheral clocks.

void updateTLC() writes Data to the TLC. Call this if you wish to propagate the changes you made to the chip.

void setTLCChannel(uint8_t channel, uint16_t val) sets a specific channel of the chip to the value given by val.
val is the brigthness value of the channel in 4095 steps. Call updateTLC() to commit your changes.

void resetTLC() resets all channels to zero. 

TODO: 
Add support for chained TLCs.
