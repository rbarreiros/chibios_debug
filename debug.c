#include <ch.h>

#include "debug.h"

#if HAL_USE_SERIAL_USB == TRUE
#include "usbcfg.h"
#endif

#if HAL_USE_SERIAL == TRUE || HAL_USE_SERIAL_USB == TRUE
static mutex_t mtx;


void initSerial(void)
{
#if STM32_SERIAL_USE_USART1
  palSetPadMode(GPIOA, 9, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  palSetPadMode(GPIOA, 10, PAL_MODE_INPUT);
#elif STM32_SERIAL_USE_USART2
  palSetPadMode(GPIOA, 2, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  palSetPadMode(GPIOA, 3, PAL_MODE_INPUT);
#elif STM32_SERIAL_USE_USART3
  palSetPadMode(GPIOB, 10, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  palSetPadMode(GPIOB, 11, PAL_MODE_INPUT);
#endif

  chMtxObjectInit(&mtx);

#if HAL_USE_SERIAL_USB == FALSE  
  SerialConfig sconf = { 115200, 0, 0, 0 };
  
  sdStart(&DBG, &sconf);
#endif

  // Start USB
#if HAL_USE_SERIAL_USB == TRUE
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);
  
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(3000);
#endif
  
  dbg("\r\n\r\n");
  dbg(":: Debug interface initialized");
}

void dbg(const char *msg)
{
  chMtxLock(&mtx);
  chprintf((BaseSequentialStream *)&DBG, msg);
  chprintf((BaseSequentialStream *)&DBG, "\r\n");
  chMtxUnlock(&mtx);
}

void dbgf(const char *fmt, ...)
{
  va_list ap;
  chMtxLock(&mtx);
  va_start(ap, fmt);
  chvprintf((BaseSequentialStream *)&DBG, fmt, ap);
  va_end(ap);
  chMtxUnlock(&mtx);
}
#endif
