#ifndef TLC_H
#define TLC_H

#include "stm32f10x_conf.h"
#include "main.h"

typedef struct
{
    SPI_TypeDef* spi;			//SPI Port to use
    GPIO_TypeDef* gpioSPI;		//GPIO of the SPI Port to use
    uint16_t pinMOSI;			//GPIO_Pin of MOSI
    uint16_t pinCLK;			//GPIO_Pin of CLK
    uint16_t pinMISO;			//GPIO_Pin of MISO
    GPIO_TypeDef* gpioCntrl;	//GPIO Port the Blank, XLAT und GSCLK Pins are connected to
    uint16_t pinBlank;			//GPIO_Pin of BLANK
    uint16_t pinXLAT;			//GPIO_Pin of XLAT
    TIM_TypeDef* timer;

} TLC_InitStruct;

//init the GPIOS/Timers/SPIs
void initTLC(TLC_InitStruct* tlc);
//write a byte to the tlc5940
void writeTLC(uint8_t data);
//write new color data to the tlc5940
void updateTLC();
//generates gsclk
//void cycleTLC();

void setTLCChannel(uint8_t channel, uint16_t val);
void resetTLC();

//array holding the data to be shifted in the driver
uint16_t leds[15];

volatile uint16_t clkCnt;
//timer for the PWM output
void TIM2_IRQHandler(void);

TLC_InitStruct tlcConf;

#endif
