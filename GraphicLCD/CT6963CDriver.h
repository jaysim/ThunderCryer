/**
  ******************************************************************************
  * @file      	CT6963CDriver.h
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	17.02.12
  * @brief		T6963C LCD Controller Driver
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CT6963CDRIVER_H_
#define CT6963CDRIVER_H_
/* Includes ------------------------------------------------------------------*/
#include "CGraphicLCD.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
const tFont c_FontNative = {0,0,0,0,0,0}; //native font from display controller
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */


class CT6963CDriver: public CGraphicLCD {
private:

	/**
	  * @brief  initialize Hardware to drive the display
	  * @param  None
	  * @retval None
	  */
	virtual void HardwareInit(void);

	/**
	  * @brief  Reset the Display
	  * @param  None
	  * @retval None
	  */
	virtual void Reset(void);

	/**
	  * @brief  writes a command
	  * @param  command
	  * @retval None
	  */
	virtual void WriteCommand(const unsigned char command);

	/**
	  * @brief  writes a data
	  * @param  data
	  * @retval None
	  */
	virtual void WriteData(const unsigned char data);

	/**
	  * @brief  reads data from display
	  * @param  None
	  * @retval data from display
	  */
	virtual unsigned char ReadData() const;

	/**
	  * @brief  set address pointer in T6963C
	  * @param  address
	  * @retval None
	  */
	void SetAddressPointer(unsigned int address);

	/**
	  * @brief  writes incremental data to display ram
	  * @param  data byte
	  * @retval None
	  */
	void WriteDisplayData(unsigned char x);

	/**
	  * @brief  sets address pointer to specific text address
	  * @param  column and line
	  * @retval None
	  */
	void TextGoTo(unsigned char x, unsigned char y);

	/**
	  * @brief  sets a pixel in graphics ram
	  * @param  coordinates from upper left corner
	  * @retval None
	  */
	void SetPixel(unsigned char x, unsigned char y);

	/**
	  * @brief  sets address pointer to specific pixel in graphics ram
	  * @param  coordinates of the pixel byte
	  * @retval None
	  */
	void GraphicGoTo(unsigned char x, unsigned char y);

	/**
	  * @brief  writes single ASCII char to display
	  * @param  ASCII char
	  * @retval None
	  */
	void WriteChar(char charCode);

public:

	/**
	  * @brief  Clear Display
	  * @param  None
	  * @retval None
	  */
	void Clear();

	/**
	  * @brief  draw a line from (x1,y1) to (x2,y2)
	  * @param  x1,y1,x2,y2
	  * @retval None
	  */
	void Line(unsigned int x1, unsigned int y1,unsigned int x2, unsigned int y2);

	/**
	  * @brief  draw a window with upper left corner at (x,y) and specified width and height, edges round or square
	  * @param  x1,y1,width,height, edge type enum
	  * @retval None
	  */
	void Window(unsigned int x,unsigned int y,unsigned int width,unsigned int height, eRectEdge edge);

	/**
	  * @brief  draw an rectangle with upper left corner at (x,y) and specified width and height, edges round or square
	  * @param  x1,y1,width,height, edge type enum, true for filled rect
	  * @retval None
	  */
	void Rectangle(unsigned int x,unsigned int y,unsigned int width,unsigned int height, eRectEdge edge, bool fill);

	/**
	  * @brief  writes a string starting at upper left corner on (x,y) and specified font
	  * @param  null terminated string, font structure, x, y
	  * @retval None
	  */
	void WriteString(char * str, const tFont font,unsigned int x, unsigned int y);

	/**
	  * @brief  writes a bitmap to graphics ram
	  * @param  coordinates, width and heigth
	  * @retval None
	  */
	void Bitmap(unsigned char * bitmap, unsigned char x, unsigned char y, unsigned char width, unsigned char height);

	/**
	  * @brief  initializes Display controller
	  * @param  None
	  * @retval None
	  */
	void Init();



	CT6963CDriver();
	virtual ~CT6963CDriver();
};



#endif /* CT6963CDRIVER_H_ */
