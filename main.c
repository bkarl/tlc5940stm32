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

    /*****************************************************
    Init of the PWM Timer
    ******************************************************/
    //activate periphal
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
    TIM_OCInitTypeDef TIM_OC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    //YOUR PWM Pin here - PA1 in this case
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //replace GPIOA with the GPIO of your OC Pin
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBase_InitStructure.TIM_Period = 3;
    //your clock speed in mhz - 1
    TIM_TimeBase_InitStructure.TIM_Prescaler = 71;
    //replace TIM2 with your desired timer
    TIM_TimeBaseInit(TIM2, &TIM_TimeBase_InitStructure);

    TIM_OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OC_InitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OC_InitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
    TIM_OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC_InitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC_InitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OC_InitStructure.TIM_Pulse = 2;	//50% duty cycle
    //replace TIM2 with your desired timer and TIM_OCxInit to match your PWM port. In this case TIM2_CH2 is used (PA1)
    TIM_OC2Init(TIM2, &TIM_OC_InitStructure);

    //replace TIM2 with your desired timer...
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);
    //replace TIM2 with your desired timer...
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    //insert hand timer to init struct
    tlcInit.timer = TIM2;

    initTLC(&tlcInit);

    //test
    setTLCChannel(1, 0x00FF);
    setTLCChannel(0, 0xFFFF);
    setTLCChannel(3, 0x000F);

    updateTLC();

   // TIM_Cmd(tlcConf.timer, ENABLE);

    setTLCChannel(4, 0x0FFF);
    updateTLC();

    while(1)
    {

    }
}
