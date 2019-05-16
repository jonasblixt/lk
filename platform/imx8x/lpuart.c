
#include <reg.h>
#include <stdio.h>
#include <trace.h>
#include <lib/cbuf.h>
#include <kernel/thread.h>
#include <platform/interrupts.h>
#include <platform/debug.h>
#include <platform/lpuart.h>


uint32_t lpuart_init(void)
{
    uint32_t tmp;
    uint64_t base = 0xFFFFFFFf5A060000;
	tmp = readl(base + CTRL);
	tmp &= ~(CTRL_TE | CTRL_RE);
	writel(tmp,base + CTRL);

	writel(0,base + MODIR);
	writel( ~(FIFO_TXFE | FIFO_RXFE), base + FIFO);

	writel(0,base + MATCH);

    writel(0x402008b, base + BAUD);


	tmp = readl(base + CTRL);
	tmp &= ~(LPUART_CTRL_PE_MASK | LPUART_CTRL_PT_MASK | LPUART_CTRL_M_MASK);
	writel(tmp,base + CTRL);

	writel( CTRL_RE | CTRL_TE, base + CTRL);

    return 0;
}

uint32_t lpuart_putc(char c)
{

    uint64_t base = 0xFFFFFFFF5A060000;
    while (!(readl( base+ STAT) & (1 << 22)))
        __asm__("nop");

    writel(c, base + DATA);
    return 0;
}

void uart_init_early(void)
{
    lpuart_init();
}

void uart_init(void)
{
}

int uart_putc(int port, char c)
{
    lpuart_putc(c);
    return 1;
}

void intc_init(void)
{
}

int uart_getc(int port, bool wait)
{
    return -1;
}
