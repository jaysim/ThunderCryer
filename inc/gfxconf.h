/**
 * This file has a different license to the rest of the GFX system.
 * You can copy, modify and distribute this file as you see fit.
 * You do not need to publish your source modifications to this file.
 * The only thing you are not permitted to do is to relicense it
 * under a different license.
 */

/**
 * Copy this file into your project directory and rename it as gfxconf.h
 * Edit your copy to turn on the GFX features you want to use.
 */

#ifndef _GFXCONF_H
#define _GFXCONF_H

/* The operating system to use - one of these must be defined */
#define GFX_USE_OS_CHIBIOS		TRUE
#define GFX_USE_OS_WIN32		FALSE
#define GFX_USE_OS_POSIX		FALSE

/* GFX subsystems to turn on */
#define GFX_USE_GDISP			TRUE
#define GFX_USE_TDISP			FALSE
#define GFX_USE_GWIN			TRUE
#define GFX_USE_GEVENT			TRUE
#define GFX_USE_GTIMER			TRUE
#define GFX_USE_GQUEUE			FALSE
#define GFX_USE_GINPUT			TRUE
#define GFX_USE_GADC			FALSE
#define GFX_USE_GAUDIN			FALSE
#define GFX_USE_GAUDOUT			FALSE
#define GFX_USE_GMISC			FALSE

/* Features for the GDISP subsystem */
#define GDISP_NEED_VALIDATION		TRUE
#define GDISP_NEED_CLIP				TRUE
#define GDISP_NEED_TEXT				TRUE
#define GDISP_NEED_CIRCLE			TRUE
#define GDISP_NEED_ELLIPSE			TRUE
#define GDISP_NEED_ARC				FALSE
#define GDISP_NEED_CONVEX_POLYGON	FALSE
#define GDISP_NEED_SCROLL			FALSE
#define GDISP_NEED_PIXELREAD		FALSE
#define GDISP_NEED_CONTROL			FALSE
#define GDISP_NEED_QUERY			TRUE
#define GDISP_NEED_IMAGE			TRUE
#define GDISP_NEED_MULTITHREAD		FALSE
#define GDISP_NEED_ASYNC			TRUE
#define GDISP_NEED_MSGAPI			TRUE

/* GDISP - builtin fonts */
#define GDISP_INCLUDE_FONT_SMALL		TRUE
#define GDISP_INCLUDE_FONT_LARGER		TRUE
#define GDISP_INCLUDE_FONT_UI1			TRUE
#define GDISP_INCLUDE_FONT_UI2			TRUE
#define GDISP_INCLUDE_FONT_LARGENUMBERS	TRUE

/* GDISP image decoders */
#define GDISP_NEED_IMAGE_NATIVE		FALSE
#define GDISP_NEED_IMAGE_GIF		FALSE
#define GDISP_NEED_IMAGE_BMP		TRUE
#define GDISP_NEED_IMAGE_JPG		FALSE
#define GDISP_NEED_IMAGE_PNG		FALSE
#define GDISP_NEED_IMAGE_ACCOUNTING	FALSE

/* Optional image support that can be turned off */

	#define GDISP_NEED_IMAGE_BMP_1		TRUE
	#define GDISP_NEED_IMAGE_BMP_4		FALSE
	#define GDISP_NEED_IMAGE_BMP_4_RLE	FALSE
	#define GDISP_NEED_IMAGE_BMP_8		FALSE
	#define GDISP_NEED_IMAGE_BMP_8_RLE	FALSE
	#define GDISP_NEED_IMAGE_BMP_16		FALSE
	#define GDISP_NEED_IMAGE_BMP_24		FALSE
	#define GDISP_NEED_IMAGE_BMP_32		FALSE


/* Features for the TDISP subsystem. */
#define TDISP_NEED_MULTITHREAD	FALSE

/* Features for the GWIN subsystem. */
#define GWIN_NEED_WINDOWMANAGER	TRUE
#define GWIN_NEED_CONSOLE		TRUE
#define GWIN_NEED_GRAPH			FALSE
#define GWIN_NEED_WIDGET		TRUE
#define GWIN_NEED_BUTTON		TRUE
#define GWIN_NEED_SLIDER		TRUE
#define GWIN_NEED_CHECKBOX		TRUE
#define GWIN_NEED_IMAGE			TRUE
#define GWIN_NEED_RADIO			TRUE

/* Features for the GEVENT subsystem. */
#define GEVENT_ASSERT_NO_RESOURCE	FALSE

/* Features for the GTIMER subsystem. */
/* NONE */

/* Features for the GQUEUE subsystem. */
#define GQUEUE_NEED_ASYNC		FALSE
#define GQUEUE_NEED_GSYNC		FALSE
#define GQUEUE_NEED_FSYNC		FALSE

/* Features for the GINPUT subsystem. */
#define GINPUT_NEED_MOUSE		FALSE
#define GINPUT_NEED_KEYBOARD	FALSE
#define GINPUT_NEED_TOGGLE		TRUE
#define GINPUT_NEED_DIAL		FALSE

/* Features for the GADC subsystem. */
/* NONE */

/* Features for the GAUDIN subsystem. */
/* NONE */

/* Features for the GAUDOUT subsystem. */
/* NONE */

/* Features for the GMISC subsystem. */
#define GMISC_NEED_ARRAYOPS		FALSE
#define GMISC_NEED_FASTTRIG		FALSE
#define GMISC_NEED_FIXEDTRIG	FALSE

/* Optional Parameters for various subsystems */
/*
	#define GDISP_MAX_FONT_HEIGHT			16
	#define GEVENT_MAXIMUM_SIZE				32
	#define GEVENT_MAX_SOURCE_LISTENERS		32
	#define GTIMER_THREAD_WORKAREA_SIZE		512
	#define GADC_MAX_LOWSPEED_DEVICES		4
	#define GWIN_BUTTON_LAZY_RELEASE		FALSE
	#define GWIN_CONSOLE_USE_BASESTREAM		FALSE
	#define GWIN_CONSOLE_USE_FLOAT			FALSE
	#define GWIN_NEED_IMAGE_ANIMATION		FALSE
*/

/* Optional Low Level Driver Definitions */
/*
	#define GDISP_USE_CUSTOM_BOARD		FALSE
	#define GDISP_SCREEN_WIDTH			320
	#define GDISP_SCREEN_HEIGHT			240
	#define GDISP_USE_FSMC
	#define GDISP_USE_GPIO
	#define TDISP_COLUMNS				16
	#define TDISP_ROWS					2
*/

#endif /* _GFXCONF_H */
