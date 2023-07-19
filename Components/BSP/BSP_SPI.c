/**
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  * @file       Drv_spi.c/h
  * @brief      将HAL库SPI函数进行二次封装，给相应的器件进行调用
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V3.0.0     2020.7.14     	              	1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  */
#include "BSP_spi.h"
#include "spi.h"

/**
  * @brief          封装SPI2读写函数（片外SPI Flash使用）
  * @param[in]     	TxData 发出的数据
  * @retval         RxData 收到的数据
  */

//MAX6675 片选引脚初始化
void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, MAX6675_CS_Pin, GPIO_PIN_SET);
  /*引脚配置 */
  GPIO_InitStruct.Pin = MAX6675_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  max66675模块初始化
  * @param  None
  * @retval None
  */
void MAX6675_Init(void)
{
  GPIO_Init();  //片选引脚初始化
  MX_SPI2_Init();  //spi总线初始化
} 
 
/**
  * @brief  max6675模块读写一个字节的数据
  * @param  txData：要发送的数据
  * @retval 接收到的数据
  */
uint8_t MAX6675_ReadWriteByte(uint8_t txData)
{		
  unsigned char txdata,rxdata;
  txdata = txData;
  HAL_SPI_TransmitReceive(&hspi2,&txdata,&rxdata,1,1000);
  return rxdata;
	
//	 unsigned char temp=0;
//    unsigned char dat=0;
//    for(temp=0x80; temp!=0; temp>>=1)
//    {
////        SCK=0;
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
////        SPI_delay();  //让SCK稳定
//				HAL_Delay(100);
////        SCK=1;
//						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
////        SPI_delay();  //让SCK稳定
//				HAL_Delay(100);

//        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)==1)
//        {
//            dat|=temp;
//        }
//        else
//        {
//            dat&=~temp;
//        }
//    }
//    return dat;
}  
 
/**
  * @brief  max6675模块读取测得的原始数据
  * @param  None
  * @retval 温度的原始数据
  */
uint16_t MAX6675_ReadRawValue(void)
{
  uint16_t tmp=0;
  MAX6675_CS(0);
  tmp = MAX6675_ReadWriteByte(0XFF); //read MSB
  tmp <<=8;
  tmp |= MAX6675_ReadWriteByte(0XFF); //read LSB
  
  MAX6675_CS(1);
  
  if (tmp & 4) 
  {
    // thermocouple open
    tmp = 4095; //未检测到热电偶
  }
  else 
  {
    tmp = tmp >>3;
  }
  return tmp;
}
 
/**
  * @brief  max6675模块读取温度
  * @param  None
  * @retval 温度值（单位：℃）
  */
float MAX6675_ReadTemperature(void)
{
  return (MAX6675_ReadRawValue() * 1024.0 / 4096);
}

//MAX6675 片选控制
void MAX6675_CS(unsigned char choose)
{
  if(choose == 1)
  {
    HAL_GPIO_WritePin(GPIOB, MAX6675_CS_Pin, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOB, MAX6675_CS_Pin, GPIO_PIN_RESET);
  }
}

u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&hspi2,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //返回收到的数据		
}


