#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <hal.h>
#include <chprintf.h>

#if STM32_SERIAL_USE_USART1 == TRUE
#define DBG SD1
#elif STM32_SERIAL_USE_USART2 == TRUE
#define DBG SD2
#elif STM32_SERIAL_USE_USART3 == TRUE
#define DBG SD3
#elif HAL_USE_SERIAL_USB == TRUE
#define DBG SDU1
#include "usbcfg.h"
#else
#define DBG
#endif

#if HAL_USE_SERIAL == TRUE || HAL_USE_SERIAL_USB == TRUE
void initSerial(void);
void dbg(const char *msg);
void dbgf(const char *fmt, ...);
#else
#define initSerial(vod)
#define dbg(x)
#define dbgf(x, args...)
#endif

#endif
