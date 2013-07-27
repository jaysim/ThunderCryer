/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    drivers/gdisp/T6963C/gdisp_lld_board_ThunderCryer.h
 * @brief   GDISP Graphic Driver subsystem board interface for the T6963C display.
 *
 * @addtogroup GDISP
 * @{
 */

#ifndef GDISP_LLD_BOARD_H
#define GDISP_LLD_BOARD_H

#include "gfx.h"
#include "ch.h"
#include "hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// data port
#define GLCD_DATA_PORT          GPIOE
#define GLCD_DATA_PORT_MASK     0xff00
#define GLCD_DATA_OFFSET        8

#define GLCD_DATA_INPUT         palSetGroupMode(GLCD_DATA_PORT, GPIOE_D0 | \
                                        GPIOE_D1 | GPIOE_D2 | GPIOE_D3 | \
                                        GPIOE_D4 | GPIOE_D5 | GPIOE_D6 | \
                                        GPIOE_D7,0, PAL_MODE_INPUT)

#define GLCD_DATA_OUTPUT        palSetGroupMode(GLCD_DATA_PORT, GPIOE_D0 | \
                                        GPIOE_D1 | GPIOE_D2 | GPIOE_D3 | \
                                        GPIOE_D4 | GPIOE_D5 | GPIOE_D6 | \
                                        GPIOE_D7,0, PAL_MODE_OUTPUT_PUSHPULL)

// control port
#define GLCD_CTRL_PORT          GPIOB

// control signals
#define GLCD_RD         GPIOB_RD
#define GLCD_WR         GPIOB_WR
#define GLCD_RESET      GPIOB_RST
#define GLCD_CE         GPIOB_CE
#define GLCD_CD         GPIOB_CD


// aux pins
#define GLCD_CONTRAST_PORT  GPIOB
#define GLCD_CONTRAST_MASK  (GPIOB_CONTRAST_A0 | GPIOB_CONTRAST_A1 | GPIOB_CONTRAST_A2)
#define GLCD_LED            GPIOB_LED


/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GPTConfig gptConf14 = {
    2e6,  //timer clock
    NULL   //timer callback
};

static PWMConfig pwmconf9 = {
   1e6,
   100,
   NULL,
   {
    {PWM_OUTPUT_ACTIVE_HIGH , NULL},
    {PWM_OUTPUT_DISABLED , NULL},
    {PWM_OUTPUT_DISABLED , NULL},
    {PWM_OUTPUT_DISABLED , NULL}
   }
};

const unsigned long c_iDelayFore = 2;
const unsigned long c_iDelayAfter = 1;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



#endif /* GDISP_LLD_BOARD_H */
/** @} */

