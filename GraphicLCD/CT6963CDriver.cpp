/**
  ******************************************************************************
  * @file      	CT6963CDriver.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	17.02.12
  * @brief		implementation of T6963C Driver
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CT6963CDriver.h"
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
const unsigned char GLCD_NUMBER_OF_LINES = 128;
const unsigned char GLCD_PIXELS_PER_LINE = 240;
const unsigned char GLCD_FONT_WIDTH	= 8;

const unsigned char GLCD_GRAPHIC_AREA	=				(GLCD_PIXELS_PER_LINE / GLCD_FONT_WIDTH);
const unsigned char GLCD_TEXT_AREA		=				(GLCD_PIXELS_PER_LINE / GLCD_FONT_WIDTH);
const unsigned int GLCD_GRAPHIC_SIZE	=				(GLCD_GRAPHIC_AREA * GLCD_NUMBER_OF_LINES);
const unsigned int GLCD_TEXT_SIZE		=				(GLCD_TEXT_AREA * (GLCD_NUMBER_OF_LINES/8));

const unsigned int GLCD_TEXT_HOME			=			0;
const unsigned int GLCD_GRAPHIC_HOME		=			(GLCD_TEXT_HOME + GLCD_TEXT_SIZE);
const unsigned char GLCD_OFFSET_REGISTER	=			2;
const unsigned int  GLCD_EXTERNAL_CG_HOME	=			(GLCD_OFFSET_REGISTER << 11);


const unsigned char T6963_SET_CURSOR_POINTER	=		0x21;
const unsigned char T6963_SET_OFFSET_REGISTER	=		0x22;
const unsigned char T6963_SET_ADDRESS_POINTER	=		0x24;

const unsigned char T6963_SET_TEXT_HOME_ADDRESS		=	0x40;
const unsigned char T6963_SET_TEXT_AREA				=	0x41;
const unsigned char T6963_SET_GRAPHIC_HOME_ADDRESS	=	0x42;
const unsigned char T6963_SET_GRAPHIC_AREA			=	0x43;

const unsigned char T6963_MODE_SET					=	0x80;

const unsigned char T6963_DISPLAY_MODE				=	0x90;
	const unsigned char T6963_CURSOR_BLINK_ON		=	0x01;
	const unsigned char T6963_CURSOR_DISPLAY_ON		=	0x02;
	const unsigned char T6963_TEXT_DISPLAY_ON		=	0x04;
	const unsigned char T6963_GRAPHIC_DISPLAY_ON	=	0x08;


const unsigned char T6963_CURSOR_PATTERN_SELECT		=	0xA0;
	const unsigned char T6963_CURSOR_1_LINE			=	0x00;
	const unsigned char T6963_CURSOR_2_LINE			=	0x01;
	const unsigned char T6963_CURSOR_3_LINE			=	0x02;
	const unsigned char T6963_CURSOR_4_LINE			=	0x03;
	const unsigned char T6963_CURSOR_5_LINE			=	0x04;
	const unsigned char T6963_CURSOR_6_LINE			=	0x05;
	const unsigned char T6963_CURSOR_7_LINE			=	0x06;
	const unsigned char T6963_CURSOR_8_LINE			=	0x07;

const unsigned char T6963_SET_DATA_AUTO_WRITE		=	0xB0;
const unsigned char T6963_SET_DATA_AUTO_READ		=	0xB1;
const unsigned char T6963_AUTO_RESET				=	0xB2;

const unsigned char T6963_DATA_WRITE_AND_INCREMENT	=	0xC0;
const unsigned char T6963_DATA_READ_AND_INCREMENT	=	0xC1;
const unsigned char T6963_DATA_WRITE_AND_DECREMENT	=	0xC2;
const unsigned char T6963_DATA_READ_AND_DECREMENT	=	0xC3;
const unsigned char T6963_DATA_WRITE_AND_NONVARIALBE=	0xC4;
const unsigned char T6963_DATA_READ_AND_NONVARIABLE	=	0xC5;

const unsigned char T6963_SCREEN_PEEK				=	0xE0;
const unsigned char T6963_SCREEN_COPY				=	0xE8;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


CT6963CDriver::CT6963CDriver() {
	// TODO Auto-generated constructor stub

}

CT6963CDriver::~CT6963CDriver() {
	// TODO Auto-generated destructor stub
}

/**
  * @brief  initializes Display controller
  * @param  None
  * @retval None
  */
void CT6963CDriver::Init(){

	HardwareInit();

	Reset();

	// Graphic home address
	WriteData(GLCD_GRAPHIC_HOME & 0xFF);
	WriteData(GLCD_GRAPHIC_HOME >> 8);
	WriteCommand(T6963_SET_GRAPHIC_HOME_ADDRESS);

	//graphic line length
	WriteData(GLCD_GRAPHIC_AREA);
	WriteData(0x00);
	WriteCommand(T6963_SET_GRAPHIC_AREA);

	//text home address
	WriteData(GLCD_TEXT_HOME & 0xFF);
	WriteData(GLCD_TEXT_HOME >> 8);
	WriteCommand(T6963_SET_TEXT_HOME_ADDRESS);

	//text line length
	WriteData(GLCD_TEXT_AREA);
	WriteData(0x00);
	WriteCommand(T6963_SET_TEXT_AREA);

	//write offset register  (no effect)
	WriteData(GLCD_OFFSET_REGISTER);
	WriteData(0x00);
	WriteCommand(T6963_SET_OFFSET_REGISTER);

	// display in XOR Mode
	WriteCommand(T6963_MODE_SET | 1);

	//Graphic and Text mode
	WriteCommand(T6963_DISPLAY_MODE  | T6963_TEXT_DISPLAY_ON | T6963_GRAPHIC_DISPLAY_ON );
}

/**
  * @brief  set address pointer in T6963C
  * @param  address
  * @retval None
  */
void CT6963CDriver::SetAddressPointer(unsigned int address){
	WriteData(address & 0xFF);
	WriteData(address >> 8);
	WriteCommand(T6963_SET_ADDRESS_POINTER);
}

/**
  * @brief  sets address pointer to specific text address
  * @param  column and line
  * @retval None
  */
void CT6963CDriver::TextGoTo(unsigned char x, unsigned char y)
{
	unsigned int address;

	address = GLCD_TEXT_HOME +  x + (GLCD_TEXT_AREA * y);
	SetAddressPointer(address);
}


/**
  * @brief  sets address pointer to specific pixel in graphics ram
  * @param  coordinates of the pixel byte
  * @retval None
  */
void CT6963CDriver::GraphicGoTo(unsigned char x, unsigned char y)
{
	unsigned int address;
	address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);
	SetAddressPointer(address);
}

/**
  * @brief  Clear Display
  * @param  None
  * @retval None
  */
void CT6963CDriver::Clear(){
	unsigned int i;
	// Graphics and Text are different mem pools in this Controller
	SetAddressPointer(GLCD_GRAPHIC_HOME);

	for(i = 0; i < GLCD_GRAPHIC_SIZE; i++)
	{
		WriteDisplayData(0x00);
	}

	SetAddressPointer(GLCD_TEXT_HOME);

	for(i = 0; i < GLCD_TEXT_SIZE; i++)
	{
		WriteDisplayData(0);
	}
}

/**
  * @brief  writes icremental data to display ram
  * @param  data byte
  * @retval None
  */
void CT6963CDriver::WriteDisplayData(unsigned char x)
{
	WriteData(x);
	WriteCommand(T6963_DATA_WRITE_AND_INCREMENT);
}

/**
  * @brief  writes single ASCII char to display
  * @param  ASCII char
  * @retval None
  */
void CT6963CDriver::WriteChar(char charCode)
{
	WriteDisplayData(charCode - 32);
}

/**
  * @brief  writes a string starting at upper left corner on (x,y) and specified font
  * @param  null terminated string, font structure, x, y
  * @retval None
  */
void CT6963CDriver::WriteString(char * str, const tFont font,unsigned int x, unsigned int y)
{
	if(&c_FontNative == &font){
		TextGoTo(x/GLCD_FONT_WIDTH, y/8);

		while(*str)
		{
			WriteChar(*str++);
		}
	} else {

	}

}


/**
  * @brief  sets a pixel in graphics ram
  * @param  coordinates from upper left corner
  * @retval None
  */
void CT6963CDriver::SetPixel(unsigned char x, unsigned char y)
{
	unsigned char tmp;
	GraphicGoTo(x,y);

	tmp = ReadData();

	if(Inverse())
		tmp &=~ (1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
	else
		tmp |= (1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));

	WriteDisplayData(tmp);

}

/**
  * @brief  draw a line from (x1,y1) to (x2,y2)
  * @param  x1,y1,x2,y2
  * @retval None
  */
void CT6963CDriver::Line(unsigned int x1, unsigned int y1,unsigned int x2, unsigned int y2){
	int CurrentX, CurrentY, Xinc, Yinc,
	Dx, Dy, TwoDx, TwoDy,
	TwoDxAccumulatedError, TwoDyAccumulatedError;

	Dx = (x2-x1); // obliczenie sk³adowej poziomej
	Dy = (y2-y1); // obliczenie sk³adowej pionowej

	TwoDx = Dx + Dx; // podwojona sk³adowa pozioma
	TwoDy = Dy + Dy; // podwojona sk³adowa pionowa

	CurrentX = x1; // zaczynamy od X1
	CurrentY = y1; // oraz Y1

	Xinc = 1; // ustalamy krok zwiêkszania pozycji w poziomie
	Yinc = 1; // ustalamy krok zwiêkszania pozycji w pionie

	if(Dx < 0) // jesli sk³adowa pozioma jest ujemna
	{
		Xinc = -1; // to bêdziemy siê "cofaæ" (krok ujemny)
		Dx = -Dx;  // zmieniamy znak sk³adowej na dodatni
		TwoDx = -TwoDx; // jak równie¿ podwojonej sk³adowej
	}

	if (Dy < 0) // jeœli sk³adowa pionowa jest ujemna
	{
		Yinc = -1; // to bêdziemy siê "cofaæ" (krok ujemny)
		Dy = -Dy; // zmieniamy znak sk³adowej na dodatki
		TwoDy = -TwoDy; // jak równiez podwojonej sk³adowej
	}

	SetPixel(x1,y1); // stawiamy pierwszy krok (zapalamy pierwszy piksel)

	if ((Dx != 0) || (Dy != 0)) // sprawdzamy czy linia sk³ada siê z wiêcej ni¿ jednego punktu ;)
	{
		// sprawdzamy czy sk³adowa pionowa jest mniejsza lub równa sk³adowej poziomej
		if (Dy <= Dx) // jeœli tak, to idziemy "po iksach"
		{
			TwoDxAccumulatedError = 0; // zerujemy zmienn¹
			do // ruszamy w drogê
			{
				CurrentX += Xinc; // do aktualnej pozycji dodajemy krok
				TwoDxAccumulatedError += TwoDy; // a tu dodajemy podwojon¹ sk³adow¹ pionow¹
				if(TwoDxAccumulatedError > Dx)  // jeœli TwoDxAccumulatedError jest wiêkszy od Dx
				{
					CurrentY += Yinc; // zwiêkszamy aktualn¹ pozycjê w pionie
					TwoDxAccumulatedError -= TwoDx; // i odejmujemy TwoDx
				}
				SetPixel(CurrentX,CurrentY);// stawiamy nastêpny krok (zapalamy piksel)
			}while (CurrentX != x2); // idziemy tak d³ugo, a¿ osi¹gniemy punkt docelowy
		}
		else // w przeciwnym razie idziemy "po igrekach"
		{
			TwoDyAccumulatedError = 0;
			do
			{
				CurrentY += Yinc;
				TwoDyAccumulatedError += TwoDx;
				if(TwoDyAccumulatedError>Dy)
				{
					CurrentX += Xinc;
					TwoDyAccumulatedError -= TwoDy;
				}
				SetPixel(CurrentX,CurrentY);
			}while (CurrentY != y2);
		}
	}
}

/**
  * @brief  draw a window with upper left corner at (x,y) and specified width and height, edges round or square
  * @param  x1,y1,width,heigth, edge type enum
  * @retval None
  */
void CT6963CDriver::Window(unsigned int x,unsigned int y,unsigned int width,unsigned int height, eRectEdge edge){

}

/**
  * @brief  draw an rectangle with upper left corner at (x,y) and specified width and height
  * @param  x1,y1,width,heigth,  true for filled
  * @retval None
  */
void CT6963CDriver::Rectangle(unsigned int x,unsigned int y,unsigned int width,unsigned int height,  bool fill){
	unsigned char i;
	unsigned char k;

	if(fill){	// fill rect
		//lines
		for (i=0;i<height;i++){
			// columns
			for (k=0;k<width;k++) {
				SetPixel(x+k,y+i);
			}
		}
	} else {
		//lines vertical
		for (i = 0; i < height; i++) {
			SetPixel(x, y + i);
			SetPixel(x + width - 1, y + i);
		}
		//lines horizontal
		for (i = 0; i < width; i++)	{
			SetPixel(x + i, y);
			SetPixel(x + i, y + height - 1);
		}
	}
}


/**
  * @brief  writes a bitmap to graphics ram
  * @param  coordinates, width and heigth
  * @retval None
  */
void CT6963CDriver::Bitmap(unsigned char * bitmap, unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
	unsigned char i, j;

	for(j = 0; j < height; j++)
	{
		GraphicGoTo(x, y + j);
		for(i = 0; i < width/GLCD_FONT_WIDTH; i++)
		{
			if(Inverse()){
				WriteDisplayData(!bitmap[i + (GLCD_GRAPHIC_AREA * j)]);
			} else {
				WriteDisplayData(bitmap[i + (GLCD_GRAPHIC_AREA * j)]);
			}
		}
	}
}

