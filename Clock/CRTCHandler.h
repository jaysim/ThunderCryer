/**
  ******************************************************************************
  * @file      	CFileHandler.h
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	16.05.12
  * @brief		Class to handle the RTC and alarm requests
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CRTCHANDLER_H_
#define CRTCHANDLER_H_

/* Includes ------------------------------------------------------------------*/
#include "AManagedTask.h"
/* namespace -----------------------------------------------------------------*/
namespace rtc_handler_namespace {
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */
class CRTCHandler: public AManagedTask {
private:
	void RTC_Config();
public:
	CRTCHandler();
	virtual ~CRTCHandler();
	bool HardwareInit();
	void Run();
};

} /* namespace rtc_handler_namespace */
#endif /* CRTCHANDLER_H_ */
