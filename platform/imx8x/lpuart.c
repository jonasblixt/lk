
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
    uint32_t base = 0x5A060000;
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

    while (!(readl( 0x5A060000+ STAT) & (1 << 22)))
        __asm__("nop");

    writel(c, 0x5A060000 + DATA);
    return 0;
}

void uart_init_early(void)
{
    lpuart_init();
}

void uart_init(void)
{
}

void uart_putc(char c)
{
    lpuart_putc(c);
}

void intc_init(void)
{
}

char uart_getc(void)
{
    return 0;
}
