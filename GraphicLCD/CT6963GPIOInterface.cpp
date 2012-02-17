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
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
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
  * @brief  delay function
  * @param  None
  * @retval None
  *
  * considering several approaches FSMC interface is beautiful but has one drawback R50 on the Discovery has to be removed
  * the next approach was to realize the delay with an Timer an binary semaphore but with 2 to 5µs Delay and approximately 1µs
  * for one context switch and maybe 10 commands in between 2 delays the Os would be switched to death.
  * longer delays for 1 tick or something in this region would slow down the display interface by factor 500 or more
  */




GPIO_InitTypeDef GPIO_InitStructure;
// activate Clock for Io Ports used
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);

init_us_timer();

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
GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD | GLCD_RD);	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR | GLCD_CE );

GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_RESET | GLCD_CE );

vTaskDelay(5);  // sleep 5 ms

GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_RESET | GLCD_CE );


/**
  * @brief  reads T6963C status byte
  * @param  None
  * @retval true for display ready
  */
bool CGraphicLCD::CheckStatus(void)
{

	uint16_t tmp;
	GLCD_DATA_INPUT;

	GPIO_ResetBits(GLCD_CTRL_PORT_CD_RD , GLCD_RD);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	delay();

	tmp = ((GPIO_ReadInputData(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_RD);GLCD_DATA_OUTPUT;

	delay();

	return (tmp&0x03==0x03);
}

/**
  * @brief  writes a command
  * @param  command
  * @retval None
  */
void CGraphicLCD::WriteCommand(unsigned char command)
{
	while(!CheckStatus());

	GLCD_DATA_PORT->ODR &= ~GLCD_DATA_PORT_MASK;  //Clear Data pins
	GLCD_DATA_PORT->ODR |= (command << (GLCD_DATA_OFFSET)); // lowbyte will stay as is in this write

	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	Delay();   // time for display to read the data

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);
	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	Delay();

}

/**
  * @brief  writes data
  * @param  data
  * @retval None
  */
void CGraphicLCD::WriteData(unsigned char data)
{
	while(!CheckStatus());

	GLCD_DATA_PORT->ODR &= ~GLCD_DATA_PORT_MASK;  //Clear Data pins
	GLCD_DATA_PORT->ODR |= ((uint16_t)data << GLCD_DATA_OFFSET); // lowbyte will stay as is in this write

	GPIO_ResetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);

	Delay();

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE);
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD);
	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_WR);

	Delay();
}


/**
  * @brief  reads data from T6963c
  * @param  None
  * @retval data byte
  */
unsigned char CGraphicLCD::ReadData(void){
	uint16_t tmp;
	while(!GLCD_CheckStatus());

	GLCD_DATA_INPUT;

	GPIO_ResetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD | GLCD_RD);
	GPIO_ResetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE );

	Delay();

	tmp = ((GPIO_ReadInputData(GLCD_DATA_PORT) & GLCD_DATA_PORT_MASK) >> GLCD_DATA_OFFSET);

	GPIO_SetBits(GLCD_CTRL_PORT_WR_CE , GLCD_CE );
	GPIO_SetBits(GLCD_CTRL_PORT_CD_RD , GLCD_CD | GLCD_RD);

	Delay();

	GLCD_DATA_OUTPUT;
	return (unsigned char)tmp;
}
