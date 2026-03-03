#include <stdio.h>
#include "myGPIO.h"
#include "driver/gpio.h"

//LED
void myGPIO_LED_aan(int pin);
void myGPIO_LED_uit(int pin);
void myGPIO_LED_toggle(int pin);

void myGPIO_LED_setup(int pin)
{
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

void myGPIO_LED_write_value(int pin, int value);

//INPUT knop
void myGPIO_KNOP_setup(int pin)
{
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
}

int myGPIO_KNOP_lees(int pin)
{
    return gpio_get_level(pin);
}

int myGPIO_KNOP_flank(int pin);