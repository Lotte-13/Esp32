#include <stdio.h>
#include "myGPIO.h"
#include "esp_random.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <unistd.h>

#define LED_PIN_1 17
#define KNOP_PIN_1 2

// Globale timing variabelen
static volatile int64_t led_start_time = 0;
static volatile int reaction_detected = 0;
static volatile int64_t reaction_time_us = 0;

// Interrupt handler voor knop
static void IRAM_ATTR KNOP_ISR(void *arg)
{
    if (led_start_time > 0 && !reaction_detected)
    {
        reaction_time_us = esp_timer_get_time() - led_start_time;
        reaction_detected = 1;
    }
}

void app_main(void)
{
    printf("\n\n=== REACTIE TESTER ===\n\n");
    
    // Setup GPIO
    myGPIO_KNOP_Setup(KNOP_PIN_1);
    myGPIO_LED_Setup(LED_PIN_1);
    
    // Setup interrupt voor knop
    myGPIO_KNOP_SetupInterrupt(KNOP_PIN_1);
    
    // Start het spel
    while (1)
    {
        printf("Wachten op willekeurig moment...\n");
        
        // Willekeurige delay tussen 1-5 seconden
        uint32_t random_delay_ms = 1000 + (esp_random() % 4000);
        vTaskDelay(random_delay_ms / portTICK_PERIOD_MS);
        
        // LED aan + timer starten
        printf("LED aan! Druk de knop in!\n");
        led_start_time = esp_timer_get_time();
        reaction_detected = 0;
        myGPIO_LED_On(LED_PIN_1);
        
        // Wachten tot knop wordt ingedrukt (timeout 5 seconden)
        int timeout_counter = 0;
        while (!reaction_detected && timeout_counter < 500)
        {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            timeout_counter++;
        }
        
        // LED uit
        myGPIO_LED_Off(LED_PIN_1);
        
        // Resultaat weergeven
        if (reaction_detected)
        {
            int reaction_time_ms = reaction_time_us / 1000;
            printf("✓ Reactietijd: %d ms\n\n", reaction_time_ms);
        }
        else
        {
            printf("✗ Timeout! Je was te traag.\n\n");
        }
        
        // Pauze voordat volgende ronde start
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
