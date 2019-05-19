/*
 * Copyright (c) 2015 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <reg.h>
#include <err.h>
#include <debug.h>
#include <trace.h>

#include <dev/uart.h>
#include <arch.h>
#include <lk/init.h>
#include <kernel/vm.h>
#include <kernel/spinlock.h>
#include <dev/timer/arm_generic.h>
#include <dev/interrupt/arm_gic.h>
#include <platform.h>
#include <platform/interrupts.h>
#include <lib/watchdog.h>
#include <libfdt.h>
#include <arch/arm64.h>
#include <arch/arm64/mmu.h>
#include <platform/imx8x.h>
#include <platform/sci/ipc.h>
#include <platform/sci/sci.h>

sc_ipc_t ipc_handle;

/* initial memory mappings. parsed by start.S */
struct mmu_initial_mapping mmu_initial_mappings[] =
{
    {
        .phys = 0x80000000,
        .virt = 0xFFFF000000000000,
        .size = (1024*1024*1024),
        .flags = 0,
        .name = "memory"
    },
    /* MU1 */
    {
        .phys = 0x5D1c0000,
        .virt = 0xFFFFFFFF5D1c0000,
        .size = (64*1024),
        .flags = MMU_INITIAL_MAPPING_FLAG_DEVICE,
        .name = "SCU MU2"
    },
    /* Debug uart */
    {
        .phys = 0x5A060000,
        .virt = 0xFFFFFFFF5A060000,
        .size = (64*1024),
        .flags = MMU_INITIAL_MAPPING_FLAG_DEVICE,
        .name = "LPUART0"
    },

    {
        .phys = 0x51a00000,
        .virt = 0xFFFFFFFF51a00000,
        .size = (16*1024*1024),
        .flags = MMU_INITIAL_MAPPING_FLAG_DEVICE,
        .name = "priv"
    },

    /* null entry to terminate the list */
    { 0 }
};

#define DEBUG_UART 1

extern void intc_init(void);
extern void arm_reset(void);


static pmm_arena_t arena =
{
    .name = "ram",
    .base = 0x82000000, 
    .size = (128*1024*1024),
    .flags = PMM_ARENA_FLAG_KMAP,
};

void platform_init_mmu_mappings(void)
{
}

extern void psci_call(ulong arg0, ulong arg1, ulong arg2, ulong arg3);

void platform_early_init(void)
{
    uart_init_early();

    intc_init();


    sc_ipc_open(&ipc_handle, 0x5D1c0000);
    
    arm_gic_init();
    

    /* add the main memory arena */
    pmm_add_arena(&arena);

}

void platform_init(void)
{
    uart_init();

    arm_generic_timer_init(14, 100);

    watchdog_hw_init(1000);
    watchdog_hw_set_enabled(true);


    uint16_t lc;
    uint16_t monotonic;
    uint32_t uid[2];

    sc_misc_seco_chip_info(ipc_handle, &lc, &monotonic, &uid[0], &uid[1]);

    dprintf (SPEW, "%lu\n", current_time());
    dprintf (SPEW, "IMX8X uid: %08x%08x\n",uid[0],uid[1]);
    dprintf (SPEW, "%lu\n", current_time());



    /* boot the secondary cpus using the Power State Coordintion Interface */
    ulong psci_call_num = 0xC4000000 + 3; 

    for (uint i = 1; i < SMP_MAX_CPUS; i++)
    {
        psci_call(psci_call_num, i, MEMBASE + KERNEL_LOAD_OFFSET, 0);
    }
}

void platform_dputc(char c)
{
    if (c == '\n')
        uart_putc(DEBUG_UART, '\r');
    uart_putc(DEBUG_UART, c);

}

int platform_dgetc(char *c, bool wait)
{
    int ret = uart_getc(DEBUG_UART, wait);
    if (ret == -1)
        return -1;
    *c = ret;
    return 0;
}

