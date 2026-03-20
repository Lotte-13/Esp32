#include <stdio.h>
#include "myGPIO.h"
#include "esp_random.h"
#include "esp_timer.h"
#include "myGPTimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <unistd.h>

#define LED_PIN_1 17
#define KNOP_PIN_1 2

int getal;
esp_timer_handle_t timer;

void app_main(void)
{   
    myGPIO_KNOP_Setup(KNOP_PIN_1);
    myGPIO_LED_Setup(LED_PIN_1);
    
    myGPIO_KNOP_SetupInterrupt(KNOP_PIN_1);
    
    while (1)
    {
        getal = esp_random()%100;
        vTaskDelay(getal*10);
        printf("randomgetal is " + getal);
    }
}
