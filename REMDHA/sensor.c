/*
 * sensor.c
 *
 * Created: 12.04.2019 16:11:33
 *  Author: nbarben
 */ 
#include <avr/io.h>							//include libraries
#include <avr/interrupt.h>
#include <stdbool.h>
#include "sensor.h"
#include "i2c_master.h"

int gesture = 0;							//define variables
int gestureold = 0;
int volumestate = 0;
volatile int gesturedelay = 0;

unsigned char MGC3130receive[256] = {};		//array for received data from MGC3130

int PAJ7620receive = 0x00;					//variable for received data from PAJ7620
int PAJ7620old = 0x00;						//variable to compare if there is a new Gesture
unsigned char initRegisterArray[][2] = {												//Array with data for initialization of PAJ7620
	{0xEF,0x00},
	{0x32,0x29},
	{0x33,0x01},
	{0x34,0x00},
	{0x35,0x01},
	{0x36,0x00},
	{0x37,0x07},
	{0x38,0x17},
	{0x39,0x06},
	{0x3A,0x12},
	{0x3F,0x00},
	{0x40,0x02},
	{0x41,0xFF},
	{0x42,0x01},
	{0x46,0x2D},
	{0x47,0x0F},
	{0x48,0x3C},
	{0x49,0x00},
	{0x4A,0x1E},
	{0x4B,0x00},
	{0x4C,0x20},
	{0x4D,0x00},
	{0x4E,0x1A},
	{0x4F,0x14},
	{0x50,0x00},
	{0x51,0x10},
	{0x52,0x00},
	{0x5C,0x02},
	{0x5D,0x00},
	{0x5E,0x10},
	{0x5F,0x3F},
	{0x60,0x27},
	{0x61,0x28},
	{0x62,0x00},
	{0x63,0x03},
	{0x64,0xF7},
	{0x65,0x03},
	{0x66,0xD9},
	{0x67,0x03},
	{0x68,0x01},
	{0x69,0xC8},
	{0x6A,0x40},
	{0x6D,0x04},
	{0x6E,0x00},
	{0x6F,0x00},
	{0x70,0x80},
	{0x71,0x00},
	{0x72,0x00},
	{0x73,0x00},
	{0x74,0xF0},
	{0x75,0x00},
	{0x80,0x42},
	{0x81,0x44},
	{0x82,0x04},
	{0x83,0x20},
	{0x84,0x20},
	{0x85,0x00},
	{0x86,0x10},
	{0x87,0x00},
	{0x88,0x05},
	{0x89,0x18},
	{0x8A,0x10},
	{0x8B,0x01},
	{0x8C,0x37},
	{0x8D,0x00},
	{0x8E,0xF0},
	{0x8F,0x81},
	{0x90,0x06},
	{0x91,0x06},
	{0x92,0x1E},
	{0x93,0x0D},
	{0x94,0x0A},
	{0x95,0x0A},
	{0x96,0x0C},
	{0x97,0x05},
	{0x98,0x0A},
	{0x99,0x41},
	{0x9A,0x14},
	{0x9B,0x0A},
	{0x9C,0x3F},
	{0x9D,0x33},
	{0x9E,0xAE},
	{0x9F,0xF9},
	{0xA0,0x48},
	{0xA1,0x13},
	{0xA2,0x10},
	{0xA3,0x08},
	{0xA4,0x30},
	{0xA5,0x19},
	{0xA6,0x10},
	{0xA7,0x08},
	{0xA8,0x24},
	{0xA9,0x04},
	{0xAA,0x1E},
	{0xAB,0x1E},
	{0xCC,0x19},
	{0xCD,0x0B},
	{0xCE,0x13},
	{0xCF,0x64},
	{0xD0,0x21},
	{0xD1,0x0F},
	{0xD2,0x88},
	{0xE0,0x01},
	{0xE1,0x04},
	{0xE2,0x41},
	{0xE3,0xD6},
	{0xE4,0x00},
	{0xE5,0x0C},
	{0xE6,0x0A},
	{0xE7,0x00},
	{0xE8,0x00},
	{0xE9,0x00},
	{0xEE,0x07},
	{0xEF,0x01},
	{0x00,0x1E},
	{0x01,0x1E},
	{0x02,0x0F},
	{0x03,0x10},
	{0x04,0x02},
	{0x05,0x00},
	{0x06,0xB0},
	{0x07,0x04},
	{0x08,0x0D},
	{0x09,0x0E},
	{0x0A,0x9C},
	{0x0B,0x04},
	{0x0C,0x05},
	{0x0D,0x0F},
	{0x0E,0x02},
	{0x0F,0x12},
	{0x10,0x02},
	{0x11,0x02},
	{0x12,0x00},
	{0x13,0x01},
	{0x14,0x05},
	{0x15,0x07},
	{0x16,0x05},
	{0x17,0x07},
	{0x18,0x01},
	{0x19,0x04},
	{0x1A,0x05},
	{0x1B,0x0C},
	{0x1C,0x2A},
	{0x1D,0x01},
	{0x1E,0x00},
	{0x21,0x00},
	{0x22,0x00},
	{0x23,0x00},
	{0x25,0x01},
	{0x26,0x00},
	{0x27,0x39},
	{0x28,0x7F},
	{0x29,0x08},
	{0x30,0x03},
	{0x31,0x00},
	{0x32,0x1A},
	{0x33,0x1A},
	{0x34,0x07},
	{0x35,0x07},
	{0x36,0x01},
	{0x37,0xFF},
	{0x38,0x36},
	{0x39,0x07},
	{0x3A,0x00},
	{0x3E,0xFF},
	{0x3F,0x00},
	{0x40,0x77},
	{0x41,0x40},
	{0x42,0x00},
	{0x43,0x30},
	{0x44,0xA0},
	{0x45,0x5C},
	{0x46,0x00},
	{0x47,0x00},
	{0x48,0x58},
	{0x4A,0x1E},
	{0x4B,0x1E},
	{0x4C,0x00},
	{0x4D,0x00},
	{0x4E,0xA0},
	{0x4F,0x80},
	{0x50,0x00},
	{0x51,0x00},
	{0x52,0x00},
	{0x53,0x00},
	{0x54,0x00},
	{0x57,0x80},
	{0x59,0x10},
	{0x5A,0x08},
	{0x5B,0x94},
	{0x5C,0xE8},
	{0x5D,0x08},
	{0x5E,0x3D},
	{0x5F,0x99},
	{0x60,0x45},
	{0x61,0x40},
	{0x63,0x2D},
	{0x64,0x02},
	{0x65,0x96},
	{0x66,0x00},
	{0x67,0x97},
	{0x68,0x01},
	{0x69,0xCD},
	{0x6A,0x01},
	{0x6B,0xB0},
	{0x6C,0x04},
	{0x6D,0x2C},
	{0x6E,0x01},
	{0x6F,0x32},
	{0x71,0x00},
	{0x72,0x01},
	{0x73,0x35},
	{0x74,0x00},
	{0x75,0x33},
	{0x76,0x31},
	{0x77,0x01},
	{0x7C,0x84},
	{0x7D,0x03},
	{0x7E,0x01},
};

void COUNTER0Init(void)						//initialize COUNTER0
{
	TCNT0 = 0x00;							//set value of counter 0
	OCR0B = 0x00;							//set value for Output Compare Register B
	OCR0A = 0x9C;							//set value for Output Compare Register A
	TCCR0B = 0x04;							//clock select
	TCCR0A = 0x02;							//CTC mode select
	TIMSK0 = 0x02;							//output compare match interrupt enable
	TIFR0 = 0x00;		
	GTCCR = 0x00;

	sei();									//global Interrupt enable
}

bool MGC3130Init(void)						//initialize MGC3130
{
	DDRB = 0x00;							//enable pull up on TS line
	PORTB = 0x04;
	int32_t i = 0;
	while (((PINB >> 2) & 1) == 0)			//wait for TS line to get high
	{
		i++;
		if(i>=1000) 
		{
			return false;
		}
	}
	return true;
}

bool MGC3130ReadGesture(void)				//read gesture from MGC3130
{
	if(((PINB >> 2) & 1) == 0)				//check TS line
	{
		DDRB = 0x04;						//disable pull up on TS line
		PORTB = 0x00;
		i2c_start(MGC3130_ID_READ);			//send READ address
		MGC3130receive[0] = i2c_read_ack();	//read first byte
		
		for(uint16_t i = 1; i < (MGC3130receive[0]-1); i++)	//read as many bytes as first byte had as value
		{
			MGC3130receive[i] = i2c_read_ack();
		}
		MGC3130receive[(MGC3130receive[0]-1)] = i2c_read_nack();
		
		i2c_stop();
		DDRB = 0x00;						//enable pull up on TS line
		PORTB = 0x04;
		int32_t i = 0;
		while (((PINB >> 2) & 1) == 0)		//wait for TS line to get high
		{
			i++;
			if(i>=1000) 
			{
				return false;
			}
		}
		return true;
	} 
	else 
	{
		return false;
	}
}

void PAJ7620Init(void)						//initialize PAJ7620
{
	int i = 0;
	i2c_start(PAJ7620_ID_WRITE);						//select BANK 0
	i2c_write(0xEF);
	i2c_write(0x00);
	i2c_stop();
	for (i = 0; i < INIT_REG_ARRAY_SIZE; i++)			//write initialization data
	{
		i2c_start(PAJ7620_ID_WRITE);
		i2c_write(initRegisterArray[i][0]);
		i2c_write(initRegisterArray[i][1]);
		i2c_stop();
	}
	i2c_start(PAJ7620_ID_WRITE);						//select BANK 1
	i2c_write(0xEF);
	i2c_write(0x01);
	i2c_stop();
	i2c_start(PAJ7620_ID_WRITE);						//Enable near mode 240fps
	i2c_write(0x65);
	i2c_write(0x12);
	i2c_stop();
	i2c_start(PAJ7620_ID_WRITE);						//select BANK 0
	i2c_write(0xEF);
	i2c_write(0x00);
	i2c_stop();
}

void PAJ7620ReadGesture(void)				//read gesture from PAJ7620
{
	i2c_start(PAJ7620_ID_WRITE);			//write Address for register 0x43
	i2c_write(0x43);
	i2c_stop();
	i2c_start(PAJ7620_ID_READ);				//read register 0x43
	PAJ7620receive = i2c_read_nack();
	i2c_stop();	
}

void init(void)								//choose init of selected sensor
{
	#if SENSOR_SELECT == PAJ7620
		PAJ7620Init();
	#elif SENSOR_SELECT == MGC3130 
		MGC3130Init();
	#endif
	COUNTER0Init();
}

void read_gesture(void)						//choose read function for selected sensor
{
	#if SENSOR_SELECT == PAJ7620
		PAJ7620ReadGesture();
	#elif SENSOR_SELECT == MGC3130 
		MGC3130ReadGesture();
	#endif
}

int process_gesture(void)					//process received data to output right gesture
{
	#if SENSOR_SELECT == PAJ7620
		switch (PAJ7620receive)
		{
			case 0x00 :						//No Gesture
				gesture = 0;
				break;
			case 0x01 :						//Left
				gesture = LEFT;
				break;
			case 0x02 :						//Right
				gesture = RIGHT;
				break;
			case 0x04 :						//Down
				gesture = DOWN;
				break;
			case 0x08 :						//Up
				gesture = UP;
				break;
			case 0x10 :						//Forward
				gesture = FORWARD;
				break;
			case 0x20 :						//Backward
				gesture = BACKWARD;
				break;
			case 0x40 :						//Clockwise
				gesture = CLOCKWISE;
				break;
			case 0x80 :						//Counterclockwise
				gesture = COUNTERCLOCKWISE;
				break;
		}
	#elif SENSOR_SELECT == MGC3130 
		if(MGC3130receive[10] == gestureold)
		{
			MGC3130receive[10] = 0;
		}
		switch (MGC3130receive[10])
		{
			case 0x00 :						//No Gesture
				gesture = 0;
				break;
			case 0x01 :						//Garbage model
				gesture = 0;
				break;
			case 0x02 :						//Flick West to East
				gesture = RIGHT;
				break;
			case 0x03 :						//Flick East to West
				gesture = LEFT;
				break;
			case 0x04 :						//Flick South to North
				gesture = UP;
				break;
			case 0x05 :						//Flick North to South
				gesture = DOWN;
				break;
			case 0x06 :						//Circle clockwise
				gesture = CLOCKWISE;
				break;
			case 0x07 :						//Circle counterclockwise
				gesture = COUNTERCLOCKWISE;
				break;
			case 0x40 :						//Hold
				gesture = HOLD;
				break;
			case 0x49 :						//Presence
				gesture = 0;
				break;
		}
		gestureold = MGC3130receive[10];
	#endif
	gesturedelay = 0;
	while(gesturedelay < 80 && gesture != 0)
	{
		#if SENSOR_SELECT == PAJ7620
			LED1_ON;
			switch (PAJ7620receive)
			{
				case 0x40 :						//Clockwise
					gesture = CLOCKWISE;
					break;
				case 0x80 :						//Counterclockwise
					gesture = COUNTERCLOCKWISE;
					break;
			}
		#elif SENSOR_SELECT == MGC3130
			LED2_ON;
			switch (MGC3130receive[10])
			{
				case 0x06 :						//Circle clockwise
					gesture = CLOCKWISE;
					break;
				case 0x07 :						//Circle counterclockwise
					gesture = COUNTERCLOCKWISE;
					break;			
				case 0x40 :						//Hold
					gesture = HOLD;
					break;
			}
		#endif
	}
	#if SENSOR_SELECT == PAJ7620
		LED1_OFF;
	#elif SENSOR_SELECT == MGC3130 
		LED2_OFF;
	#endif
	return gesture;
}

void inputdelay(void)						//delay for process_gesture
{
	if(gesturedelay <=80)
	{
		gesturedelay++;
	}
	
}

int encoder(int volume)						//sub function for simulating encoder
{
	if (volume == 1)						//volume up
	{
		switch (volumestate)
		{
			case 0:
				PIN_A_ON;
				PIN_B_ON;
				volumestate = 1;
				break;
			case 1:
				PIN_A_OFF;
				volumestate = 2;
				break;
			case 2:
				PIN_B_OFF;
				volumestate = 3;
				break;
			case 3:
				PIN_A_ON;
				volumestate = 4;
				break;
			case 4:
				PIN_B_ON;
				volumestate = 5;
				break;
			case 5:
				PIN_A_OFF;
				volumestate = 6;
				break;
			case 6:
				PIN_B_OFF;
				volumestate = 7;
				break;
			case 7:
				PIN_A_ON;
				volumestate = 8;
				break;
			case 8:
				PIN_B_ON;
				volumestate = 0;
				volume = 0;
				break;
		}
	}
	else if (volume == 2)					//volume down
	{
		switch (volumestate)
		{
			case 0:
				PIN_A_ON;
				PIN_B_ON;
				volumestate = 1;
				break;
			case 1:
				PIN_B_OFF;
				volumestate = 2;
				break;
			case 2:
				PIN_A_OFF;
				volumestate = 3;
				break;
			case 3:
				PIN_B_ON;
				volumestate = 4;
				break;
			case 4:
				PIN_A_ON;
				volumestate = 5;
				break;
			case 5:
				PIN_B_OFF;
				volumestate = 6;
				break;
			case 6:
				PIN_A_OFF;
				volumestate = 7;
				break;
			case 7:
				PIN_B_ON;
				volumestate = 8;
				break;
			case 8:
				PIN_A_ON;
				volumestate = 0;
				volume = 0;
				break;
		}
	}
	return volume;
}