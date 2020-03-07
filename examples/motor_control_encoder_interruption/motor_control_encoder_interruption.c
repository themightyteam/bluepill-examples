#include "setup.h"
#include "utils.h"

#define MILLISEC_SLICES 9000

volatile uint32_t millis = 0;
volatile uint32_t left_encoder_updates = 0;

/*
 * Setup clocks of internal connections
 */

void clock_setup(void)
{
	/* Enable GPIOB clocks (for PWM and motor control pins) */
	rcc_periph_clock_enable(RCC_GPIOB);

	/* Enable GPIOA */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable GPIO clock C (internal LED */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Enable USART */
	rcc_periph_clock_enable(RCC_USART1);

	/* Enable TIMER for encoder (left encoder) */
	rcc_periph_clock_enable(RCC_LEFT_ENCODER);
	timer_reset(RCC_LEFT_ENCODER);

	/* Activate clock for AFIO, if used */
	if (USE_ALTERNATE_FUNCTIONS) {
		rcc_periph_clock_enable(RCC_AFIO);
	}
}

/*
 * @brief Setup usart
 */
void usart_setup(void)
{
	nvic_set_priority(NVIC_USART1_IRQ, 15);
	nvic_enable_irq(NVIC_USART1_IRQ);

	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_USART1_TX);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		      GPIO_USART1_RX);

	/* Setup USART PARAMETERS */
	usart_set_baudrate(USART1, USART_BAUDRATE);
	usart_set_databits(USART1, USART_DATABITS);
	usart_set_stopbits(USART1, USART_STOPBITS);
	usart_set_mode(USART1, USART_MODE);
	usart_set_parity(USART1, USART_PARITY);
	usart_set_flow_control(USART1, USART_FLOWCONTROL);

	/* Enable RX interruptions to usart1_isr() function */
	usart_enable_rx_interrupt(USART1);
	/* Enable USART */
	usart_enable(USART1);
}

void encoder_setup(uint32_t timer, int afio_function, int channel1,
		   int channel2, int channel1_ti, int channel2_ti)
{
	if (afio_function) {
		gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
				   afio_function);
	}

	/* No reset clock: full period */
	timer_set_period(timer, 1);

	/* Encoders in quadrature */
	timer_slave_set_mode(timer, TIM_SMCR_SMS_EM3);

	/* Set input channels */
	timer_ic_set_input(timer, channel1, channel1_ti);
	timer_ic_set_input(timer, channel2, channel2_ti);

	timer_enable_counter(timer);
}

/*
 * @brief systick setup
 *
 * @note configure the system tick to interrupt each 1 msec
 */
void systick_setup()
{
	/* 72MHz / 8 => 9000000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	/* 9000000/9000 = 1000 overflows per second - one interrupt every 1ms*/
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(MILLISEC_SLICES - 1);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();
}

void sys_tick_handler(void)
{
	// Increase systick calls
	millis++;
}

void tim2_isr(void)
{
	if (timer_get_flag(TIM2, TIM_SR_UIF)) {
		// count the number of left_encoder updates
		left_encoder_updates++;

		/* Clear interrrupt flag. */
		timer_clear_flag(TIM2, TIM_SR_UIF);
	}
}

void setup_irq_timers(void)
{

	/* Without this the timer interrupt routine will never be called. */
	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 1);

	/* Update interrupt enable. */
	timer_enable_irq(TIM2, TIM_DIER_UIE);
}

void gpio_setup(void)
{
	/* Set internal LED */
	gpio_set_mode(LED1_PORT, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, LED1_PIN);
}

/*
 * @brief setup of microcontroller functionality
 */
void setup_microcontroller(void)
{
	/* Change interrupt vector table location to avoid
	 *  conflict with serial bootloader interrupt vectors
	 */

	SCB_VTOR = (uint32_t)0x08000000;
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	clock_setup();
	gpio_setup();
	usart_setup();
	/* left encoder */
	encoder_setup(LEFT_ENCODER_TIMER, LEFT_ENCODER_AFIO,
		      LEFT_ENCODER_CHANNEL1, LEFT_ENCODER_CHANNEL2,
		      LEFT_ENCODER_CHANNEL1_TI, LEFT_ENCODER_CHANNEL2_TI);

	/* Line sensor setup */
	systick_setup();

	/* Timer interruptions setup */
	setup_irq_timers();
}

/*
 * @brief main function
 *
 */
int main(void)
{

	setup_microcontroller();

	uint32_t last_loop_ms = 0;
	uint32_t current_loop_ms = 0;

	while (1) {
		current_loop_ms = millis;

		if ((current_loop_ms - last_loop_ms) >= 1000) {
			gpio_toggle(INTERNAL_LED_PORT, INTERNAL_LED);
			printf("Time: %lu Updates: %lu\n", millis / 1000,
			       left_encoder_updates);
			last_loop_ms = current_loop_ms;
		}
	}

	return 0;
}
