/**
 * C Program to set the PIO in motion blinking the LEDs
 */

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blink.pio.h"

const uint LED_PIN = 25;

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq, float clkdiv);

int main() {
    int i = 0;
    
    setup_default_uart();

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &blink_program);
    printf("Loaded program at %d\n", offset);

    while(1)
    {
        i++;
        blink_pin_forever(pio, 0, offset, LED_PIN, 10, (i % 10) + 1);        
        printf("Busy counting away i = %d\n", i);
        sleep_ms(10000);       
    }
}

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq, float clkdiv) {
    blink_program_init(pio, sm, offset, pin, clkdiv);
    pio_sm_set_enabled(pio, sm, true);

    printf("Blinking pin %d at %f Hz\n", pin, freq / clkdiv);
    pio->txf[sm] = clock_get_hz(clk_sys) / freq;
}
