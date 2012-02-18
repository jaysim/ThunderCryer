/**
  ******************************************************************************
  * @file      	CGraphicLCD.h
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	17.02.12
  * @brief		a general graphics LCD interface for various controller drivers
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CGRAPHICLCD_H_
#define CGRAPHICLCD_H_
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct {
   const unsigned int glyph_width;           /* glyph width in pixels              */
                                             /*     0 for variable width fonts     */
   const unsigned int glyph_height;          /* glyph height for storage           */
   const unsigned char *glyph_table;         /* font table start address in memory */
   const unsigned char *mapping_table;       /* used to find the index of a glyph  */
   const unsigned char *width_table;         /* width table start adress           */
                                             /*        NULL for fixed width fonts  */
   const unsigned int *offset_table;         /* ks the offsets of the first byte   */
                                             /*                      of each glyph */
} tFont;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */

class CGraphicLCD {
private:
	/**
	  * @brief  inverted display
	  */
	bool bInverse;

public:

	/**
	  * @brief  Clear Display
	  * @param  None
	  * @retval None
	  */
	virtual void Clear();

	/**
	  * @brief  draw a line from (x1,y1) to (x2,y2)
	  * @param  x1,y1,x2,y2
	  * @retval None
	  */
	virtual void Line(unsigned int x1, unsigned int y1,unsigned int x2, unsigned int y2);

	/**
	  * @brief  draw a window with upper left corner at (x,y) and specified width and height
	  * @param  x1,y1,width,height
	  * @retval None
	  */
	virtual void Window(unsigned int x,unsigned int y,unsigned int width,unsigned int height);

	/**
	  * @brief  draw an rectangle with upper left corner at (x,y) and specified width and height, edges round or square
	  * @param  x1,y1,width,heigth, edge type enum, true for filled rect
	  * @retval None
	  */
	virtual void Rectangle(unsigned int x,unsigned int y,unsigned int width,unsigned int height, bool fill);

	/**
	  * @brief  clear sector from upper left (x,y) with specified width and height
	  * @param  x1,y1,width, height
	  * @retval None
	  */
	virtual void SectorClear(unsigned int x,unsigned int y,unsigned int width,unsigned int height);

	/**
	  * @brief  draw a circle with center at (cx,cy) and specified radius
	  * @param  x1,y1,width,
	  * @retval None
	  */
	virtual void Circle(unsigned char cx, unsigned char cy ,unsigned char radius);

	/**
	  * @brief  writes a string starting at upper left corner on (x,y) and specified font
	  * @param  null terminated string, font structure, x, y
	  * @retval None
	  */
	virtual void WriteString(char * str, const tFont font, unsigned int x, unsigned int y);

	/**
	  * @brief  writes a bitmap to graphics ram
	  * @param  coordinates, width and height
	  * @retval None
	  */
	virtual void Bitmap(unsigned char * bitmap, unsigned char x, unsigned char y, unsigned char width, unsigned char height);

	/**
	  * @brief  initializes Display controller
	  * @param  None
	  * @retval None
	  */
	virtual void Init();

	/**
	  * @brief gives inverted draw status
	  * @param  None
	  * @retval true on inverse mode
	  */
	bool Inverse();

	/**
	  * @brief  sets inverted draw mode
	  * @param  true for inverted draw mode
	  * @retval None
	  */
	void Inverse(bool inverse);

	CGraphicLCD();
	CGraphicLCD(bool inverse);
	virtual ~CGraphicLCD();
};

#endif /* CGRAPHICLCD_H_ */



