#ifndef __IMX_WDOG_H__
#define __IMX_WDOG_H__

#include <stdint.h>

#define WDOG_WCR  0x0000
#define WDOG_WSR  0x0002
#define WDOG_WRSR 0x0004
#define WDOG_WICR 0x0006
#define WDOG_WMCR 0x0008

uint32_t imx_wdog_init(uint64_t base, uint32_t delay);
void imx_wdog_kick(void);
void imx_wdog_reset_now(void);

#endif
