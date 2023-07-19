#include "CCS811.h"
 #include "main.h"
 
ccs811_measurement_t CCS;
 

 
#define nWAKE_HIGN() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET)
#define nWAKE_LOW() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)
#define nINT_LOW() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET)
#define nINT_HIGN() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET) 
 
 
//�鿴�Ƿ��������� ����ֵ 1����
//						  0��û��
uint8_t CheckData_Ready(void)
{
		readStatus();
	if((CCS.status&8)==1) return 1;
	else return 0;
}
 
//��ȡһ��bit����
uint8_t CCS811_ReadOneByte(uint8_t ReadAddr)
{
	uint8_t temp=0;
		nWAKE_LOW();
		delay_us(50);//����ȴ�50us
		IIC_Start();  						
		//addr�ӵأ�7λ��ַΪ0x5A������ַΪAB,д��ַAA
		IIC_Send_Byte(WRIT);	   //����д����
		if(IIC_Wait_Ack()){IIC_Stop(); return 254;}
		//else nINT=0;
		IIC_Send_Byte(ReadAddr);
		if(IIC_Wait_Ack()){IIC_Stop(); return 255;}
		//if(!IIC_Wait_Ack()){nINT=1;delay_ms(200);nINT=0;delay_ms(200);}
		delay_ms(1);
		IIC_Start();
		IIC_Send_Byte(READ);
		if(IIC_Wait_Ack()){IIC_Stop(); return 253;}
		temp=IIC_Read_Byte(0); 
 
		IIC_Stop();
		nWAKE_HIGN();
		delay_us(50);
	return temp;
}
 
void CCS811_WriteOneByte(uint8_t RegAddr,uint8_t DataToWrite)
{
	nWAKE_LOW();
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(WRIT);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(RegAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);
	IIC_Wait_Ack();
	IIC_Stop();
	nWAKE_HIGN();
}
//��������ǰ���뿪��
void app_Start(void)
{
	nWAKE_LOW();
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(WRIT);	   //����д����
	if(IIC_Wait_Ack())
	{
	IIC_Stop();return;
	}
	IIC_Send_Byte(0xF4);
	delay_us(180);			//����ȴ�180us���ϲ��ܹ��յ�Ӧ��
	if(IIC_Wait_Ack()){IIC_Stop();return;}
	IIC_Stop();
	nWAKE_HIGN();
 
}
 
void readStatus(void)
{
  CCS.status=CCS811_ReadOneByte(STATUS);
}
 
 
void readHW_ID(void)
{
	CCS.hw_id=CCS811_ReadOneByte(HW_ID);
}
 
 
void readErrorID(void)
{
	CCS.error_id=CCS811_ReadOneByte(ERROR_ID);
}
 
void setMode(uint8_t mode)
{
//	nWAKE=0;
//	delay_us(50);
	CCS811_WriteOneByte(MEAS_MODE,mode);
//	nWAKE=1;
}
	
 
//һ�ζ�8bit���ݣ�ǰ2bitΪeco2���ݣ�����2bitΪtvoc����
//��status����λΪ1ʱ�������ݡ�
void getData(void)
{
	uint8_t i,buffer[8];
	
	nWAKE_LOW();
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(WRIT);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(ALG_RESULT_DATA);
	IIC_Wait_Ack();
	IIC_Stop();
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(READ);	   //����д����
	IIC_Wait_Ack();
	for(i=0;i<8-1;i++)
	buffer[i]=IIC_Read_Byte(1);
 
	buffer[i]=IIC_Read_Byte(0);
	IIC_Stop();
	nWAKE_HIGN();
	
	CCS.eco2 = ((uint16_t)buffer[0] << 8) + buffer[1];
	CCS.tvoc = ((uint16_t)buffer[2] << 8) + buffer[3];
	CCS.status=buffer[4];
	
}
//˯��ģʽ
void sleep(void)
{
	CCS811_WriteOneByte(MEAS_MODE,0x00000000);
}
 
//�����ã��ص�bootģʽ
void reset(void)
{
	uint8_t ResetData[4]={0X11,0XE5,0X72,0X8A};
	nWAKE_LOW();
	delay_us(50);
	IIC_Start(); 
	IIC_Send_Byte(WRIT);	   //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(SW_RESET);
	IIC_Wait_Ack();
	IIC_Send_Byte(0X11);
	IIC_Wait_Ack();
	IIC_Send_Byte(0XE5);
	IIC_Wait_Ack();
	IIC_Send_Byte(0X72);
	IIC_Wait_Ack();
	IIC_Send_Byte(0X8A);
	IIC_Wait_Ack();
	IIC_Stop();
	nWAKE_HIGN();
}

