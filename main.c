
#include "stm32f10x_conf.h"
#include "tlc.h"
#include "main.h"

//init a 1 ms timer
void initTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


    TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //init 1 ms timer
    TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBase_InitStructure.TIM_Period = 1;
    TIM_TimeBase_InitStructure.TIM_Prescaler = 35999;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBase_InitStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);


    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);

}

//interrupt handler for the timer
void TIM3_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    if (currentDelay)
    {
        currentDelay--;
        return;
    }

}

int main(void)
{
    currentDelay = 0;

    //activate periphals
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);

    initTimer();

    //Using SPI1
    TLC_InitStruct tlcInit;
    tlcInit.spi = SPI1;
    tlcInit.gpioSPI = GPIOA;
    tlcInit.pinMISO = GPIO_Pin_6;
    tlcInit.pinMOSI = GPIO_Pin_7;
    tlcInit.pinCLK = GPIO_Pin_5;

    //Pins to Control the TLC
    tlcInit.gpioCntrl = GPIOA;
    tlcInit.pinXLAT = GPIO_Pin_4;
    tlcInit.pinBlank = GPIO_Pin_3;
    tlcInit.pinGSCLK = GPIO_Pin_1;

    initTLC(&tlcInit);

    //test
    setTLCChannel(1, 0x00FF);
    updateTLC();

    while(1)
    {
        //todo: do the GSCLK pulses with PWM
        cycleTLC();
    }
}
