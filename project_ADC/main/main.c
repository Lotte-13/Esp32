#include <stdio.h>
#include "myADC.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN1 37
#define LED_PIN2 38
#define LED_PIN3 32

#define CHANNEL ADC_CHANNEL_2

void app_main(void)
{
    int waarde;
    
    gpio_reset_pin(LED_PIN1);
    gpio_reset_pin(LED_PIN2);
    gpio_reset_pin(LED_PIN3);
    gpio_set_direction(LED_PIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PIN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PIN3, GPIO_MODE_OUTPUT);    

    while (1) {
        myADC_setup(CHANNEL);
        waarde = myADC_inlezen(CHANNEL);
        int millivolt = myADC_getMillivolt(CHANNEL);
        printf("ADC waarde: %d, mV: %d\n", waarde, millivolt); 
    }
}
