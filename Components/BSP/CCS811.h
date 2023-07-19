#ifndef CCS811_h
#define CCS811_h
 
#include "delay.h"
#include "i2c.h"
#include "main.h"
 
#define boolean uint8_t
#define byte 	uint8_t
 
  
 
#define nWAKE_HIGN() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET)
#define nWAKE_LOW() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)
#define nINT_LOW() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET)
#define nINT_HIGN() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET) 
 
 
 
typedef struct {
	uint16_t eco2;
	uint16_t tvoc;
	uint8_t status;
	uint8_t error_id;
	uint8_t hw_id;
	uint16_t raw_data;
} ccs811_measurement_t;
 
#define CCSADDR				0x5A<<1 // when I2C_ADDR pin is LOW
// #define CCSADDR					0x5B<<1 when I2C_ADDR pin is HIGH
#define READ            	0xB5  
#define WRIT            	0xB4   
// Registers for CCS811
#define STATUS          	0x00
#define MEAS_MODE       	0x01
#define ALG_RESULT_DATA 	0x02
#define RAW_DATA			0x03
#define ENV_DATA        	0x05
#define NTC        			0x06
#define THRESHOLDS      	0x10
#define BASELINE      		0x11
#define HW_ID           	0x20
#define HW_Version 			0x21
#define FW_Boot_Version		0x23
#define FW_App_Version  	0x24
#define ERROR_ID        	0xE0
#define SW_RESET        	0xFF
 
#define MODE0 				0x00    
#define MODE1 				0x10	
#define MODE2 				0x20	
#define MODE3 				0x30	
#define MODE4 				0x40	
 
extern ccs811_measurement_t CCS;
 
void readStatus(void);
void readHW_ID(void);
void readErrorID(void);
void readALG_RESULT_DATA(uint8_t *data);
void app_Start(void);
	
int readTVOC(void);
int readCO2(void);
void getData(void);
void compensate(float t, float rh);
void setMode(uint8_t mode);
 
void reset(void);
void sleep(void);
#endif


