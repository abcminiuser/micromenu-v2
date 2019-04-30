//------------------------------------------------------
/* File:       Library for HD44780 compatible displays  */
/* Version:	   v2.01  						 			*/
/* Author:     GrAnd/www.MakeSystem.net		 			*/
/* Tested on:  AVR, STM32F10X		 	 	 		 	*/
/* License:	   GNU LGPLv2.1		 		 	 			*/
//------------------------------------------------------
/* Copyright (C)2014 GrAnd. All right reserved 			*/
//------------------------------------------------------

/*
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Contact information :
						mail@makesystem.net
						http://makesystem.net/?page_id=2
*/

#include "hd44780.h"

/*!	\brief	Macro-definitions. */
#define BIT(n)						(1u << (n))
#define SET(x,n)           			((x) |= BIT(n))
#define CLR(x,n)  		  			((x) &= ~BIT(n))
#define GET(x,n)   		  			(((x) & BIT(n)) ? 1u : 0u)

#define LCD_D7_MASK					GPIO_PIN_7 //0x80u
#define LCD_D6_MASK					GPIO_PIN_6 //0x40u
#define LCD_D5_MASK					GPIO_PIN_5 //0x20u
#define LCD_D4_MASK					GPIO_PIN_4 //0x10u
#define LCD_D3_MASK					GPIO_PIN_3 //0x08u
#define LCD_D2_MASK					GPIO_PIN_2 //0x04u
#define LCD_D1_MASK					GPIO_PIN_1 //0x02u
#define LCD_D0_MASK					GPIO_PIN_0 //0x01u

#define ENABLE_CYCLE_TIME			1u	/* Minimal value ~ 1us */
#define AC_UPDATE_TIME				1u	/* Minimal value ~ 4us */

#if (USE_PROGRESS_BAR)
/*!	\brief	Progress bar definitions. */
#define CGROM_PROGRESS_BAR_SIZE		6u
#define FULL_LOAD					5u
#define EMPTY_LOAD					0u
#define EMPTY_ROW					0x00u /* xxx00000 */

static const uint8_t progress_bar[CGROM_PROGRESS_BAR_SIZE] = {0x00u,0x10u,0x18u,0x1Cu,0x1Eu,0x1Fu};
static uint8_t current_bar_pixel;
static uint8_t current_cell_load;

static void lcdInitBar(void);
#endif

/*!	\brief	Low-level functions. */
static void lcdConfig(uint8_t param);
static void lcdWrite(uint8_t data);
static void lcdStrobe(void);
static void lcdHigh(uint8_t data);
static void lcdLow(uint8_t data);
static void lcd10usDelay(volatile uint32_t us);
static uint32_t lcdPow10(uint8_t n);

#if (USE_BUSY_FLAG)
static void lcd_busy_delay(void);

/*!	\brief	*/
static void lcd_busy_delay(void)
{
	uint8_t BusyFlag;

	Set_D7_as_Input(); /* Set D7 as input. */
	/* When RS = 0 and R/W = 1, the busy flag is output to DB7. */
	CLR(LCD_RS_OUT, LCD_RS);
	SET(LCD_RW_OUT, LCD_RW);

	do
	{/* Note: two cycles are needed for the busy flag check. */
		/* Read busy flag. */
		lcdStrobe();
		/* D7 is used as busy flag. */
		BusyFlag = GET(LCD_D7_IN, LCD_D7);
		/* Discard D3. */
		lcdStrobe();
		/* Verify the busy flag */
	}while (BusyFlag);

	CLR(LCD_RW_OUT, LCD_RW);
	Set_D7_as_Outut(); /* Restore D7 as the output. */
}
#endif /* USE_BUSY_FLAG */

/*!	\brief	Creates delay multiples of 10us. */
static void lcd10usDelay(volatile uint32_t us)
{
	/* Сonversion to us */
	us *= MCU_FREQ_VALUE;
	/* Wait */
	while (us > 0u)
	{
		us--;
	}
}

/*!	\brief	Initiate the transfer of data/commands to LCD. */
static void lcdStrobe(void)
{/* Low level function. */
	SET(LCD_E_OUT, LCD_E);
	lcd10usDelay(ENABLE_CYCLE_TIME);
	CLR(LCD_E_OUT, LCD_E);	/* Enable strobe */
}

/*!	\brief	Send the msb nibble of the data / command to LCD. */
static void lcdHigh(uint8_t data)
{/* Low level function. */
	if(data & LCD_D7_MASK) SET(LCD_D7_OUT, LCD_D7); else CLR(LCD_D7_OUT, LCD_D7);
	if(data & LCD_D6_MASK) SET(LCD_D6_OUT, LCD_D6); else CLR(LCD_D6_OUT, LCD_D6);
	if(data & LCD_D5_MASK) SET(LCD_D5_OUT, LCD_D5); else CLR(LCD_D5_OUT, LCD_D5);
	if(data & LCD_D4_MASK) SET(LCD_D4_OUT, LCD_D4); else CLR(LCD_D4_OUT, LCD_D4);
}

/*!	\brief	Send the lsb nibble of the data / command to LCD. */
static void lcdLow(uint8_t data)
{/* Low level function. */
	if(data & LCD_D3_MASK) SET(LCD_D7_OUT, LCD_D7); else CLR(LCD_D7_OUT, LCD_D7);
	if(data & LCD_D2_MASK) SET(LCD_D6_OUT, LCD_D6); else CLR(LCD_D6_OUT, LCD_D6);
	if(data & LCD_D1_MASK) SET(LCD_D5_OUT, LCD_D5); else CLR(LCD_D5_OUT, LCD_D5);
	if(data & LCD_D0_MASK) SET(LCD_D4_OUT, LCD_D4); else CLR(LCD_D4_OUT, LCD_D4);
}

/*!	\brief	Send data/commands to the display. */
static void lcdWrite(uint8_t data)
{/* Low level function. */
#if (USE_BUSY_FLAG)
	/* Write data/commands to LCD. */
	CLR(LCD_RW_OUT, LCD_RW);
#endif /* USE_BUSY_FLAG */

	lcdHigh(data);
	lcdStrobe();
	lcdLow(data);
	lcdStrobe();

	/* The busy flag must be checked after the 4-bit data has been transferred twice. */
#if (USE_BUSY_FLAG)
	lcd_busy_delay();
#else
	lcd10usDelay(BUSY_CYCLE_TIME);
#endif /* USE_BUSY_FLAG */
}

/*!	\brief	Initializing by instruction. 4-bit interface initialization. */
static void lcdConfig(uint8_t param)
{/* Low level function. */
	/* Send commands to LCD. */
	CLR(LCD_RS_OUT, LCD_RS);
#if (USE_BUSY_FLAG)
	/* Write data/commands to LCD. */
	CLR(LCD_RW_OUT, LCD_RW);
#endif /* USE_BUSY_FLAG */

	lcdHigh(param);
	lcdStrobe();		// Change 8-bit interface to 4-bit interface
	lcd10usDelay(BUSY_CYCLE_TIME);
	lcdStrobe();		/* DB7 to DB4 of the "Function set" instruction is written twice. */
	lcd10usDelay(BUSY_CYCLE_TIME);
	lcdLow(param);
	lcdStrobe();		// 4-bit, two lines, 5x8 pixel
	lcd10usDelay(BUSY_CYCLE_TIME);
	/* Note: The number of display lines and character font cannot be changed after this point. */
}

							//-----------------------------//
							/*         LCDlib API          */
							//-----------------------------//

/*!	\details	Clear display writes space code 20H into all DDRAM addresses.
 * 				It then sets DDRAM address 0 into the address counter,
 * 				and returns the display to its original status if it was shifted.
 * 				In other words, the display disappears and the cursor
 * 				or blinking goes to the left edge of the display (in the first line if 2 lines are displayed).
 * 				It also sets I/D to 1 (increment mode) in entry mode (S of entry mode does not change). */
void lcdClrScr(void)
{
	/* Send a command to LCD. */
	CLR(LCD_RS_OUT, LCD_RS);
	/* Clear screen */
	lcdWrite(0x01u);
	/* Busy delay */
#if (USE_BUSY_FLAG)
	lcd_busy_delay();
#else
	lcd10usDelay(CLRSCR_CYCLE_TIME);
#endif /* USE_BUSY_FLAG */
}

/*!	\details	"Return home" sets DDRAM address 0 into the address counter,
 * 				and returns the display to its original status if it was shifted.
 * 				The DDRAM contents do not change.
 * 				The cursor or blinking go to the left edge of the display
 * 				(in the first line if 2 lines are displayed). */
void lcdReturn(void)
{
	/* Send a command to LCD. */
	CLR(LCD_RS_OUT, LCD_RS);
	/* Return home */
	lcdWrite(0x02u);
	/* Busy delay */
#if (USE_BUSY_FLAG)
	lcd_busy_delay();
#else
	lcd10usDelay(RETHOME_CYCLE_TIME);
#endif /* USE_BUSY_FLAG */
}

/*!	\details	lcdScroll shifts the display to the right or left without writing or reading display data.
 * 				This function is used to correct or search the display.
 *	\note		The first and second line displays will shift at the same time. */
void lcdScroll(uint8_t direction)
{
	/* Send a command to LCD. */
	CLR(LCD_RS_OUT, LCD_RS);
	/* Scroll display */
	switch (direction)
	{
	/* To left */
		case LEFT  :
			lcdWrite(0x18u);
			break;

		/* To right */
		case RIGHT :
			lcdWrite(0x1Cu);
			break;

		default:
			/* Ignore this command */
			break;
	}
}

/*!	\details	"Cursor shift" shifts the cursor position to the right or left,
 * 				without writing or reading display data.
 * 				This function is used to correct or search the display.
 * 				In a 2-line display, the cursor moves to the second line
 * 				when it passes the 40th digit of the first line. */
void cursorShift(uint8_t direction)
{
	/* Send a command to LCD. */
	CLR(LCD_RS_OUT, LCD_RS);
	/* Shift cursor */
	switch (direction)
	{
		/* To left */
		case LEFT  :
			lcdWrite(0x10u);
			break;

		/* To right */
		case RIGHT :
			lcdWrite(0x14u);
			break;

		default:
			/* Ignore this command */
			break;
	}
}

/*!	\details	Go to the specified (DDRAM/CGRAM) memory address.*/
void lcdGoto(uint8_t line, uint8_t address)
{
	/* Send a command to LCD. */
	CLR(LCD_RS_OUT, LCD_RS);
	/* Set DDRAM/CGRAM address. */
	switch (line)
	{
		/* Set DDRAM address. */
		case LCD_1st_LINE: lcdWrite(0x80u | START_ADDRESS_1st_LINE | address); break;
		case LCD_2nd_LINE: lcdWrite(0x80u | START_ADDRESS_2nd_LINE | address); break;
		case LCD_3rd_LINE: lcdWrite(0x80u | START_ADDRESS_3rd_LINE | address); break;
		case LCD_4th_LINE: lcdWrite(0x80u | START_ADDRESS_4th_LINE | address); break;
		/* Set CGRAM address. */
		case CGRAM : lcdWrite(0x40u | address); break;

		default:
			/* Ignore this command */
			break;
	}
}

/*!	\details	Change LCD settings. */
void lcdSetMode(uint8_t param)
{
	/* Send a command to LCD. */
	CLR(LCD_RS_OUT, LCD_RS);
	lcdWrite(param);
}

/*!	\details	Write a single char to the current memory space (DDRAM/CGRAM). */
void lcdPutc(uint8_t data)
{
	/* Send data to LCD. */
	SET(LCD_RS_OUT, LCD_RS);
	lcdWrite(data);
	/* Note:
	 * After execution of the CGRAM/DDRAM data write/read instruction, the RAM address counter is incremented
	 * or decremented by 1. The RAM address counter is updated after the busy flag turns off.
	 * tADD is the time elapsed after the busy flag turns off until the address counter is updated. */
	lcd10usDelay(AC_UPDATE_TIME);	/* Update RAM address counter delay. */
}

/*!	\details	Writes ANSI-C string to LCD (DDRAM memory space). */
//void lcdPuts(const uint8_t *str)
void lcdPuts(const char *str)
{
	/* Send a ANSI-C string to LCD. */
	while ('\0' != *str)
	{
#if ( USE_FORMATTED_OUTPUT )
		if(('\n' == *str))
		{/*New line */
			lcdGoto(LCD_2nd_LINE, 0u);
		}
		else if(('\r' == *str))
		{/* Return home */
			lcdReturn();
		}
		else if(('\t' == *str))
		{/* Tab space */
			uint8_t i;

			for(i=0u; i<TAB_SPACE; i++)
			{/* Shift cursor to the right. */
				cursorShift(RIGHT);
			}
		}
		else
#endif
		{
			/* Display a symbol. */
			lcdPutc(*str);
		}
		/* Get the next symbol. */
		str++;
	}
}

/*!	\details	Load the user-defined symbol into the CGRAM memory. */
void lcdLoadChar(uint8_t* vector, uint8_t position)
{
	uint8_t i;
	/* Go to the CGRAM memory space: 0 to 7 */
	lcdGoto(CGRAM, (position * FONT_HEIGHT));

	for(i = 0u; i < FONT_HEIGHT; i++)
	{/* Load one row of pixels into the CGRAM register. */
		lcdPutc(vector[i]);
	}

	/* Return to the DDRAM memory space. */
	lcdGoto(LCD_1st_LINE, 0u);
}

/*!	\details	Load and display the user-defined symbol. */
void lcdDrawChar( uint8_t* vector,
			   	  uint8_t position,
			   	  uint8_t line,
			   	  uint8_t address )
{
	/* Load the user-defined symbol into the CGRAM memory. */
	lcdLoadChar(vector, position);
	/* Select LCD position. */
	lcdGoto(line, address);
	/* Display the user-defined symbol. */
	lcdPutc(position);
}

/*!	\details	Erase a symbol from the left of the cursor. */
void lcdBackSpace(void)
{
	cursorShift(LEFT);		// Рухаємо курсор на одну позицію вліво
	lcdPutc(' ');			// Очищаємо, після чого відбувається автоінкремент вправо
	cursorShift(LEFT);		// Рухаємо курсор на одну позицію вліво
}

/*!	\brief	Returns 10^n value. */
static uint32_t lcdPow10(uint8_t n)
{
	uint32_t retval = 1u;

	while (n > 0u)
	{
		retval *= 10u;
		n--;
	}

	return retval;
}

/*!	\brief	Display a integer number: +/- 2147483647. */
void lcdItos(int32_t value)
{
	int32_t i;

	if (value < 0)
	{
		lcdPutc('-');
		value = -value;
	}

	i = 1;
	while ((value / i) > 9)
	{
		i *= 10;
	}

	lcdPutc(value/i + '0');	/* Display at least one symbol */
	i /= 10;

	while (i > 0)
	{
		lcdPutc('0' + ((value % (i*10)) / i));
		i /= 10;
	}
}

/*!	\brief	Display a floating point number. */
void lcdFtos(float value, uint8_t n)
{
	if (value < 0.0)
	{
		lcdPutc('-');
		value = -value;
	}

	lcdItos((int32_t)value); // Вивід цілої частини

	if (n > 0u)
	{
		lcdPutc('.'); // Крапка

		lcdNtos((uint32_t)(value * (float)lcdPow10(n)), n); // Вивід дробової частини
	}
}

/*!	\brief	Display "n" right digits of "value". */
void lcdNtos(uint32_t value, uint8_t n)
{
	if (n > 0u)
	{
		uint32_t i = lcdPow10(n - 1u);

		while (i > 0u)	/* Display at least one symbol */
		{
			lcdPutc('0' + ((value/i) % 10u));

			i /= 10u;
		}
	}
}

#if ( USE_PROGRESS_BAR )
/*!	\brief	Initialize the progress bar
 * 			(i.e. preload elements of the progress bar into CGRAM and reset all variables). */
static void lcdInitBar(void)
{
	uint8_t i, j;

	for (i = 0u; i < CGROM_PROGRESS_BAR_SIZE; i++)
	{
		lcdGoto(CGRAM, (i * FONT_HEIGHT));

		for (j = 0u; j < FONT_HEIGHT; j++)
		{
			if (j < PROGRESS_BAR_HEIGHT)
			{
				lcdPutc(progress_bar[i]);
			}
			else
			{/* Load an empty row of pixels in CGRAM. */
				lcdPutc(EMPTY_ROW);
			}
		}
	}

	/* Clear the entire bar and initialize all variables. */
	lcdClrBar();
}

/*!	\brief	Draw progress bar. */
void lcdDrawBar(uint8_t next_bar_pixel)
{
	/* Go to the current cell position in the progress bar. */
	lcdGoto(PROGRESS_BAR_LINE, (current_bar_pixel / FONT_WIDTH));

	if (next_bar_pixel > current_bar_pixel)
	{
		/* Increment LCD cursor */
		lcdSetMode(ENTRY_MODE_INC_NO_SHIFT);

		/* Prevent the progress bar overflow */
		if (next_bar_pixel > PROGRESS_BAR_MAX_LOAD)
		{
			next_bar_pixel = PROGRESS_BAR_MAX_LOAD;
		}

		while (current_bar_pixel != next_bar_pixel)
		{
			/* Go to the next pixel. */
			current_bar_pixel++;
			current_cell_load++;
			/* Display the load of the current cell. */
			lcdPutc(current_cell_load);

			if (current_cell_load < FULL_LOAD)
			{/* Return the cursor to the current cell. */
				cursorShift(LEFT);
			}
			else
			{/* Go to the next cell. */
				current_cell_load = EMPTY_LOAD;
			}
		}
	 }
#if (USE_REGRESS_BAR)
	 else if (next_bar_pixel < current_bar_pixel)
	 {
		 /* Decrement LCD cursor */
		lcdSetMode(ENTRY_MODE_DEC_NO_SHIFT);

		do
		{
			if (EMPTY_LOAD == current_cell_load)
			{/* Go to the next cell. */
				cursorShift(LEFT);
				current_cell_load = FULL_LOAD;
			}
			/* Go to the next pixel. */
			current_bar_pixel--;
			current_cell_load--;
			/* Display the load of the current cell. */
			lcdPutc(current_cell_load);
			/* Return the cursor to the current cell. */
			cursorShift(RIGHT);
		}
		while (current_bar_pixel != next_bar_pixel);
	 }
#endif /* USE_REGRESS_BAR */
	 else
	 {
		 /* Nothing to do. */
	 }

	/* Restore the default entry mode. */
	lcdSetMode(DEFAULT_ENTRY_MODE);
	/* Return home. */
	lcdGoto(LCD_1st_LINE, 0u);
}

/*!	\brief	Clear the entire progress bar. */
void lcdClrBar(void)
{
	uint8_t i;
	/* Go to the last cell in the progress bar. */
	lcdGoto(PROGRESS_BAR_LINE, (PROGRESS_BAR_WIDTH - 1u));
	/* Set the decrement mode. */
	lcdSetMode(ENTRY_MODE_DEC_NO_SHIFT);

	for(i = 0u; i < PROGRESS_BAR_WIDTH; i++)
	{/* Display the "empty cell" symbol (i.e. clear the LCD cell). */
		lcdPutc(EMPTY_LOAD);
	}

	/* Reset the progress bar variables. */
	current_bar_pixel = 0u;
	current_cell_load = EMPTY_LOAD;

	/* Restore the default entry mode. */
	lcdSetMode(DEFAULT_ENTRY_MODE);
	/* Return home. */
	lcdGoto(LCD_1st_LINE, 0u);
}
#endif

/*!	\brief	Initialize the LCD.
 * 	\note	This library use the 4-bit interface. */
void lcdInit(void)
{
	/* GPIO initialization. */
	setAllPinsAsOutputs();
	/* LCD initialization. */
	lcdWrite(0x30);
	lcd10usDelay(INIT_CYCLE_TIME);
	lcdWrite(0x30);
	lcd10usDelay(INIT_CYCLE_TIME);
	lcdConfig(DEFAULT_DISPLAY_CONFIG);
	lcdSetMode(DEFAULT_VIEW_MODE);
	lcdSetMode(DEFAULT_ENTRY_MODE);
	lcdClrScr();
	lcdReturn();
	#if (USE_PROGRESS_BAR)
		lcdInitBar();
	#endif
}

void lcdGpioInit(void)
{
	/*
	 GPIO_InitTypeDef GPIO_InitStruct;

	 __HAL_RCC_GPIOC_CLK_ENABLE();

	 GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  	 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
	 */
}

//-------------------------------
/* END OF FILE */
//-------------------------------
