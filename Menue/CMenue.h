/**
  ******************************************************************************
  * @file      	CMenue.h
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	18.02.12
  * @brief		Class for Menu displayed on GraphicsLCD
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CMENUE_H_
#define CMENUE_H_
/* Includes ------------------------------------------------------------------*/
#include "AManagedTask.h"
#include "CGraphicLCD.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */

class CMenue: public AManagedTask {
private:
	CGraphicLCD &glcd;
public:
	bool HardwareInit(void);
	void Run(void);

	CMenue(CGraphicLCD display);
	virtual ~CMenue();
};

#endif /* CMENUE_H_ */
