#include "tlc.h"

void cycleTLC()
{
    uint16_t i;

    //generate GSCLK
    for (i = 0; i < 4096; i++)
    {
        GPIO_WriteBit(tlcConf.gpioCntrl, tlcConf.pinGSCLK, SET);
        GPIO_WriteBit(tlcConf.gpioCntrl, tlcConf.pinGSCLK, RESET);
    }
    GPIO_WriteBit(tlcConf.gpioCntrl, tlcConf.pinBlank, SET);
    GPIO_WriteBit(tlcConf.gpioCntrl, tlcConf.pinBlank, RESET);
}

void setTLCChannel(uint8_t channel, uint16_t val)
{
    leds[channel] = (val & 0x0FFF);
}


//delay in milliseconds
void delay(uint16_t d)
{
    currentDelay = d;
    while (currentDelay);
}

//init all GPIOs, timers, spi...
void initTLC(TLC_InitStruct* tlc)
{

    tlcConf = *tlc;

    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //init GPIOs
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = tlcConf.pinBlank | tlcConf.pinXLAT | tlcConf.pinGSCLK;
    GPIO_Init(tlcConf.gpioCntrl, &GPIO_InitStructure);

    //initial setup
    GPIO_WriteBit(tlcConf.gpioCntrl, tlcConf.pinBlank | tlcConf.pinXLAT | tlcConf.pinGSCLK, RESET);

    /*SPI*/
    GPIO_InitStructure.GPIO_Pin = tlcConf.pinCLK | tlcConf.pinMOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(tlcConf.gpioSPI, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = tlcConf.pinMISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(tlcConf.gpioSPI, &GPIO_InitStructure);

    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CRCPolynomial = 0;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_Init(tlcConf.spi, &SPI_InitStructure);

    SPI_Cmd(tlcConf.spi, ENABLE);

    //disable all outputs
    resetTLC();

}

void resetTLC()
{
    uint8_t i;
    for (i = 0; i < 16; i++)
    {
        leds[i] = 0x0000;
    }
    updateTLC();

}
//write a byte to the chip
void writeTLC(uint8_t data)
{
    SPI_I2S_SendData(tlcConf.spi, data);
    while (SPI_I2S_GetFlagStatus(tlcConf.spi, SPI_I2S_FLAG_TXE) == RESET);
}

//write new data to chip - i hope everything is right here
void updateTLC()
{
    GPIO_WriteBit(tlcConf.gpioCntrl, tlcConf.pinBlank, SET);
    delay(1);
    int8_t i;
    for (i = 15; i >= 0; i-=2)
    {
        uint8_t send1 = 0;
        uint8_t send = leds[i] >> 4;
        writeTLC(send);

        send = (leds[i] & 0x000F);
        send <<= 4;
        send1 = (leds[i-1]) >> 8;

        send |= send1;
        writeTLC(send);

        send = leds[i-1];
        writeTLC(send);
    }
    //delay(1);
    GPIO_WriteBit(tlcConf.gpioCntrl, tlcConf.pinXLAT, SET);
    delay(1);
    GPIO_WriteBit(tlcConf.gpioCntrl, tlcConf.pinBlank, RESET);
    GPIO_WriteBit(tlcConf.gpioCntrl, tlcConf.pinXLAT, RESET);
    delay(1);
}
