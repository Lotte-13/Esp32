#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "TFT_eSPI.h"

void app_main(void)
{
    tft_init();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}