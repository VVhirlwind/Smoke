#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_
#include "main.h"
#include "spi.h"

#define MAX6675_CS_Pin GPIO_PIN_12
#define MAX6675_CS_GPIO_Port GPIOB
extern SPI_HandleTypeDef hspi2;


void GPIO_Init(void);
void MAX6675_Init(void);
uint8_t MAX6675_ReadWriteByte(uint8_t txData);
uint16_t MAX6675_ReadRawValue(void);
float MAX6675_ReadTemperature(void);
void MAX6675_CS(unsigned char choose);
u8 SPI2_ReadWriteByte(u8 TxData);


#endif
