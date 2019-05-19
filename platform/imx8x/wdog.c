#include <reg.h>
#include <stdio.h>
#include <err.h>
#include <trace.h>
#include <lib/watchdog.h>
#include <kernel/thread.h>
#include <platform.h>
#include <platform/interrupts.h>
#include <platform/debug.h>
#include <platform/sci/ipc.h>
#include <platform/sci/sci.h>

#define LOCAL_TRACE 1

extern sc_ipc_t ipc_handle;

status_t platform_watchdog_init(lk_time_t  target_timeout,
                                lk_time_t *recommended_pet_period)
{
    LTRACEF("target_timeout %u\n", (uint32_t)target_timeout);

    sc_timer_set_wdog_timeout(ipc_handle, target_timeout);
    sc_timer_set_wdog_action(ipc_handle,SC_RM_PT_ALL,
                                        SC_TIMER_WDOG_ACTION_COLD);
    sc_timer_start_wdog(ipc_handle, true);
    
    return NO_ERROR;
}

void platform_watchdog_set_enabled(bool enabled)
{
    LTRACEF("enabled %u\n", enabled);
}

void platform_watchdog_pet(void)
{
    LTRACEF("kick\n");
    sc_timer_ping_wdog(ipc_handle);
}


