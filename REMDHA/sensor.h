/*
 * sensor.h
 *
 * Created: 12.04.2019 16:11:17
 *  Author: nbarben
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#define PAJ7620			0
#define MGC3130			1
#define SENSOR_SELECT	PAJ7620							//choose Sensor MGC3130 / PAJ7620


#define RIGHT				1							//define value for gestures
#define LEFT				2
#define DOWN				3
#define UP					4
#define CLOCKWISE			5
#define COUNTERCLOCKWISE	6
#define HOLD				7
#define FORWARD				8
#define BACKWARD			9

#define LED1_ON				PORTD |= (1 << 6)			//define states of LED
#define LED1_OFF			PORTD &= ~(1 << 6)
#define LED2_ON				PORTD |= (1 << 7)
#define LED2_OFF			PORTD &= ~(1 << 7)

#define RUFTON				PORTD |= 1					//define outputs
#define FUNKTION			PORTD |= (1 << 1)
#define TOE					PORTC |= (1 << 1)
#define SPRECHEN			PORTC |= (1 << 2)
#define LICHT				PORTC |= (1 << 3)

#define PIN_A_ON			PORTD |= (1 << 2)			//encoder pins
#define PIN_A_OFF			PORTD &= ~(1 << 2)
#define PIN_B_ON			PORTC |= (1 << 0)
#define PIN_B_OFF			PORTC &= ~(1 << 0)


#define MGC3130_ID			0x42						//I2C ID MGC3130
#define MGC3130_ID_READ		(MGC3130_ID << 1) + 1
#define MGC3130_ID_WRITE	(MGC3130_ID << 1) + 0

#define PAJ7620_ID			0x73						//I2C ID PAJ7620
#define PAJ7620_ID_READ		(PAJ7620_ID << 1) + 1
#define PAJ7620_ID_WRITE	(PAJ7620_ID << 1) + 0
#define INIT_REG_ARRAY_SIZE (sizeof(initRegisterArray)/sizeof(initRegisterArray[0]))	//read out size of array of data for initializing PAJ7620

void COUNTER0Init(void);								//initialize COUNTER0

//MGC3130//////////////////////////////////////////////////////////////////////////////////////////
bool MGC3130Init(void);									//initialize MGC3130
bool MGC3130ReadGesture(void);							//read gesture from MGC3130

//PAJ7620//////////////////////////////////////////////////////////////////////////////////////////
void PAJ7620Init(void);									//initialize PAJ7620
void PAJ7620ReadGesture (void);							//read gesture from PAJ7620


void init(void);										//choose init of selected sensor
void read_gesture(void);								//choose read function for selected sensor
int process_gesture(void);								//process received data to output right gesture
void inputdelay(void);									//delay for process_gesture

int encoder(int volume);								//sub function for simulating encoder

#endif /* SENSOR_H_ */