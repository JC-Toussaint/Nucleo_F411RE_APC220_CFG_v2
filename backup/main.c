/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "hal_lcd.h"
#include "fonts.h"
#include "bluetooth_logo.h"

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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
HAL_StatusTypeDef hal_status = HAL_OK;

char buffer [UART_RX_BUFFER_SIZE];
int len;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

//static void OnError_Handler(uint32_t condition);
static void StartUartReception(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// WARNING UART2 is connected to st-link
// Properties -> Settings -> MCU_Settings -> select use float with printf
/*
 * USART2, BaudRate = 115200, WordLength = UART_WORDLENGTH_8B, StopBits = UART_STOPBITS_1;
 * */
int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2,(uint8_t *)ptr, len, 10);
	return len;
}

/*
int _write(int file, char *ptr, int len)
{
	for (uint8_t i=0; i<len; i++){
		LL_USART_TransmitData8(USART2, ptr[i]);
		while ( !LL_USART_IsActiveFlag_TXE(USART2) ){ ; }
	}
	return len;
}
 */

void UART_Transmit(uint8_t *data, uint16_t length)
{
	// Wait for TXE (transmit data register empty) flag to be set
	while((USART1->SR & USART_SR_TXE) == 0);

	// Write data to the data register
	for(uint16_t i = 0; i < length; i++)
	{
		USART1->DR = data[i];

		// Wait for TC (transmission complete) flag to be set
		while((USART1->SR & USART_SR_TC) == 0);
	}
}

int UART_Receive(char *data, uint16_t max_length)
{
	char c;
	int i = 0;
	data[0]=0;
	do {
		c = uartGetChar();
		if (c == '\r' || c == '\n') break;

		data[i++] = c;
		if (i >= max_length) break;
	}
	while (isUartRxNotEmpty());
	return i;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

#define noSSD1306
#ifdef SSD1306
	LCD_Init();
	LCD_PrintTest(0, 0, "test", SSD1306_COLOR_WHITE, LCD_CHAR_SMALL);

	SSD1306_Fill(SSD1306_COLOR_BLACK);

	FontDef_t Font = Font_11x18;
	SSD1306_GotoXY(0, 0);
    SSD1306_Puts("12345678901", &Font, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();

    SSD1306_GotoXY(0, 14);
    SSD1306_Puts("ABCDEFGHIJK", &Font, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
#endif

	printf("Start ...\n");
	StartUartReception();

	HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SET_GPIO_Port, SET_Pin, GPIO_PIN_SET);
	HAL_Delay(50); // 50ms at least

	HAL_GPIO_WritePin(SET_GPIO_Port, SET_Pin, GPIO_PIN_RESET); // pulling SET to low will put apc220 in config mode
	HAL_Delay(50); // stabilize please

	/*
  Parameters      Bytes Instruction
  Frequency       6     Unit is KHz,for example 434MHz is 434000.
  Rf data rate    1     1,2,3 and 4 refer to 2400, 4800, 9600, 19200bps separetely.
  Output power    1     from 0 to 9, 9 means 13dBm(20mW).
  UART rate       1     0,1,2,3,4,5 and 6 refers to 1200, 2400, 4800, 9600, 19200, 38400, 57600bps separately.
  Series checkout 1     Series checkout：0 means no check,1 means even parity,2 means odd parity.
	 */

//  len = snprintf ( buffer, 128, "WR 433900 4 9 3 0\r\n"); // ask for data
	len = snprintf ( buffer, 128, "WR 433900 4 9 3 0\r\n"); // ask for data

	printf("RX Flush\n");
	do {
		uartGetChar();
	}
	while (isUartRxNotEmpty());

	UART_Transmit((uint8_t *)buffer, len);
	HAL_Delay(50);

	printf("Write and Read\n");
	do {
		printf("%c", uartGetChar());
	}
	while (isUartRxNotEmpty());
	printf("\n");

	/* ------------------------*/
#define noBaudRate57600
#ifdef BaudRate57600
	LL_USART_Disable(USART1);
	LL_USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.BaudRate = 57600;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART1, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USART1);
	LL_USART_Enable(USART1);
#endif
	/* ------------------------*/

	HAL_GPIO_WritePin(SET_GPIO_Port, SET_Pin, GPIO_PIN_SET); // put apc220 back in operation
	HAL_Delay(200);

	HAL_GPIO_WritePin(SET_GPIO_Port, SET_Pin, GPIO_PIN_RESET); // pulling SET to low will put apc220 in config mode
	HAL_Delay(50); // stabilize please

	len = snprintf ( buffer, 128, "RD\r\n"); // ask for data
	UART_Transmit((uint8_t *)buffer, len);
	HAL_Delay(50);

	printf("Read\n");
	do {
		printf("%c", uartGetChar());
	}
	while (isUartRxNotEmpty());
	printf("\n");

	HAL_GPIO_WritePin(SET_GPIO_Port, SET_Pin, GPIO_PIN_SET); // put apc220 back in operation
	HAL_Delay(200);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//		len = UART_Receive(buffer, UART_RX_BUFFER_SIZE);
		//		for (int i=0; i<len; i++){
		//			printf("%d", buffer[i]);
		//		}
		//		printf("%s\n", buffer);
		//		if (len) printf("len %d\n");
#define SZ (32)
		char str[SZ];
		int sz=0;
		while (isUartRxNotEmpty()){
#ifdef SSD1306
			str[sz++] = uartGetChar();
#else
			printf("%c", uartGetChar());
#endif
		}
		str[sz]='\0';

		FontDef_t Font = Font_11x18;
		SSD1306_Fill(SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(0, 0);
	    SSD1306_Puts(str, &Font, SSD1306_COLOR_WHITE);
	    SSD1306_UpdateScreen();

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA10   ------> USART1_RX
  PA15   ------> USART1_TX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 9600;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|SET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin SET_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|SET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ENABLE_Pin */
  GPIO_InitStruct.Pin = ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ENABLE_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/**
 * @brief  On Error Handler on condition TRUE.
 * @param  condition : Can be TRUE or FALSE
 * @retval None
 */
//static void OnError_Handler(uint32_t condition)
//{
//	if(condition)
//	{
//	    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//		while(1) { ; } /* Blocking on error */
//	}
//}

/**
 * @brief  This function prints user info on PC com port and initiates RX transfer
 * @param  None
 * @retval None
 */
void StartUartReception(void)
{
	uartTxFlush();

	/* Clear Overrun flag, in case characters have already been sent to USART */
	LL_USART_ClearFlag_ORE(USART1);

	/* Enable RXNE and Error interrupts */
	LL_USART_EnableIT_RXNE(USART1);
	LL_USART_EnableIT_ERROR(USART1);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	while (1)
	{
	}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
