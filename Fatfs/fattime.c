

#include "integer.h"
#include "fattime.h"
//#include "rtc.h" //RPi

/**
 *
 * @return 	bit 31:25
 * 				Year from 1980 (0..127)
 * 			bit 24:21
 * 			    Month (1..12)
 * 			bit 20:16
 * 			    Day in month(1..31)
 * 			bit 15:11
 * 				 Hour (0..23)
 * 			bit 10:5
 * 				 Minute (0..59)
 * 			bit 4:0
 * 				 Second / 2 (0..29)
 *
 */
DWORD get_fattime (void)
{


  return 0;
}

