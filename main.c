#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <avr/pgmspace.h> 

//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h"
#include "usart_ATmega1284.h" 
enum DirStae {INIT,Forward,Backward,Right,Left,brake} dir_state;

void BlueRe()
{
	char temp=0;
	if(USART_HasReceived(0))
	{
		temp=USART_Receive(0);
	}
	switch(temp)
	{
		case 'B':
		dir_state=Forward;
		break;
		case 'F':
		dir_state=Backward;
		break;
		case 'R':
		dir_state=Right;
		break;
		case 'L':
		dir_state=Left;
		break;
		case 'S':
		dir_state=brake;
		break;
		case 'V':
		TCCR1A = ((1 << COM1A1) | (1 << WGM11));
		TCCR1B = ((1 << WGM12)  | (1 << WGM13) | (1 << CS11));
		ICR1=20000;		
		OCR1A=1000;
		break;
		case 'W':
		TCCR1A = ((1 << COM1A1) | (1 << WGM11));
		TCCR1B = ((1 << WGM12)  | (1 << WGM13) | (1 << CS11));
		ICR1=20000;
		OCR1A=2000;
		break;
		case 'U':
		TCCR1A = ((1 << COM1A1) | (1 << WGM11));
		TCCR1B = ((1 << WGM12)  | (1 << WGM13) | (1 << CS11));
		ICR1=20000;
		OCR1A=1500;
		break;		
	}
}
void Car_Init(){
	dir_state = INIT;	
}

void Car_Tick(){
	//Actions
	switch(dir_state){
		case INIT:
		PORTA=0x00;
		break;
		case Forward:
		PORTA = 0x0A;
		break;
		case Backward:
		PORTA = 0x05;
		break;
		case Right:
		PORTA = 0x09;
		break;
		case Left:
		PORTA = 0x06;
		break;
		case brake:
		PORTA = 0x00;
		break;
		default:
		PORTA = 0x00;
		break;
	}
	BlueRe();
}

void CarTask()
{
	Car_Init();
   for(;;) 
   { 	
	Car_Tick();   
	vTaskDelay(10); 
   } 
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(CarTask, (signed portCHAR *)"CarTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0xFF; 
   DDRD=0xF0;
   initUSART(0); 
   StartSecPulse(1); 
   vTaskStartScheduler(); 

	
	return 0; 
}