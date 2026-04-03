#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"
#include "myGPIO.h"

int counter = 0;

void app_main(void)
{
    int random = esp_random()%100 + 1; 

    bool juist_geraden = false;

    while (juist_geraden == false)
    {
        printf("Raad het getal:\n");
        int gok = 0;

        while (gok == 0)
        {
            scanf("%d", &gok);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        printf("gok: %d\n", gok);
        if (gok == random)
        {
            counter++;
            printf("je hebt het getal geraden in %d pogingen\n", counter);
            juist_geraden = true;
            counter = 0;
        }
        else if (gok < random)
        {
            printf("getal is hoger\n");
            gok = 0;
            counter++;
        }
        else
        {
            printf("getal is lager\n");
            gok = 0;
            counter++;
        }

    }
    
}
