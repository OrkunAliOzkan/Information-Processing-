#include <stdio.h>
#include <string.h>
#define CHARLIM 256		// Maximum character length of what the user places in memory.  Increase to allow longer sequences
#define QUITLETTER '~' 		// Letter to kill all processing
#include "system.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <stdlib.h>
#include "alt_types.h"
#include "sys/times.h"


#define OFFSET -32
#define PWM_PERIOD 16

alt_8 pwm = 0;
alt_u8 led;
int level;

void print_text(char *text, const int length) {
	char *printMsg;
	asprintf(&printMsg, "<--> Detected %d characters: %s <--> \n %c", length, text, 0x4); 	// Print out the strings
	alt_putstr(printMsg);
	free(printMsg);
	memset(text, 0, 2*CHARLIM);								// Empty the text buffer for next input
}

char generate_text(char curr, int *length, char *text, int *running) {
	if(curr == '\n') return curr;								// If the line is empty, return nothing.
	int idx = 0;											// Keep track of how many characters have been sent down for later printing
	char newCurr = curr;

	while (newCurr != EOF && newCurr != '\n'){						// Keep reading characters until we get to the end of the line
		if (newCurr == QUITLETTER) { *running = 0; }					// If quitting letter is encountered, setting running to 0
		text[idx] = newCurr;								// Add the next letter to the text buffer
		idx++;										// Keep track of the number of characters read
		newCurr = alt_getchar();							// Get the next character
	}
	*length = idx;

	return newCurr;
}

int read_chars() {
	char text[2*CHARLIM];									// The buffer for the printing text
	char prevLetter = '!';
	int length = 0;
	int running = 1;

	while (running) {									// Keep running until QUITLETTER is encountered
		prevLetter = alt_getchar();							// Extract the first character (and create a hold until one arrives)
		prevLetter = generate_text(prevLetter, &length, text, &running);		// Process input text
		//print_text(text, length);							// Print input text
		return prevLetter;
	}
}

void led_write(alt_u8 led_pattern) {
    IOWR(LED_BASE, 0, led_pattern);
}

void convert_read(alt_32 acc_read, int * level, alt_u8 * led) {
    acc_read += OFFSET;
    alt_u8 val = (acc_read >> 6) & 0x07;
    * led = (8 >> val) | (8 << (8 - val));
    * level = (acc_read >> 1) & 0x1f;
}

void sys_timer_isr() {
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

    if (pwm < abs(level)) {

        if (level < 0) {
            led_write(led << 1);
        } else {
            led_write(led >> 1);
        }

    } else {
        led_write(led);
    }

    if (pwm > PWM_PERIOD) {
        pwm = 0;
    } else {
        pwm++;
    }

}

void timer_init(void * isr) {

    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x0900);
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0000);
    alt_irq_register(TIMER_IRQ, 0, isr);
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);

}

float FIR(float x_read, float* x, float* coef, int len){
		float y = 0.0;
			for (int i = (len-1) ; i > 0 ; i--){
				x[i] = x[i-1];
				y = y + (coef[i] * x[i]);
			}
			x[0] = x_read;
			y = y + (coef[0] * x[0]);
		return y;
	}


int main() {

	alt_32 x_read;
	alt_32 y;
	alt_32 cmd;
	alt_up_accelerometer_spi_dev * acc_dev;
	acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
	if (acc_dev == NULL) { // if return 1, check if the spi ip name is "accelerometer_spi"
	    return 1;
	}
	timer_init(sys_timer_isr);
	cmd = alt_getchar();
	while (1){
		if(cmd=='n'){
			alt_up_accelerometer_spi_read_x_axis(acc_dev, & x_read);
			alt_printf("raw data: %x\n", x_read);
			convert_read(x_read, & level, & led);
		}
		if(cmd=='f') {
//			alt_up_accelerometer_spi_read_x_axis(acc_dev, & x_read);
//			alt_printf("filtered data: %x\n", x_read);
//			convert_read(x_read, & level, & led);
//			float coef[5] = {5,5,5,5,5};
//			float x[5]; /* or any other initial condition*/
//			y = FIR(x_read, x,  coef, 5);
			IOWR(LED_BASE, 0, 2);
		}
	}




}
