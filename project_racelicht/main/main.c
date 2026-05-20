#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_random.h"
#include "myGPIO.h"
#include "myGPTimer.h"

#define BTN_PIN 25
#define LED_PIN1 12
#define LED_PIN2 13
#define LED_PIN3 15
#define LED_PIN4 2
#define LED_PIN5 32

void app_main(void)
{
    myGPIO_KNOP_Setup(BTN_PIN);
    myGPIO_LED_Setup(LED_PIN1);
    myGPIO_LED_Setup(LED_PIN2);
    myGPIO_LED_Setup(LED_PIN3);
    myGPIO_LED_Setup(LED_PIN4);
    myGPIO_LED_Setup(LED_PIN5);

    myGPIO_LED_On(LED_PIN1);
    myGPIO_LED_On(LED_PIN2);
    myGPIO_LED_On(LED_PIN3);
    myGPIO_LED_On(LED_PIN4);
    myGPIO_LED_On(LED_PIN5);
    
}
