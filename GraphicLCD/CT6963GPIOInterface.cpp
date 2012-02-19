/**
  ******************************************************************************
  * @file      	CT6963GPIOInterface.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	17.02.12
  * @brief		implementation of T6963C Gpio Interface
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CT6963GPIOInterface.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ustime.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

// data port
#define GLCD_DATA_PORT 			GPIOE
#define GLCD_DATA_PORT_MASK 	0xff00
#define GLCD_DATA_MODE_MASK_IN 	0xffff0000
#define GLCD_DATA_MODE_MASK_OUT	0x55550000
#define GLCD_DATA_OFFSET		8
#define GLCD_DATA_INPUT 		GLCD_DATA_PORT->MODER &= ~GLCD_DATA_MODE_MASK_IN;
#define GLCD_DATA_OUTPUT		GLCD_DATA_PORT->MODER |= GLCD_DATA_MODE_MASK_OUT;

// control port
#define GLCD_CTRL_PORT_CD_RD	GPIOC
#define GLCD_CTRL_PORT_WR_CE	GPIOB

// control signals
#define GLCD_WR			GPIO_Pin_0
#define GLCD_RD			GPIO_Pin_5
#define GLCD_CE			GPIO_Pin_1
#define GLCD_CD			GPIO_Pin_4
#define GLCD_RESET		GPIO_Pin_11

const unsigned int c_iDelayFore = 5;
const unsigned int c_iDelayAfter = 3;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



CT6963_GPIO_Interface::CT6963_GPIO_Interface() {
	// TODO Auto-generated constructor stub

}

CT6963_GPIO_Interface::~CT6963_GPIO_Interface() {
	// TODO Auto-generated destructor stub
}

/**
  * @brief  delay for us
  * @param  number of us
  * @retval None
  */
void delay_us(unsigned int us){
	unsigned int i;

	for(i=0; i< (SystemCoreClock/1000000) * us; i++){
		asm volatile ("nop");
	}
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

	GPIO_ResetBits(GLCD_CTRL_PORT_CD_RD , GLCD_RD);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	delay_us(c_iDelayFore);

	tmp = ((GPIO_ReadInputData(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_RD);GLCD_DATA_OUTPUT;

	delay_us(c_iDelayAfter);

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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);

	// all pins as pp outputs with no pull
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

	//Control pins CD and RD
	GPIO_InitStructure.GPIO_Pin = GLCD_CD | GLCD_RD;
	GPIO_Init(GLCD_CTRL_PORT_CD_RD,&GPIO_InitStructure);
	//Control pins WR, CE and RST
	GPIO_InitStructure.GPIO_Pin = GLCD_WR | GLCD_CE | GLCD_RESET;
	GPIO_Init(GLCD_CTRL_PORT_WR_CE,&GPIO_InitStructure);

	// data pins are high byte
	GPIO_InitStructure.GPIO_Pin = GLCD_DATA_PORT_MASK;
	GPIO_Init(GLCD_DATA_PORT,&GPIO_InitStructure);

	//Set all Control pins to high level
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD | GLCD_RD);	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR | GLCD_CE );

}

/**
  * @brief  Reset the Display
  * @param  None
  * @retval None
  */
void CT6963_GPIO_Interface::Reset(void){

	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_RESET | GLCD_CE );

	vTaskDelay(20);  // sleep 20 ms

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_RESET | GLCD_CE );

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

	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	delay_us(c_iDelayFore);   // time for display to read the data

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);
	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);


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

	GPIO_ResetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	delay_us(c_iDelayFore);

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD);
	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);

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

	GPIO_ResetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD | GLCD_RD);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE );

	delay_us(c_iDelayFore);

	tmp = ((GPIO_ReadInputData(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE );
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD | GLCD_RD);

	delay_us(c_iDelayAfter);

	GLCD_DATA_OUTPUT;
	return (unsigned char)tmp;
}
