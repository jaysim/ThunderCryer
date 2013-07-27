/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    drivers/gdisp/ILI9320/gdisp_lld.c
 * @brief   GDISP Graphics Driver subsystem low level driver source for the ILI9320 display.
 *
 * @addtogroup GDISP
 * @{
 */

#include "gfx.h"



#if GFX_USE_GDISP /*|| defined(__DOXYGEN__)*/

/* Include the emulation code for things we don't support */
#include "gdisp/lld/emulation.c"

#include "gdisp_lld_board_ThunderCryer.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/
#if 0 /* defined in Board header */
/* This controller is only ever used with a 240 x 320 display */
#if defined(GDISP_SCREEN_HEIGHT)
	#warning "GDISP: This low level driver does not support setting a screen size. It is being ignored."
	#undef GDISP_SCREEN_HEIGHT
#endif
#if defined(GDISP_SCREEN_WIDTH)
	#warning "GDISP: This low level driver does not support setting a screen size. It is being ignored."
	#undef GDISP_SCREEN_WIDTH
#endif

#define GDISP_SCREEN_WIDTH		240
#define GDISP_SCREEN_HEIGHT		128

#define GDISP_INITIAL_CONTRAST	50
#define GDISP_INITIAL_BACKLIGHT	50
#endif
/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
/**
  * @brief  polled timer supported us delay
  * @param  us micro seconds to wait
  */
static inline void lld_lcdDelay(uint16_t us) {
  gptPolledDelay(&GPTD14, (us) * 10));
}

/**
  * @brief  reads T6963C status byte
  * @param  None
  * @retval true for display ready
  */
bool gdisp_lld_check_status(void)
{

    uint16_t tmp;
    // set data pins as input
    GLCD_DATA_INPUT;


    palClearPort(GLCD_CTRL_PORT, GLCD_RD | GLCD_CE);

    US_DELAY(c_iDelayFore);

    tmp = ((palReadPort(GLCD_DATA_PORT)(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

    palSetPort(GLCD_CTRL_PORT,  GLCD_RD | GLCD_CE);

    US_DELAY(c_iDelayAfter);

    GLCD_DATA_OUTPUT;

    return ((tmp&0x03)==0x03);
}

static inline void gdisp_lld_init_board(void) {
  // init us delay timer
  gptStart(&GPTD14,&gptConf14);
  pwmEnableChannel(&PWMD9, 0, 0);
}


/**
  * @brief  Reset the Display
  * @param  None
  * @retval None
  */
static inline void gdisp_lld_reset() {
  palClearPort(GLCD_CTRL_PORT, GLCD_RESET | GLCD_CE );

  chThdSleepMilliseconds(100);

  palSetPort(GLCD_CTRL_PORT, GLCD_RESET | GLCD_CE );
}

/**
  * @brief  writes a command
  * @param  command
  * @retval None
  */
static inline void gdisp_lld_write_command(uint16_t cmd) {
  while(!gdisp_lld_check_status());

  palClearPort(GLCD_DATA_PORT,GLCD_DATA_PORT_MASK);
  GLCD_DATA_PORT->ODR |= (command << (GLCD_DATA_OFFSET)); // lowbyte will stay as is in this write

  palClearPort(GLCD_CTRL_PORT, GLCD_WR | GLCD_CE);

  US_DELAY(c_iDelayFore);   // time for display to read the data

  palSetPort(GLCD_CTRL_PORT , GLCD_WR | GLCD_CE);


  US_DELAY(c_iDelayAfter);
}

/**
  * @brief  writes data
  * @param  data
  * @retval None
  */
static inline void gdisp_lld_write_data(uint16_t data) {
  while(!gdisp_lld_check_status());

  palClearPort(GLCD_DATA_PORT,GLCD_DATA_PORT_MASK);
  GLCD_DATA_PORT->ODR |= ((uint16_t)data << GLCD_DATA_OFFSET); // lowbyte will stay as is in this write

  palClearPort(GLCD_CTRL_PORT, GLCD_CD | GLCD_WR | GLCD_CE);

  US_DELAY(c_iDelayFore);

  palSetPort(GLCD_CTRL_PORT, GLCD_CD | GLCD_WR | GLCD_CE);

  US_DELAY(c_iDelayAfter);
}

/**
  * @brief  writes data with increment of the address pointer
  * @param  data
  * @retval None
  */
static inline void gdisp_lld_write_data_inc(uint16_t data) {
  gdisp_lld_write_data(x);
  gdisp_lld_write_command(T6963_DATA_WRITE_AND_INCREMENT);
}

/**
  * @brief  reads data from display
  * @param  None
  * @retval data from display
  */
static inline uint16_t gdisp_lld_read_data(void) {
  uint16_t tmp;
  while(!gdisp_lld_check_status());

  GLCD_DATA_INPUT;

  palClearPort(GLCD_CTRL_PORT, GLCD_RD | GLCD_CD | GLCD_CE);

  US_DELAY(c_iDelayFore);

  tmp = ((palReadPort(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

  palSetPort(GLCD_CTRL_PORT, GLCD_RD | GLCD_CD | GLCD_CE );

  US_DELAY(c_iDelayAfter);

  GLCD_DATA_OUTPUT;
  return (unsigned char)tmp;
}

/**
  * @brief  sets backlight
  * @param  percent     Backlight level in percent
  */
static inline void gdisp_lld_backlight(uint8_t percent) {
    pwmEnableChannel(&PWMD9, 0, percent );
}

/**
  * @brief  sets the address pointer to a specific address
  * @param  address     new address for the address pointer
  */
static inline void glcd_lld_set_address_pointer(uint16_t address){
  address += 2; // display offset
  glcd_lld_write_data(address & 0xFF);
  glcd_lld_write_data(address >> 8);
  glcd_lld_write_command(T6963_SET_ADDRESS_POINTER);
}

/**
  * @brief  sets the cursor to a specific coordinate
  * @param  x   new cursor x coordinate
  * @param  y   new cursor y coordinate
  */
static inline void glcd_lld_set_cursor(uint16_t x, uint16_t y){
  uint16_t address;

  address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);
  glcd_lld_set_address_pointer(address);
}



bool_t gdisp_lld_init(void) {
	/* Initialise your display */
	gdisp_lld_init_board();

	glcd_lld_reset();

	// Graphic home address
	glcd_lld_write_data(GLCD_GRAPHIC_HOME & 0xFF);
	glcd_lld_write_data(GLCD_GRAPHIC_HOME >> 8);
	glcd_lld_write_command(T6963_SET_GRAPHIC_HOME_ADDRESS);

	//graphic line length
	glcd_lld_write_data(GLCD_GRAPHIC_AREA);
	glcd_lld_write_data(0x00);
	glcd_lld_write_command(T6963_SET_GRAPHIC_AREA);

	//text home address
	glcd_lld_write_data(GLCD_TEXT_HOME & 0xFF);
	glcd_lld_write_data(GLCD_TEXT_HOME >> 8);
	glcd_lld_write_command(T6963_SET_TEXT_HOME_ADDRESS);

	//text line length
	glcd_lld_write_data(GLCD_TEXT_AREA);
	glcd_lld_write_data(0x00);
	glcd_lld_write_command(T6963_SET_TEXT_AREA);

	//write offset register  (no effect)
	glcd_lld_write_data(GLCD_OFFSET_REGISTER);
	glcd_lld_write_data(0x00);
	glcd_lld_write_command(T6963_SET_OFFSET_REGISTER);

	// display in XOR Mode
	glcd_lld_write_command(T6963_MODE_SET | 1);

	//Graphic and no Text mode
	glcd_lld_write_command(T6963_DISPLAY_MODE | T6963_GRAPHIC_DISPLAY_ON );


	// Turn on the backlight
	gdisp_lld_backlight(GDISP_INITIAL_BACKLIGHT);
	
    /* Initialise the GDISP structure */
    GDISP.Width = GDISP_SCREEN_WIDTH;
    GDISP.Height = GDISP_SCREEN_HEIGHT;
    GDISP.Orientation = GDISP_ROTATE_0;
    GDISP.Powermode = powerOn;
    GDISP.Backlight = GDISP_INITIAL_BACKLIGHT;
    gdisp_lld_backlight(GDISP.Backlight);
    GDISP.Contrast = GDISP_INITIAL_CONTRAST;
    #if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
	GDISP.clipx0 = 0;
	GDISP.clipy0 = 0;
	GDISP.clipx1 = GDISP.Width;
	GDISP.clipy1 = GDISP.Height;
    #endif

	return TRUE;
}

void gdisp_lld_draw_pixel(coord_t x, coord_t y, color_t color) {
    #if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
        if (x < GDISP.clipx0 || y < GDISP.clipy0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
    #endif
    unsigned char tmp;


    glcd_lld_set_cursor(x,y);

    tmp = (GLCD_FONT_WIDTH - 1) - (x % GLCD_FONT_WIDTH);

    if(color)
      glcd_lld_write_command(T6963_BIT_RESET | tmp);
    else
      glcd_lld_write_command(T6963_BIT_SET | tmp);
}

#if GDISP_HARDWARE_CLEARS || defined(__DOXYGEN__)
void gdisp_lld_clear(color_t color) {
  unsigned int i;
  // Graphics and Text are different mem pools in this Controller
  glcd_lld_set_address_pointer(GLCD_GRAPHIC_HOME);

  for(i = 0; i < GLCD_GRAPHIC_SIZE; i++)
  {
    if(color)
      gdisp_lld_write_data_inc(0xFF);
    else
      gdisp_lld_write_data_inc(0x00);
  }
}
#endif

#if GDISP_HARDWARE_FILLS || defined(__DOXYGEN__)
	void gdisp_lld_fill_area(coord_t x, coord_t y, coord_t cx, coord_t cy, color_t color) {
		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < GDISP.clipx0) { cx -= GDISP.clipx0 - x; x = GDISP.clipx0; }
			if (y < GDISP.clipy0) { cy -= GDISP.clipy0 - y; y = GDISP.clipy0; }
			if (cx <= 0 || cy <= 0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
			if (x+cx > GDISP.clipx1)	cx = GDISP.clipx1 - x;
			if (y+cy > GDISP.clipy1)	cy = GDISP.clipy1 - y;
		#endif

		unsigned i, area;

		area = cx*cy;
		lld_lcdSetViewPort(x, y, cx, cy);
		lld_lcdWriteStreamStart();
		for(i = 0; i < area; i++)
			lld_lcdWriteData(color);
		lld_lcdWriteStreamStop();
		lld_lcdResetViewPort();
	}
#endif

#if GDISP_HARDWARE_BITFILLS || defined(__DOXYGEN__)
	void gdisp_lld_blit_area_ex(coord_t x, coord_t y, coord_t cx, coord_t cy, coord_t srcx, coord_t srcy, coord_t srccx, const pixel_t *buffer) {
		coord_t endx, endy;
		unsigned lg;

		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < GDISP.clipx0) { cx -= GDISP.clipx0 - x; srcx += GDISP.clipx0 - x; x = GDISP.clipx0; }
			if (y < GDISP.clipy0) { cy -= GDISP.clipy0 - y; srcy += GDISP.clipy0 - y; y = GDISP.clipy0; }
			if (srcx+cx > srccx)		cx = srccx - srcx;
			if (cx <= 0 || cy <= 0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
			if (x+cx > GDISP.clipx1)	cx = GDISP.clipx1 - x;
			if (y+cy > GDISP.clipy1)	cy = GDISP.clipy1 - y;
		#endif

		lld_lcdSetViewPort(x, y, cx, cy);
		lld_lcdWriteStreamStart();

		endx = srcx + cx;
		endy = y + cy;
		lg = srccx - cx;
		buffer += srcx + srcy * srccx;
		for(; y < endy; y++, buffer += lg)
			for(x=srcx; x < endx; x++)
				lld_lcdWriteData(*buffer++);
		lld_lcdWriteStreamStop();
		lld_lcdResetViewPort();
	}
#endif

#if (GDISP_NEED_PIXELREAD && GDISP_HARDWARE_PIXELREAD) || defined(__DOXYGEN__)
	color_t gdisp_lld_get_pixel_color(coord_t x, coord_t y) {
		color_t color;

		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < 0 || x >= GDISP.Width || y < 0 || y >= GDISP.Height) return 0;
		#endif


		return color;
	}
#endif

#if (GDISP_NEED_SCROLL && GDISP_HARDWARE_SCROLL) || defined(__DOXYGEN__)
	void gdisp_lld_vertical_scroll(coord_t x, coord_t y, coord_t cx, coord_t cy, int lines, color_t bgcolor) {
		static color_t buf[((GDISP_SCREEN_HEIGHT > GDISP_SCREEN_WIDTH ) ? GDISP_SCREEN_HEIGHT : GDISP_SCREEN_WIDTH)];
		coord_t row0, row1;
		unsigned i, gap, abslines;

		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < GDISP.clipx0) { cx -= GDISP.clipx0 - x; x = GDISP.clipx0; }
			if (y < GDISP.clipy0) { cy -= GDISP.clipy0 - y; y = GDISP.clipy0; }
			if (!lines || cx <= 0 || cy <= 0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
			if (x+cx > GDISP.clipx1)	cx = GDISP.clipx1 - x;
			if (y+cy > GDISP.clipy1)	cy = GDISP.clipy1 - y;
		#endif

		abslines = lines < 0 ? -lines : lines;

		if (abslines >= cy) {
			abslines = cy;
			gap = 0;
		} else {
			gap = cy - abslines;
			for(i = 0; i < gap; i++) {
				if(lines > 0) {
					row0 = y + i + lines;
					row1 = y + i;
				} else {
					row0 = (y - i - 1) + lines;
					row1 = (y - i - 1);
				}

				/* read row0 into the buffer and then write at row1*/
				lld_lcdSetViewPort(x, row0, cx, 1);
				lld_lcdReadStreamStart();
				lld_lcdReadStream(buf, cx);
				lld_lcdReadStreamStop();

				lld_lcdSetViewPort(x, row1, cx, 1);
				lld_lcdWriteStreamStart();
				lld_lcdWriteStream(buf, cx);
				lld_lcdWriteStreamStop();
			}
		}

		/* fill the remaining gap */
		lld_lcdSetViewPort(x, lines > 0 ? (y+gap) : y, cx, abslines);
		lld_lcdWriteStreamStart();
		gap = cx*abslines;
		for(i = 0; i < gap; i++) lld_lcdWriteData(bgcolor);
		lld_lcdWriteStreamStop();
		lld_lcdResetViewPort();
	}
#endif

#if (GDISP_NEED_CONTROL && GDISP_HARDWARE_CONTROL) || defined(__DOXYGEN__)
	void gdisp_lld_control(unsigned what, void *value) {
	  switch(what) {
	  case GDISP_CONTROL_POWER:
	    if(GDISP.Powermode == (gdisp_powermode_t)value)
	      return;
	    switch((gdisp_powermode_t)value) {
	    case powerOff:

	      gdisp_lld_backlight(0);
	      break;

	    case powerOn:
	      //*************Power On sequence ******************//

	      gdisp_lld_backlight(GDISP.Backlight);

	      if(GDISP.Powermode != powerSleep || GDISP.Powermode != powerDeepSleep)
	        gdisp_lld_init();
	      else
	        glcd_lld_write_command(T6963_DISPLAY_MODE | T6963_GRAPHIC_DISPLAY_ON);
	      break;

	    case powerSleep:
	      //deactivate graphic display
	      glcd_lld_write_command(T6963_DISPLAY_MODE);
	      gdisp_lld_backlight(0);
	      break;

	    case powerDeepSleep:
	      //deactivate graphic display
	      glcd_lld_write_command(T6963_DISPLAY_MODE);
	      gdisp_lld_backlight(0);
	      break;

	    default:
	      return;
	    }
	    GDISP.Powermode = (gdisp_powermode_t)value;
	    return;


	    case GDISP_CONTROL_BACKLIGHT:
	      if((unsigned)value > 100) value = (void *)100;
	      gdisp_lld_backlight((unsigned)value);
	      GDISP.Backlight = (unsigned)value;
	      break;

	    default:
	      return;
	  }
	}

#endif

#endif /* GFX_USE_GDISP */
/** @} */

