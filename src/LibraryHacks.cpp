/**
 ******************************************************************************
 * @file      	LibraryHacks.cpp
 * @author    	Tecnologic86
 * @version   	V0.0.0
 * @date      	03.07.2012 15:08:09
 * @project	ThunderCryer
 ******************************************************************************
 */



/*
 * LibraryHacks.cpp  *
 *  Created on: 23 Jan 2011  *
 *  Author: Andy  */

#include "ch.hpp"
#include "hal.h"
#include "chrtclib.h"
#include <cstdlib>


/*
 * The default pulls in 70K of garbage
 */
namespace __gnu_cxx {
void __verbose_terminate_handler() {
	for(;;);
}
}

/*
 * The default pulls in about 12K of garbage
 */
extern "C" void __cxa_pure_virtual() {
	for(;;);
}


/*
 * time implementation
 */
extern "C" int _gettimeofday( struct timeval *ptimeval, void *ptimezone){
  (void)ptimezone;
  (void)ptimeval;
  return (int)rtcGetTimeUnixSec(&RTCD1);
}


/*
 * Implement C++ new/delete operators using the heap
 */
void *operator new(size_t size) {
	return chCoreAlloc(size);
}

void *operator new[](size_t size) {
	return chCoreAlloc(size);
}

void operator delete(void *p) {
	free(p);
}

void operator delete[](void *p) {
	free(p);
}
extern SerialUSBDriver SDU2;
int _write_r(struct _reent *r, int file, char * ptr, int len)
{
  (void)r;
  (void)file;
  (void)ptr;
  while(len){
    chSequentialStreamPut(&SDU2, *ptr);
    ptr++;
    len--;
  }
  return len;
}


extern "C" {
  void *__dso_handle = 0;

/*! \brief User defined behaviour for the assert macro \ingroup Functions \synopsis

 \desc There is no default implementation of \b \this. Keeping \b \this 
  out of the library means that you can can customize its behaviour without rebuilding 
  the library. You must implement this function where \a expression is the stringized
  expression, \a filename is the filename of the source file and \a line
  is the linenumber of the failed assertion.
*/
#if 0
void __assert(const char *__expression, const char *__filename, int __line){

	//debug_printf("%s in file: %s, at line: %s", __expression, __filename, __line);
}
#endif
}

