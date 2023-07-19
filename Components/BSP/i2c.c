/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"
#include "delay.h"
 
 
 
 
 
 
 
 
#define HT_SDA GPIO_PIN_7 //B7
#define HT_SCK GPIO_PIN_6 //B6
 
static void SDA_IN(){
 GPIO_InitTypeDef GPIO_InitStruct = {0};
 
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
 
  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = HT_SDA;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
 
static void SDA_OUT(){
 GPIO_InitTypeDef GPIO_InitStruct = {0};
 
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
 
  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = HT_SDA;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void SDA_HIGH(){
  HAL_GPIO_WritePin(GPIOB, HT_SDA, GPIO_PIN_SET);
}
 
void SDA_LOW(){
  HAL_GPIO_WritePin(GPIOB, HT_SDA, GPIO_PIN_RESET);
}
 
void SCL_HIGH(){
  HAL_GPIO_WritePin(GPIOB, HT_SCK, GPIO_PIN_SET);
}
 
void SCL_LOW(){
  HAL_GPIO_WritePin(GPIOB, HT_SCK, GPIO_PIN_RESET);
}
 
GPIO_PinState read_sdaval(){
   return HAL_GPIO_ReadPin(GPIOB,HT_SDA);
}
 
void IIC_Start(void)
{
	SDA_OUT(); 
 
	SDA_HIGH();
	SCL_HIGH();
	delay_us(5);
	SDA_LOW();//START:when CLK is high,DATA change form high to low
	delay_us(5);
	SCL_LOW();
	delay_us(5);
}	  
 
void IIC_Stop(void)
{
	SDA_OUT();
 
	SCL_LOW();
	SDA_LOW();//STOP:when CLK is high DATA change form low to high
 	delay_us(5);
 	SCL_HIGH();
	SDA_HIGH();
	delay_us(5);							   	
}
 
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN(); //
	SDA_HIGH();delay_us(1);
	SCL_HIGH();delay_us(1);
	while(read_sdaval())
	{
		ucErrTime++;
		delay_us(1);
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL_LOW();
	return 0;  
} 
 
void IIC_Ack(void)
{
	SCL_LOW();
	SDA_OUT();
 
	SDA_LOW();
	delay_us(20);
	SCL_HIGH();
	delay_us(5);
	SCL_LOW();
}
 
void IIC_NAck(void)
{
	SCL_LOW();
	SDA_OUT();
 
	SDA_HIGH();
	delay_us(5);
	SCL_HIGH();
	delay_us(5);
	SCL_LOW();
}					 				     
 
void IIC_Send_Byte(uint8_t txd)
{                        
	uint8_t t;
	SDA_OUT();
 
	SCL_LOW();
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			SDA_HIGH();
		else
			SDA_LOW();
		txd<<=1; 	  
		delay_us(50);
		SCL_HIGH();
		delay_us(50); 
		SCL_LOW();
		delay_us(50); 
		
    }	 
 
} 	    
 
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
    	SCL_LOW();
        delay_us(50);
        SCL_HIGH();
        receive<<=1;
        delay_us(25);
        if(read_sdaval())
        {
        	receive++;
        }
		delay_us(25);
    }					 
    if (!ack)
        IIC_NAck();//nACK
    else
        IIC_Ack(); //ACK
    return receive;
}



