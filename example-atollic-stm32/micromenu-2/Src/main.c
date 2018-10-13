/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
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
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */

#include "hd44780.h"	// Бібліотека для дисплею WH1602
#include <stdbool.h>	// Бібліотека визначення змінних типу bool
#include "MicroMenu.h"	// Бібліотека мікро меню

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

bool flagButton[BUTTONn] = {};

Button_TypeDef key_pressed = BUTTON_NOTHING;

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {BUTTON_LEFT_GPIO_Port,
									  BUTTON_UP_GPIO_Port,
									  BUTTON_DOWN_GPIO_Port,
									  BUTTON_RIGHT_GPIO_Port,
									  BUTTON_SELECT_GPIO_Port};

const uint16_t BUTTON_PIN[BUTTONn] = {BUTTON_LEFT_Pin,
									  BUTTON_UP_Pin,
									  BUTTON_DOWN_Pin,
									  BUTTON_RIGHT_Pin,
									  BUTTON_SELECT_Pin};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

static uint8_t getPressKey(void);		// Функція отримання коду натиснутої кнопки
static void Generic_Write(const char* Text); // Функція виводу тексту на дисплей за замовчуванням

// Menus function
static void Level1Item3_Enter(void);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

// Menus  Name | Next | Prev | Parent | Child | SelectFunction | EnterFunction | Text
MENU_ITEM(Menu_1, Menu_2, Menu_3, NULL_MENU, Menu_1_1, NULL, NULL, "\tMenu-1");
MENU_ITEM(Menu_2, Menu_3, Menu_1, NULL_MENU, NULL_MENU,	NULL, NULL,	"\tMenu-2");
MENU_ITEM(Menu_3, Menu_1, Menu_2, NULL_MENU, NULL_MENU, NULL, Level1Item3_Enter, "  Press Enter\nON/OFF BACKLIGHT");

MENU_ITEM(Menu_1_1, Menu_1_2, Menu_1_2, Menu_1, Menu_1_1_1, NULL, NULL, "\tMenu-1.1");
MENU_ITEM(Menu_1_2, Menu_1_1, Menu_1_1, NULL_MENU, NULL_MENU, NULL, NULL, "\tMenu-1.2");

MENU_ITEM(Menu_1_1_1, NULL_MENU, NULL_MENU, Menu_1_1, NULL_MENU, NULL, NULL, "\tMenu-1.1.1");

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	bool flagPressed = false;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */

	lcdInit();
	HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_Port,LCD_BACKLIGHT_Pin,ENABLE);

	/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
	Menu_SetGenericWriteCallback(Generic_Write);
	Menu_Navigate(&Menu_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (getPressKey() != BUTTON_NOTHING && !flagPressed)
	  {
		  flagPressed = true;
		  switch(getPressKey())
		  {
			  case BUTTON_LEFT:
				  Menu_Navigate(MENU_PARENT);
				  break;
			  case BUTTON_UP:
				  Menu_Navigate(MENU_PREVIOUS);
				  break;
			  case BUTTON_DOWN:
				  Menu_Navigate(MENU_NEXT);
				  break;
			  case BUTTON_RIGHT:
				  Menu_Navigate(MENU_CHILD);
				  break;
			  case BUTTON_SELECT:
				  Menu_EnterCurrentItem();
				  break;
			  default:
				  break;
		  }
	  } else if (getPressKey() == BUTTON_NOTHING && flagPressed) {
		  flagPressed = false;
	}
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BUZZER_Pin|LCD_BACKLIGHT_Pin|LCD_D4_Pin|LCD_D5_Pin 
                          |LCD_D6_Pin|LCD_D7_Pin|LCD_RS_Pin|LCD_E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BUZZER_Pin LCD_BACKLIGHT_Pin LCD_D4_Pin LCD_D5_Pin 
                           LCD_D6_Pin LCD_D7_Pin LCD_RS_Pin LCD_E_Pin */
  GPIO_InitStruct.Pin = BUZZER_Pin|LCD_BACKLIGHT_Pin|LCD_D4_Pin|LCD_D5_Pin 
                          |LCD_D6_Pin|LCD_D7_Pin|LCD_RS_Pin|LCD_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON_SELECT_Pin BUTTON_LEFT_Pin BUTTON_UP_Pin BUTTON_DOWN_Pin 
                           BUTTON_RIGHT_Pin */
  GPIO_InitStruct.Pin = BUTTON_SELECT_Pin|BUTTON_LEFT_Pin|BUTTON_UP_Pin|BUTTON_DOWN_Pin 
                          |BUTTON_RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

static void Generic_Write(const char* Text)
{
	if (Text)			// Якщо є текст то...
	{
		lcdClrScr();	// Очистимо дисплей
		lcdPuts(Text);	// Надрукуємо текст
	}
}

static uint8_t getPressKey(void)
{
  for(uint8_t i = 0; i < BUTTONn; i++)
  {
	  if(HAL_GPIO_ReadPin(BUTTON_PORT[i],BUTTON_PIN[i]) && !flagButton[i])
		  {
			  flagButton[i] = true;
			  key_pressed = i;
		  }

	  if(!HAL_GPIO_ReadPin(BUTTON_PORT[i],BUTTON_PIN[i]) && flagButton[i])
	  {
		  flagButton[i] = false;
		  key_pressed = BUTTON_NOTHING;
	  }
  }
  return key_pressed;
}

static void Level1Item3_Enter(void)
{

	bool flagPressed = false;	// Прапорець натиснутої кнопки

	lcdClrScr();				// Очищаємо екран дисплею

	lcdGoto(LCD_1st_LINE,0);	// Друкуємо в першому рядку
	lcdPuts("  BackLight is");	// Назву параметру

// Поки не натиснути кнопка "ліворуч" тут виконує функцію як "вихід"
	while(getPressKey() != BUTTON_LEFT)
	{

		// Перевіримо в якому стані ніжка мікроконтролера куди під'єднано підсвічування дисплею
		if(HAL_GPIO_ReadPin(LCD_BACKLIGHT_GPIO_Port,LCD_BACKLIGHT_Pin))
		{
			lcdGoto(LCD_2nd_LINE,0);	// В другому рядку друкуємо стан параметру
			lcdPuts("\tON ");			// Підсвічування увімкнено
		} else {
			lcdGoto(LCD_2nd_LINE,0);	// В інакшому разі
			lcdPuts("\tOFF");			// Підсвічування вимкнено
		}
// Скануємо тільки кнопки "вгору" і "вниз"
		if(getPressKey() != BUTTON_NOTHING && !flagPressed)
		{
			flagPressed = true; // Коли якусь кнопку натиснули

			switch(getPressKey()) 	// Перевіряємо що натиснуто
			{
			case BUTTON_UP:			// Якщо кнопку "вгору", то увімкнемо підсвічування дисплею
				HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_Port,LCD_BACKLIGHT_Pin,ENABLE);
				break;
			case BUTTON_DOWN:		// Якщо кнопку "вниз", то вимкнемо підсвічування дисплею
				HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_Port,LCD_BACKLIGHT_Pin,DISABLE);
				break;
			default:				// В будь якому іншому випадку просто вихід з switch
				break;
			}
		} else if(getPressKey() == BUTTON_NOTHING && flagPressed) {
			flagPressed = false;	// Коли кнопку відпустили
		}
	}

	lcdClrScr();			// Очищення дисплею
	Menu_Navigate(&Menu_3);	// Повертаємося до того ж меню де були
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
