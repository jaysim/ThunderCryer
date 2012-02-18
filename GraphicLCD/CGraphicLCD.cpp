/**
  ******************************************************************************
  * @file      	CGraphicLCD.cpp
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	17.02.12
  * @brief		T6963C Graphics LCD driver Class
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CGraphicLCD.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

CGraphicLCD::CGraphicLCD() {
	bInverse=false;
}

CGraphicLCD::CGraphicLCD(bool inverse) {
	bInverse=inverse;
}

CGraphicLCD::~CGraphicLCD() {
	// TODO Auto-generated destructor stub
}

/**
  * @brief gives inverted draw status
  * @param  None
  * @retval true on inverse mode
  */
bool CGraphicLCD::Inverse(){
	return bInverse;
}

/**
  * @brief  sets inverted draw mode
  * @param  ture for inverted draw mode
  * @retval None
  */
void CGraphicLCD::Inverse(bool inverse){
	bInverse = inverse;
}
