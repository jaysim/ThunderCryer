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
#define GLCD_DATA_MODE_MASK_IN  0xffff0000
#define GLCD_DATA_MODE_MASK_OUT 0x55550000
#define GLCD_DATA_OFFSET        8
#define GLCD_DATA_INPUT         GLCD_DATA_PORT->MODER &= ~GLCD_DATA_MODE_MASK_IN;
#define GLCD_DATA_OUTPUT        GLCD_DATA_PORT->MODER |= GLCD_DATA_MODE_MASK_OUT;

// control port
#define GLCD_CTRL_PORT          GPIOB

// control signals
#define GLCD_RD         GPIO_Pin_11
#define GLCD_WR         GPIO_Pin_12
#define GLCD_RESET      GPIO_Pin_13
#define GLCD_CE         GPIO_Pin_14
#define GLCD_CD         GPIO_Pin_15


// aux pins
#define GLCD_CONTRAST_PORT  GPIOB
#define GLCD_CONTRAST_MASK  (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2)
#define GLCD_LED                GPIO_Pin_7

#define GLCD_AUX_PORT       GPIOE

#define GLCD_BACKLIGHT      GPIO_Pin_5
#define GLCD_BUZZER     GPIO_Pin_6


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


const unsigned long c_iDelayFore = 2;
const unsigned long c_iDelayAfter = 1;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



static inline void gdisp_lld_init_board(void) {

}

static inline void gdisp_lld_reset_pin(bool_t state) {

}

static inline void gdisp_lld_write_index(uint16_t reg) {

}

static inline void gdisp_lld_write_data(uint16_t data) {

}

static inline uint16_t gdisp_lld_read_data(void) {

}

static inline void gdisp_lld_backlight(uint8_t percent) {

}


/**
  * @brief  reads T6963C status byte
  * @param  None
  * @retval true for display ready
  */
bool CheckStatus(void)
{

    uint16_t tmp;
    GLCD_DATA_INPUT;

    GPIO_ResetBits(GLCD_CTRL_PORT, GLCD_RD | GLCD_CE);

    delay_us(c_iDelayFore);

    tmp = ((GPIO_ReadInputData(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

    GPIO_SetBits(GLCD_CTRL_PORT,  GLCD_RD | GLCD_CE);

    delay_us(c_iDelayAfter);

    GLCD_DATA_OUTPUT;

    return ((tmp&0x03)==0x03);
}



/**
  * @brief  initialize Hardware to drive the display
  * @param  None
  * @retval None
  */
void CT6963_GPIO_Interface::HardwareInit(void){


    GPIO_InitTypeDef GPIO_InitStructure;
    // activate Clock for Io Ports used
    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);

    // all pins as pp outputs with no pull
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

    //Control pins CD and RD
    GPIO_InitStructure.GPIO_Pin = GLCD_CD | GLCD_RD | GLCD_WR | GLCD_CE | GLCD_RESET;
    GPIO_Init(GLCD_CTRL_PORT,&GPIO_InitStructure);

    // data pins are high byte
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GLCD_DATA_PORT_MASK;
    GPIO_Init(GLCD_DATA_PORT,&GPIO_InitStructure);

    //Set all Control pins to high level
    GPIO_SetBits(GLCD_CTRL_PORT, GLCD_CD | GLCD_RD | GLCD_WR | GLCD_CE);



    /*
     * setup Aux Pins for Bl and so on
     */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStructure.GPIO_Pin = GLCD_CONTRAST_MASK | GLCD_LED;
    GPIO_Init(GLCD_CONTRAST_PORT,&GPIO_InitStructure);

    // set contrast to 4 off 8
    GPIO_SetBits(GLCD_CONTRAST_PORT, (GLCD_CONTRAST_MASK & 4));
        GPIO_ResetBits(GLCD_CONTRAST_PORT, GLCD_LED);

    // set light pins
    GPIO_InitStructure.GPIO_Pin = GLCD_BACKLIGHT | GLCD_BUZZER;
    GPIO_Init(GLCD_AUX_PORT,&GPIO_InitStructure);
    // "Festtagsbeleuchtung"
    GPIO_SetBits(GLCD_AUX_PORT, GLCD_BACKLIGHT | GLCD_BUZZER);

    init_us_timer(); //delay timer init

}

/**
  * @brief  Reset the Display
  * @param  None
  * @retval None
  */
void CT6963_GPIO_Interface::Reset(void){

    GPIO_ResetBits(GLCD_CTRL_PORT, GLCD_RESET | GLCD_CE );
    GPIO_SetBits(GLCD_AUX_PORT, GLCD_BUZZER);

    CTask::Delay(20);  // sleep 20 ms

    GPIO_ResetBits(GLCD_AUX_PORT, GLCD_BUZZER);
    GPIO_SetBits(GLCD_CTRL_PORT, GLCD_RESET | GLCD_CE );

}

/**
  * @brief  writes a command
  * @param  command
  * @retval None
  */
void CT6963_GPIO_Interface::WriteCommand(const unsigned char command){

    while(!CheckStatus());

    GLCD_DATA_PORT->ODR &= ~GLCD_DATA_PORT_MASK;  //Clear Data pins
    GLCD_DATA_PORT->ODR |= (command << (GLCD_DATA_OFFSET)); // lowbyte will stay as is in this write

    GPIO_ResetBits(GLCD_CTRL_PORT, GLCD_WR | GLCD_CE);

    delay_us(c_iDelayFore);   // time for display to read the data

    GPIO_SetBits(GLCD_CTRL_PORT , GLCD_WR | GLCD_CE);


    delay_us(c_iDelayAfter);
}

/**
  * @brief  writes a data
  * @param  data
  * @retval None
  */
void CT6963_GPIO_Interface::WriteData(const unsigned char data){
    while(!CheckStatus());

    GLCD_DATA_PORT->ODR &= ~GLCD_DATA_PORT_MASK;  //Clear Data pins
    GLCD_DATA_PORT->ODR |= ((uint16_t)data << GLCD_DATA_OFFSET); // lowbyte will stay as is in this write

    GPIO_ResetBits(GLCD_CTRL_PORT, GLCD_CD | GLCD_WR | GLCD_CE);

    delay_us(c_iDelayFore);

    GPIO_SetBits(GLCD_CTRL_PORT, GLCD_CD | GLCD_WR | GLCD_CE);

    delay_us(c_iDelayAfter);
}

/**
  * @brief  reads data from display
  * @param  None
  * @retval data from display
  */
unsigned char CT6963_GPIO_Interface::ReadData(){
    uint16_t tmp;
    while(!CheckStatus());

    GLCD_DATA_INPUT;

    GPIO_ResetBits(GLCD_CTRL_PORT, GLCD_RD | GLCD_CD | GLCD_CE);

    delay_us(c_iDelayFore);

    tmp = ((GPIO_ReadInputData(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

    GPIO_SetBits(GLCD_CTRL_PORT, GLCD_RD | GLCD_CD | GLCD_CE );

    delay_us(c_iDelayAfter);

    GLCD_DATA_OUTPUT;
    return (unsigned char)tmp;
}

#endif /* GDISP_LLD_BOARD_H */
/** @} */

