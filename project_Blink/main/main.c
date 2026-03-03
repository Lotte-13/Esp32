#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "myGPIO.h"

#define LED 33
#define KNOP 35

void app_main(void)
{
    int teller = 0;
    myGPIO_LED_setup(LED);
    myGPIO_KNOP_setup(KNOP);

    if (myGPIO_KNOP_lees(KNOP) == 1)
    {
        teller++;
    }
    if (teller == 5)
    {
        myGPIO_LED_aan(LED);
        vTaskDelay(100);
        myGPIO_LED_uit(LED);
        teller = 0;
    } 
}
