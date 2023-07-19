/* USER CODE BEGIN Header */
/**
  ******************************************************************************

    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    pb12     ------> CS
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BSP_GPIO.h"
#include "BSP_USART.h"
#include "stdio.h"	
#include "BSP_spi.h"
#include "w25qxx.h"
#include "CCS811.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int ffputc(int ch, FILE *f)
{      
	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
    USART1->RDR = (u8) ch;      
	return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  float K_Temperature ; //K型热电偶 温度值
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
 MAX6675_Init();  

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	
	
	HAL_Delay(1000);
	
  readHW_ID();
//  printf("id:%d\r\n",CCS.hw_id);
  readStatus();
//  printf("sta0:%d\r\n",CCS.status);
  app_Start();
  readStatus();
//  printf("sta1:%d\r\n",CCS.status);
  setMode(0x10);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	__HAL_SPI_ENABLE(&hspi2);
	
	
	//串口屏数据初始化
		printf("t4.txt=\"0\"");//向串口屏发送指令，字符串类型
		printf("\xff\xff\xff");//发送结束符0xff、0xff、0xff
		printf("t8.txt=\"0\"");//向串口屏发送指令，字符串类型
		printf("\xff\xff\xff");//发送结束符0xff、0xff、0xff
		printf("t10.txt=\"致癌物指标正常\"");//向串口屏发送指令，字符串类型
		printf("\xff\xff\xff");//发送结束符0xff、0xff、0xff
		printf("t10.pco=BLACK");//向串口屏发送指令，字符串类型
		printf("\xff\xff\xff");//发送结束符0xff、0xff、0xff
		
		HAL_UART_Transmit(&huart2, "The VOCs are low*",40, 1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		if(MAX6675_ReadTemperature()<1000)
		K_Temperature = MAX6675_ReadTemperature();  //读取热电偶温度值
//		printf("Temperature:%f\r\n",K_Temperature);
		printf("t1.txt=\"%4.2f\"",K_Temperature);
		printf("\xff\xff\xff");//发送结束符0xff、0xff、0xff
		
	  getData();
//	  printf("eco2:%d\t",CCS.eco2);
//	  printf("tvoc:%d\r\n",CCS.tvoc);
		printf("t4.txt=\"%d\"",CCS.eco2);//向串口屏发送指令，字符串类型
		printf("\xff\xff\xff");//发送结束符0xff、0xff、0xff
		printf("t8.txt=\"%d\"",CCS.tvoc);//向串口屏发送指令，字符串类型
		printf("\xff\xff\xff");//发送结束符0xff、0xff、0xff
	  readStatus();
//	  printf("sta:%d\r\n",CCS.status);

		
	if(CCS.tvoc>50)//tvoc的浓度值高于50时
	{ 
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);//蜂鸣器发出警报
		HAL_UART_Transmit(&huart2, "The VOCs are off the charts*",40, 1000);//物联网模块同时发出警报信息
		printf("t10.txt=\"致癌物超标！！！\"");//串口屏显示警报信息
		printf("\xff\xff\xff");//发送结束符0xff、0xff、0xff
		printf("t10.pco=RED");//向串口屏发送指令，字符串类型
		printf("\xff\xff\xff");//发送结束符0xff、0xff、0xff
	}
	else  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
		
	HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
