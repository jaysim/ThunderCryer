/**
  ******************************************************************************
  * @file      	CUSB_MassStorage.h
  * @author    	Tecnologic86
  * @version   	V0.0.0
  * @date      	10.03.12
  * @brief		Wrapper for USB_MSC Handling
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSBMASSSTORAGE_H_
#define CUSBMASSSTORAGE_H_
/* Includes ------------------------------------------------------------------*/
#include "AManagedTask.h"
#include "usb_hcd_int.h"
#include "usbh_usr.h"
#include "usbh_core.h"
#include "usbh_msc_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definitions -------------------------------------------------------- */

class CUSB_MassStorage: public AManagedTask {
private:

public:

	CUSB_MassStorage();
	virtual ~CUSB_MassStorage();
	bool HardwareInit(void);
	void Run(void);
	bool IsDeviceConnected(void);
};


#endif /* CUSBMASSSTORAGE_H_ */
