#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_random.h"

#define BTN_PIN 26
#define LED_PIN 27

bool button_pressed;
int counter; 
static void IRAM_ATTR ButtonISR(void* args) 
{
    button_pressed = 1;
    gpio_set_level(LED_PIN, 0);

}

void app_main(void)
{
    gpio_reset_pin(BTN_PIN);
    gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_PIN, GPIO_PULLDOWN_ONLY);

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    gpio_set_intr_type(BTN_PIN, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BTN_PIN, ButtonISR, (void *)BTN_PIN);
    
    while (1) 
    {
        counter = 0;
        int random = esp_random()%10;
        printf("Random number is: %d\n", random);
        int delay = 1000 / portTICK_PERIOD_MS;
        while(counter <= random)
        {
            counter++;
            vTaskDelay(delay);
        }
        button_pressed = 0;
        gpio_set_level(LED_PIN, 1);
        counter = 0;
        while (button_pressed == 0)
        {
            button_pressed = gpio_get_level(BTN_PIN);
            counter++;
            vTaskDelay(1 / portTICK_PERIOD_MS);

        }
        printf("Button pressed after %d ms\n", counter / 100);
        
    }
    
}