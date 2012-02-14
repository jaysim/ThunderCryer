//-------------------------------------------------------------------------------------------------
// Graphic LCD with Toshiba T6963 controller
// Copyright (c) Rados�aw Kwiecie�, 2007r
// http://en.radzio.dxp.pl/t6963/
// Compiler : avr-gcc
//-------------------------------------------------------------------------------------------------

#include <inttypes.h>
#include "stm32f4xx_gpio.h"

// data port
#define GLCD_DATA_PORT 			GPIOE
#define GLCD_DATA_PORT_MASK 	0xff00
#define GLCD_DATA_MODE_MASK 	0x55550000
#define GLCD_DATA_OFFSET		8
#define GLCD_DATA_INPUT 		GLCD_DATA_PORT->MODER &= ~GLCD_DATA_MODE_MASK;
#define GLCD_DATA_OUTPUT		GLCD_DATA_PORT->MODER |= GLCD_DATA_MODE_MASK;

// control port
#define GLCD_CTRL_PORT_CD_RD	GPIOC
#define GLCD_CTRL_PORT_WR_CE	GPIOB

// control signals
#define GLCD_WR			GPIO_Pin_0
#define GLCD_RD			GPIO_Pin_5
#define GLCD_CE			GPIO_Pin_1
#define GLCD_CD			GPIO_Pin_4
#define GLCD_RESET		GPIO_Pin_11

// display properties
#define GLCD_NUMBER_OF_LINES				128
#define GLCD_PIXELS_PER_LINE				240
#define GLCD_FONT_WIDTH						8
//
#define GLCD_GRAPHIC_AREA					(GLCD_PIXELS_PER_LINE / GLCD_FONT_WIDTH)
#define GLCD_TEXT_AREA						(GLCD_PIXELS_PER_LINE / GLCD_FONT_WIDTH)
#define GLCD_GRAPHIC_SIZE					(GLCD_GRAPHIC_AREA * GLCD_NUMBER_OF_LINES + 2)
#define GLCD_TEXT_SIZE						(GLCD_TEXT_AREA * (GLCD_NUMBER_OF_LINES/8) + 2)



#define GLCD_TEXT_HOME						0
#define GLCD_GRAPHIC_HOME					(GLCD_TEXT_HOME + GLCD_TEXT_SIZE + 100)
#define GLCD_OFFSET_REGISTER				2
#define GLCD_EXTERNAL_CG_HOME				(GLCD_OFFSET_REGISTER << 11)


#define T6963_SET_CURSOR_POINTER			0x21
#define T6963_SET_OFFSET_REGISTER			0x22
#define T6963_SET_ADDRESS_POINTER			0x24

#define T6963_SET_TEXT_HOME_ADDRESS			0x40
#define T6963_SET_TEXT_AREA					0x41
#define T6963_SET_GRAPHIC_HOME_ADDRESS		0x42
#define T6963_SET_GRAPHIC_AREA				0x43

#define T6963_MODE_SET						0x80


#define T6963_DISPLAY_MODE					0x90
	#define T6963_CURSOR_BLINK_ON			0x01
	#define T6963_CURSOR_DISPLAY_ON			0x02
	#define T6963_TEXT_DISPLAY_ON			0x04
	#define T6963_GRAPHIC_DISPLAY_ON		0x08				


#define T6963_CURSOR_PATTERN_SELECT			0xA0
	#define T6963_CURSOR_1_LINE				0x00
	#define T6963_CURSOR_2_LINE				0x01
	#define T6963_CURSOR_3_LINE				0x02
	#define T6963_CURSOR_4_LINE				0x03
	#define T6963_CURSOR_5_LINE				0x04
	#define T6963_CURSOR_6_LINE				0x05
	#define T6963_CURSOR_7_LINE				0x06
	#define T6963_CURSOR_8_LINE				0x07

#define T6963_SET_DATA_AUTO_WRITE			0xB0
#define T6963_SET_DATA_AUTO_READ			0xB1
#define T6963_AUTO_RESET					0xB2

#define T6963_DATA_WRITE_AND_INCREMENT		0xC0
#define T6963_DATA_READ_AND_INCREMENT		0xC1
#define T6963_DATA_WRITE_AND_DECREMENT		0xC2
#define T6963_DATA_READ_AND_DECREMENT		0xC3
#define T6963_DATA_WRITE_AND_NONVARIALBE	0xC4
#define T6963_DATA_READ_AND_NONVARIABLE		0xC5

#define T6963_SCREEN_PEEK					0xE0
#define T6963_SCREEN_COPY					0xE8


void GLCD_InitalizeInterface(void);
unsigned char GLCD_CheckStatus(void);
void GLCD_WriteCommand(unsigned char);
void GLCD_WriteData(unsigned char);
unsigned char GLCD_ReadData(void);
void GLCD_ClearText(void);
void GLCD_ClearCG(void);
void GLCD_ClearGraphic(void);
void GLCD_WriteChar(char ch);
void GLCD_WriteString(char * str);
void GLCD_TextGoTo(unsigned char, unsigned char);
void GLCD_DefineCharacter(unsigned char, unsigned char *);
void GLCD_Initalize(void);
void GLCD_SetPixel(unsigned char, unsigned char, unsigned char);
void GLCD_WriteDisplayData(unsigned char);
