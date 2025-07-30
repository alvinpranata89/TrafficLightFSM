#include <stdint.h>
#include "stm32f4xx.h"

#define LIGHT_PORT			GPIOA
#define SENSOR_PORT			GPIOC

#define NORTH_SENSOR		(1<<1)
#define EAST_SENSOR			(1<<0)

#define EAST_LED_RED			(1U<<9)
#define EAST_LED_YELLOW			(1U<<8)
#define EAST_LED_GREEN			(1U<<7)

#define NORTH_LED_RED			(1U<<6)
#define NORTH_LED_YELLOW		(1U<<5)
#define NORTH_LED_GREEN			(1U<<4)

#define GPIOAEN					0x01
#define GPIOCEN					0x04

#define SYSCLK					16000000

struct State{
	uint32_t output;
	uint32_t time;
	uint8_t next_state[4];
};

typedef const struct State stateType;

#define go_north	0
#define wait_north	1
#define go_east		2
#define wait_east	3

stateType StateMachine [4]={
		{0x210,3000,{go_north,wait_north,go_north,wait_north}},
		{0x220,500,{go_east,go_east,go_east,go_east}},
		{0x0C0,3000,{go_east,go_east,wait_east,wait_east}},
		{0x140,500,{go_north,go_north,go_north,go_north}},
};

void TrafficSystemInit(void);
void delay_ms(int delay);

uint8_t current_state;
uint8_t input;

int main(void)
{
	TrafficSystemInit();

	current_state = go_north;

	while(1)
	{
		LIGHT_PORT->ODR = StateMachine[current_state].output; //turn on red/yellow/green light accordingly
		delay_ms(StateMachine[current_state].time); //call delay
		input = SENSOR_PORT->IDR & (NORTH_SENSOR | EAST_SENSOR); // check for sensor value
		current_state = StateMachine[current_state].next_state[input]; //cycle through the next FSM
	}
}

void TrafficSystemInit(){
	RCC->AHB1ENR |= GPIOAEN | GPIOCEN; //enable clock access to GPIOA and GPIOC
	LIGHT_PORT->MODER |= (1<<18) | (1<<16) | (1<<14) | (1<<12) | (1<<10) | (1<<8); //setting GPIO PINC for LED to output mode
}

void delay_ms(int delay)
{
	SysTick->LOAD = SYSCLK/1000;
	SysTick->VAL = 0;
	SysTick->CTRL=0x05;

	for(int i = 0; i<delay; i++)
	{
		while((SysTick->CTRL & 0x10000)==0){};
	}
	SysTick->CTRL=0;
}
