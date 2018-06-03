/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "crc.h"
#include "modbus.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t modbusResponse[20];
uint8_t modbusData[8];
int resultValue,readContol;
int raspDataControl = 0;
uint8_t raspData[12];
char raspRet[12];
int functionCode;
int value;
int s, temp;
int storeLoc = 0;
int waitCounter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  int deneme = 1;
  raspDataControl = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(raspDataControl == 0){
      // waiting raspberry pi data
      HAL_UART_Receive_IT(&huart1, (uint8_t*)raspData, 6);
      raspDataControl = 2;
    }
    else if(waitCounter == 1){
      // raspberry pi sent the data
      HAL_Delay(5);
      
      s = 0;
      temp = 0;
      for(int i = 0; i < 6; i++){
        if(raspData[i] == 'a'){
          s = i;
          break;
        }
      }
      if(s == 1){
        functionCode = raspData[0] - '0';
      }
      else if(s == 2){
        int temp1 = raspData[0] - '0';
        temp1 = temp1 * 10;
        int temp2 = raspData[1] - '0';
        functionCode = temp1 + temp2;
      }
      temp = s;
      s = s + 1;
      for(int i = s; i < 6; i++){
        if(raspData[i] == 'a'){
          s = i - 1;
          break;
        }
        else if(i == 5){
          s = i;
          break;
        }
      }
      HAL_Delay(3);
      int tempDif = s - temp;
      if(tempDif == 3){
        int temp1 = raspData[s - 2] - '0';
        temp1 = temp1 * 100;
        int temp2 = raspData[s - 1] - '0';
        temp2 = temp2 * 10;
        int temp3 = raspData[s] - '0';
        value = temp1 + temp2 + temp3;
      }
      else if((s - temp) == 2){
        int temp1 = raspData[s - 1] - '0';
        temp1 = temp1 * 10;
        int temp2 = raspData[s] - '0';
        value = temp1 + temp2;
      }
      else if((s - temp) == 1){
        if(raspData[s] == 'b'){
          value = -1;
        }
        else{
          value = raspData[s] - '0';
        }
      }
      // Perde
      if(functionCode == 50){
        if(value == -1){
          // Data read
          int tempr = (storeLoc - 1000) / 10;
          char dummy[20] = "dummyyoy";
          if(tempr >= 0 && tempr <= 9){
            HAL_UART_Transmit(&huart2, (uint8_t*)dummy, 8, 1000);
            HAL_Delay(2);
            sprintf(raspRet, "00%d", tempr);
            HAL_UART_Transmit(&huart2, (uint8_t*)raspRet, 3, 1000);
          }
          else if(tempr >= 10 && tempr <= 99){
            HAL_UART_Transmit(&huart2, (uint8_t*)dummy, 8, 1000);
            HAL_Delay(2);
            sprintf(raspRet, "0%d", tempr);
            HAL_UART_Transmit(&huart2, (uint8_t*)raspRet, 3, 1000);
          }
          else{
            HAL_UART_Transmit(&huart2, (uint8_t*)dummy, 8, 1000);
            HAL_Delay(2);
            sprintf(raspRet, "%d", tempr);
            HAL_UART_Transmit(&huart2, (uint8_t*)raspRet, 3, 1000);
          }
        }
        else{
          // Data write
          if(value == 1){
            value = 2000;
          }
          else{
            value = value / 10;
            value = (value * 10) + 1000;
          }
          
          storeLoc = value;
          __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,storeLoc);
          HAL_Delay(3);
        }
      }
      // Isik
      else if(functionCode == 51){
        // resetting light pin
        if(value == 0){
          HAL_GPIO_WritePin(Light_GPIO_Port,Light_Pin,GPIO_PIN_RESET);
        }
        // setting light pin
        else{
          HAL_GPIO_WritePin(Light_GPIO_Port,Light_Pin,GPIO_PIN_SET);
        }
      }
      else{
        if(value == -1){
          //read data
          createModBusData(01,03,functionCode,01,modbusData);
          readContol = 1;
          readSingleRegister(&huart2,modbusData,modbusResponse);
          while(readContol !=0){}
          if(resultValue >= 0 && resultValue <= 9){
            sprintf(raspRet, "%00d", resultValue);
            HAL_UART_Transmit(&huart2, (uint8_t*)raspRet, 3, 1000);
          }
          else if(resultValue >= 10 && resultValue <= 99){
            sprintf(raspRet, "%0d", resultValue);
            HAL_UART_Transmit(&huart2, (uint8_t*)raspRet, 3, 1000);
          }
          else{
            sprintf(raspRet, "%d", resultValue);
            HAL_UART_Transmit(&huart2, (uint8_t*)raspRet, 3, 1000);
          }
        }
        else{
          // write data
          createModBusData(01,06,functionCode,value,modbusData);
          writeSingleRegister(&huart2,modbusData,modbusResponse);
          HAL_Delay(50);
        }
      }
      
      raspDataControl = 0;
      waitCounter = 0;
    }
    else{
      // do nothing.
    }
    /*
    if(0){
      //Klima sinyali gelirse
      for(int i = 1000; i < 2001; i+=20){
        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,i);
        HAL_Delay(33);
       }
      for(int i = 2000; i > 999; i-=20){
        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,i);
        HAL_Delay(33);
       }
    }
    else if(0){
      if(deneme == 1){
         
        //Write example
        //createModBusData(01,06,02,05,modbusData);
        //writeSingleRegister(&huart2,modbusData,modbusResponse);
        //deneme = 0;
        //HAL_Delay(500);
        //Read example
        createModBusData(01,03,03,01,modbusData);
        readContol = 1;
        readSingleRegister(&huart2,modbusData,modbusResponse);
        deneme = 0;
        HAL_Delay(500);
         //HAL_GPIO_TogglePin(Led_Blue_GPIO_Port, Led_Blue_Pin);
                    char aa[20];
            sprintf(aa, "%d", resultValue);
        HAL_UART_Transmit(&huart2, (uint8_t*)aa, 3, 1000);
        
      }
    }
    else if(1){
      if(deneme==1){
        readContol = 5;
        HAL_UART_Receive_IT(&huart1, (uint8_t*)raspData, 6);
        while(readContol == 5){}
        createModBusData(01,03,03,01,modbusData);
        readContol = 1;
        readSingleRegister(&huart2,modbusData,modbusResponse);
        deneme = 0;
        HAL_Delay(500);
        char aa[20];
        sprintf(aa, "%d", resultValue);
        HAL_UART_Transmit(&huart2, (uint8_t*)aa, 3, 1000);
        
        char aa[20] = "deneme\n";
        HAL_UART_Transmit(&huart2, (uint8_t*)aa, 7, 1000);
        HAL_UART_Receive_IT(&huart1, (uint8_t*)raspData, 12);
        HAL_UART_Transmit(&huart2, (uint8_t*)aa, 7, 1000);
        //char aa[20] = "deneme\n";
        //HAL_UART_Transmit(&huart2, (uint8_t*)aa, 7, 1000);
        deneme = 0;
        
      }
    }
  */
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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 15;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 19999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim1);

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, Light_Pin|Modbus_Control_Pin|Led_Green_Pin|Led_Orange_Pin 
                          |Led_Red_Pin|Led_Blue_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Light_Pin Modbus_Control_Pin Led_Green_Pin Led_Orange_Pin 
                           Led_Red_Pin Led_Blue_Pin */
  GPIO_InitStruct.Pin = Light_Pin|Modbus_Control_Pin|Led_Green_Pin|Led_Orange_Pin 
                          |Led_Red_Pin|Led_Blue_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if(readContol == 1){
    readContol = 0;
    int retL,retH;
    retL = (modbusResponse[3] & 0x0F) * (16*16);
    retH = ((modbusResponse[3] & 0xF0) >> 4) * (16*16*16);
    resultValue = retL + retH + (int)modbusResponse[4];
  }
  if(raspDataControl == 2){
    raspDataControl = 1;
    HAL_GPIO_TogglePin(Led_Blue_GPIO_Port, Led_Blue_Pin);
    waitCounter = 1;
  }
  
  if(huart == &huart1){
    // rasp data came from raspberry pi
    int rec[1];
    rec[0] = raspData[0] * 10;
    HAL_UART_Transmit(&huart1, (uint8_t*)rec, 1, 1000);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
