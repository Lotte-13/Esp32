#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"
#include "myGPIO.h"

void app_main(void)
{
    int random = esp_random()%100 + 1; 

    bool guessed_correctly = false;

    while (guessed_correctly == false)
    {
        printf("Guess the random number:\n");
        int guess = 0;
        while (guess == 0)
        {
            scanf("%d", &guess);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        printf("You guessed: %d\n", guess);
        if (guess == random)
        {
            printf("Correct! You guessed the number.\n");
            guessed_correctly = true;
        }
        else if (guess < random)
        {
            printf("Too low! Try again.\n");
            guess = 0;
        }
        else
        {
            printf("Too high! Try again.\n");
            guess = 0;
        }

    }
    
}
