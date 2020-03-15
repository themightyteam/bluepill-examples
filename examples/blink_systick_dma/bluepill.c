#include "bluepill.h"

static void clock_setup(void)
{
	/* Set main clock at 72MHz */
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable LED clock. */
	rcc_periph_clock_enable(RCC_INTERNAL_LED);

	/* Enable USART clock. */
	rcc_periph_clock_enable(RCC_GPIOA); // Needed when BOOT0 is 0
	rcc_periph_clock_enable(RCC_USART1);

	/* Enable DMA */
	rcc_periph_clock_enable(RCC_DMA1);
}

static void gpio_setup()
{
	/* Set GPIO mode of internal LED */
	gpio_set_mode(INTERNAL_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, INTERNAL_LED);

	/* Setup GPIO mode of pin GPIO_USART1_TX. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		      GPIO_USART1_RX);
}

static void dma_setup()
{
	// UART TX on DMA1 Channel 4
	nvic_set_priority(NVIC_DMA1_CHANNEL4_IRQ, 0);
	nvic_enable_irq(NVIC_DMA1_CHANNEL4_IRQ);
}

static void usart_setup(void)
{

	nvic_set_priority(NVIC_USART1_IRQ, 16);
	nvic_enable_irq(NVIC_USART1_IRQ);

	/* Setup USART PARAMETERS */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 9);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_parity(USART1, USART_PARITY_EVEN);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Enable RX interruptions to usart1_isr() function */
	usart_enable_rx_interrupt(USART1);

	/* Enable USART */
	usart_enable(USART1);
}

static void systick_setup()
{
	/* 72MHz => 72.000.000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);

	/* 72.000.000/72.000 = 1000 overflows per second - one interrupt every
	 * 1ms*/
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(72000 - 1);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();
}

void bluepill_setup()
{
	/* Change interrupt vector table location to avoid
	 * conflict with serial bootloader interrupt vectors
	 */
	SCB_VTOR = (uint32_t)0x08000000;

	clock_setup();
	gpio_setup();
	systick_setup();
	usart_setup();
	dma_setup();
}

void bluepill_update()
{
	dma_update();
}