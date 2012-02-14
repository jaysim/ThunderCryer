//-------------------------------------------------------------------------------------------------
// Graphic LCD with Toshiba T6963 controller
// Copyright (c) Radoslaw Kwiecien, 2007r
// http://en.radzio.dxp.pl/t6963/
// Compiler : avr-gcc modified for STM32 StdPeriphDriver
//-------------------------------------------------------------------------------------------------
#include "T6963C.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "FreeRTOS.h"
#include "task.h"

//-------------------------------------------------------------------------------------------------
//
// Delay function for 1µs
//	
//-------------------------------------------------------------------------------------------------
void delay(void)
{
	volatile int i;

	for(i = 0; i < (SystemCoreClock/2000000); i++)
	{
		asm volatile ("nop");
	}
}
//-------------------------------------------------------------------------------------------------
//
// Ports intalization
//
//-------------------------------------------------------------------------------------------------
void GLCD_InitalizeInterface(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// activate Clock for Io Ports used
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
	// all pins as pp outouts with no pull
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

	//Controlpins CD and RD
	GPIO_InitStructure.GPIO_Pin = GLCD_CD | GLCD_RD;
	GPIO_Init(GLCD_CTRL_PORT_CD_RD,&GPIO_InitStructure);

	//Conrolpins WR, CE and RST
	GPIO_InitStructure.GPIO_Pin = GLCD_WR | GLCD_CE | GLCD_RESET;
	GPIO_Init(GLCD_CTRL_PORT_WR_CE,&GPIO_InitStructure);

	// data pins are highbyte
	GPIO_InitStructure.GPIO_Pin = GLCD_DATA_PORT_MASK;
	GPIO_Init(GLCD_DATA_PORT,&GPIO_InitStructure);

	//Set all Controlpins to high level
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD | GLCD_RD);
	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR | GLCD_CE );
}
//-------------------------------------------------------------------------------------------------
//
// Reads dispay status
//
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_CheckStatus(void)
{
	uint16_t tmp;
	GLCD_DATA_INPUT;

	GPIO_ResetBits(GLCD_CTRL_PORT_CD_RD , GLCD_RD);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	delay();

	tmp = ((GPIO_ReadInputData(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_RD);

	GLCD_DATA_OUTPUT;

	delay();



	return (unsigned char)tmp;
}
//-------------------------------------------------------------------------------------------------
//
// Writes instruction 
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteCommand(unsigned char command)
{
	while(!(GLCD_CheckStatus()&0x03));

	GLCD_DATA_PORT->ODR &= ~GLCD_DATA_PORT_MASK;  //Clear Data pins
	GLCD_DATA_PORT->ODR |= (command << (GLCD_DATA_OFFSET)); // lowbyte will stay as is in this write

	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	delay();			// time for display to read the data

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);
	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	delay();

}

//-------------------------------------------------------------------------------------------------
//
// Writes data
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteData(unsigned char data)
{
	while(!(GLCD_CheckStatus()&0x03));

	GLCD_DATA_PORT->ODR &= ~GLCD_DATA_PORT_MASK;  //Clear Data pins
	GLCD_DATA_PORT->ODR |= ((uint16_t)data << GLCD_DATA_OFFSET); // lowbyte will stay as is in this write

	GPIO_ResetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	delay();

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD);
	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);

	delay();
}
//-------------------------------------------------------------------------------------------------
//
// Reads data
//
//-------------------------------------------------------------------------------------------------
unsigned char GLCD_ReadData(void)
{
	uint16_t tmp;
	while(!(GLCD_CheckStatus()&0x03));

	GLCD_DATA_INPUT;

	GPIO_ResetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD | GLCD_RD);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE );

	delay();

	tmp = ((GPIO_ReadInputData(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE );
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD | GLCD_RD);

	delay();

	GLCD_DATA_OUTPUT;
	return (unsigned char)tmp;
}
//-------------------------------------------------------------------------------------------------
//
// Sets address pointer for display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetAddressPointer(unsigned int address)
{
	GLCD_WriteData((address+2) & 0xFF);
	GLCD_WriteData((address+2) >> 8);
	GLCD_WriteCommand(T6963_SET_ADDRESS_POINTER);
}
//-------------------------------------------------------------------------------------------------
//
// Clears text area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearText(void)
{
	int i;
	GLCD_SetAddressPointer(GLCD_TEXT_HOME);

	for(i = 0; i < GLCD_TEXT_SIZE; i++)
	  {
	  GLCD_WriteDisplayData(0);
	  }
}
//-------------------------------------------------------------------------------------------------
//
// Clears characters generator area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearCG(void)
{
	unsigned int i;
	GLCD_SetAddressPointer(GLCD_EXTERNAL_CG_HOME);

	for(i = 0; i < 256 * 8; i++)
	  {
	  GLCD_WriteDisplayData(0);
	  }
}
//-------------------------------------------------------------------------------------------------
//
// Clears graphics area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearGraphic(void)
{
	int i;
	GLCD_SetAddressPointer(GLCD_GRAPHIC_HOME);
	for(i = 0; i < GLCD_GRAPHIC_SIZE; i++)
	  {
	  GLCD_WriteDisplayData(0x00);
	  }
}
//-------------------------------------------------------------------------------------------------
//
// Writes a single character (ASCII code) to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar(char charCode)
{
	GLCD_WriteDisplayData(charCode - 32);
}
//-------------------------------------------------------------------------------------------------
//
// Writes null-terminated string to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString(char * string)
{
	while(*string)
	  {
	  GLCD_WriteChar(*string++);
	  }
}
//-------------------------------------------------------------------------------------------------
//
// Sets display coordinates
//
//-------------------------------------------------------------------------------------------------
void GLCD_TextGoTo(unsigned char x, unsigned char y)
{
	unsigned int address;

	address = GLCD_TEXT_HOME +  x + (GLCD_TEXT_AREA * y);

	GLCD_SetAddressPointer(address);
}
//-------------------------------------------------------------------------------------------------
//
// Writes single char pattern to character generator area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_DefineCharacter(unsigned char charCode, unsigned char * defChar)
{
	unsigned int address;
	unsigned char i;

	address = GLCD_EXTERNAL_CG_HOME + (8 * charCode);

	GLCD_SetAddressPointer(address);

	for(i = 0; i < 8 ; i++)
	  {
	  GLCD_WriteDisplayData(*(defChar + i));
	  }
}

//-------------------------------------------------------------------------------------------------
//
// Set (if color==1) or clear (if color==0) pixel on screen
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
	unsigned char tmp;
	unsigned int address;

	address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);

	GLCD_SetAddressPointer(address);

	GLCD_WriteCommand(T6963_DATA_READ_AND_NONVARIABLE);
	tmp = GLCD_ReadData();

	if(color)
	  tmp |= (1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
	else
	 tmp &= ~(1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));

	GLCD_WriteDisplayData(tmp);

}
//-------------------------------------------------------------------------------------------------
//
// Writes display data and increment address pointer
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteDisplayData(unsigned char x)
{
	GLCD_WriteData(x);
	GLCD_WriteCommand(T6963_DATA_WRITE_AND_INCREMENT);
}
//-------------------------------------------------------------------------------------------------
//
// Sets graphics coordinates
//
//-------------------------------------------------------------------------------------------------
void GLCD_GraphicGoTo(unsigned char x, unsigned char y)
{
	unsigned int address;
	address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);
	GLCD_SetAddressPointer(address);
}
//-------------------------------------------------------------------------------------------------
//
// Displays bitmap from program memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_Bitmap(unsigned char * bitmap, unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
	unsigned char i, j;

	for(j = 0; j < height; j++)
	{
	GLCD_GraphicGoTo(x, y + j);
	for(i = 0; i < width/GLCD_FONT_WIDTH; i++)
	  {
	  GLCD_WriteDisplayData(bitmap[i + (GLCD_GRAPHIC_AREA * j)]);
	  }
	}
}
//-------------------------------------------------------------------------------------------------
//
// Display initalization
//
//-------------------------------------------------------------------------------------------------
void GLCD_Initalize(void)
{
	GLCD_InitalizeInterface();

	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_RESET | GLCD_CE );

	vTaskDelay(5);  // sleep 5 ms

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_RESET | GLCD_CE );

	GLCD_WriteData(GLCD_GRAPHIC_HOME & 0xFF);
	GLCD_WriteData(GLCD_GRAPHIC_HOME >> 8);
	GLCD_WriteCommand(T6963_SET_GRAPHIC_HOME_ADDRESS);

	GLCD_WriteData(GLCD_GRAPHIC_AREA);
	GLCD_WriteData(0x00);
	GLCD_WriteCommand(T6963_SET_GRAPHIC_AREA);

	GLCD_WriteData(GLCD_TEXT_HOME & 0xFF);
	GLCD_WriteData(GLCD_TEXT_HOME >> 8);
	GLCD_WriteCommand(T6963_SET_TEXT_HOME_ADDRESS);

	GLCD_WriteData(GLCD_TEXT_AREA);
	GLCD_WriteData(0x00);
	GLCD_WriteCommand(T6963_SET_TEXT_AREA);

	//GLCD_WriteData(GLCD_OFFSET_REGISTER);
	//GLCD_WriteData(0x00);
	//GLCD_WriteCommand(T6963_SET_OFFSET_REGISTER);

	GLCD_WriteCommand(T6963_MODE_SET | 0);

	GLCD_WriteCommand(T6963_DISPLAY_MODE  | T6963_TEXT_DISPLAY_ON | T6963_GRAPHIC_DISPLAY_ON );



}
