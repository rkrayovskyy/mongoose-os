/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */

#include <inc/hw_types.h>
#include <driverlib/prcm.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <ti/drivers/net/wifi/netutil.h>

#include "common/cs_dbg.h"
#include "common/platform.h"

#include "mgos_hal.h"
#include "cc32xx_exc.h"

void mgos_dev_system_restart(void) {
  sl_DeviceDisable(); /* Turn off NWP */
  MAP_PRCMMCUReset(true /* bIncludeSubsystem */);
}

void SimpleLinkFatalErrorEventHandler(SlDeviceFatal_t *e) {
  cc32xx_exc_printf(
      "SL fatal error 0x%x assert 0x%x,0x%x no_ack 0x%x timeout 0x%x", e->Id,
      e->Data.DeviceAssert.Code, e->Data.DeviceAssert.Value,
      e->Data.NoCmdAck.Code, e->Data.CmdTimeout.Code);
  mgos_system_restart();
}

int mg_ssl_if_mbed_random(void *ctx, unsigned char *buf, size_t len) {
  _u16 len16 = len;
  return sl_NetUtilGet(SL_NETUTIL_TRUE_RANDOM, 0, buf, &len16);
}

#ifndef MGOS_HAVE_WIFI
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *e) {
  (void) e;
}
void SimpleLinkWlanEventHandler(SlWlanEvent_t *e) {
  (void) e;
}
#endif