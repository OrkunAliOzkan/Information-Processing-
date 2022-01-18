#include <sys/alt_stdio.h>
#include <stdio.h>
#include "altera_avalon_pio_regs.h"
#include "system.h"

int main()
{
	int switch_datain;
	alt_putstr("Hello from Nios II!\n");

	while (1){
		switch_datain = ~IORD_ALTERA_AVALON_PIO_DATA(BUTTON_BASE);
		switch_datain &= (0b0000001111);
		IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE,switch_datain);
	}
	return 0;
}