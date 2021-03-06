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
#include "stm32l4xx_hal.h"
#include "dfsdm.h"
#include "i2c.h"
#include "quadspi.h"
#include "spi.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef FLASH_BANK1
#define MESSAGE1    "BANK 1"
#else
#define MESSAGE1    "BANK 2"
#endif
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_OBProgramInitTypeDef OBInit;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

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
	MX_DFSDM1_Init();
	MX_I2C2_Init();
	MX_QUADSPI_Init();
	MX_SPI3_Init();
	MX_USART1_UART_Init();
	MX_USART3_UART_Init();
	MX_USB_OTG_FS_PCD_Init();
	/* USER CODE BEGIN 2 */

	HAL_UART_Transmit(&huart1, (uint8_t *)MESSAGE1, sizeof(MESSAGE1), 1000);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (HAL_GPIO_ReadPin(BUTTON_EXTI13_GPIO_Port, BUTTON_EXTI13_Pin) == GPIO_PIN_RESET)
		{
			while (HAL_GPIO_ReadPin(BUTTON_EXTI13_GPIO_Port, BUTTON_EXTI13_Pin) == GPIO_PIN_RESET)
				;

			/* Set BFB2 bit to enable boot from Flash Bank2 */
			/* Allow Access to Flash control registers and user Flash */
			HAL_FLASH_Unlock();

			/* Clear OPTVERR bit set on virgin samples */
			__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

			/* Allow Access to option bytes sector */
			HAL_FLASH_OB_Unlock();

			/* Get the Dual boot configuration status */
			HAL_FLASHEx_OBGetConfig(&OBInit);

			/* Enable/Disable dual boot feature */
			OBInit.OptionType = OPTIONBYTE_USER;
			OBInit.USERType = OB_USER_BFB2;

			if (((OBInit.USERConfig) & (OB_BFB2_ENABLE)) == OB_BFB2_ENABLE)
			{
				OBInit.USERConfig = OB_BFB2_DISABLE;
			}
			else
			{
				OBInit.USERConfig = OB_BFB2_ENABLE;
			}

			if (HAL_FLASHEx_OBProgram(&OBInit) != HAL_OK)
			{
				/*
				 Error occurred while setting option bytes configuration.
				 User can add here some code to deal with this error.
				 To know the code error, user can call function 'HAL_FLASH_GetError()'
				 */
				/* Infinite loop */
				while (1)
				{
					HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
					HAL_GPIO_TogglePin(LED3_WIFI__LED4_BLE_GPIO_Port, LED3_WIFI__LED4_BLE_Pin);
					HAL_Delay(1000);
				}
			}

			/* Start the Option Bytes programming process */
			if (HAL_FLASH_OB_Launch() != HAL_OK)
			{
				/*
				 Error occurred while reloading option bytes configuration.
				 User can add here some code to deal with this error.
				 To know the code error, user can call function 'HAL_FLASH_GetError()'
				 */
				/* Infinite loop */
				while (1)
				{
					HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
					HAL_GPIO_TogglePin(LED3_WIFI__LED4_BLE_GPIO_Port, LED3_WIFI__LED4_BLE_Pin);
					HAL_Delay(1000);
				}
			}
		}
		else
		{
#ifdef FLASH_BANK1
			/* Toggle LED3 */
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
#else
			/* Toggle LED3 */
			HAL_GPIO_TogglePin(LED3_WIFI__LED4_BLE_GPIO_Port, LED3_WIFI__LED4_BLE_Pin);
#endif

			/* Insert 50 ms delay */
			HAL_Delay(50);
		}

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
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Configure LSE Drive Capability
	 */
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_DFSDM1 | RCC_PERIPHCLK_USB;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
	PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
	PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/**Enable MSI Auto calibration
	 */
	HAL_RCCEx_EnableMSIPLLMode();

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

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
