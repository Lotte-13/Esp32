#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"

// Change these GPIO numbers to match your board wiring.
static const gpio_num_t led_pins[5] = {
    GPIO_NUM_2,
    GPIO_NUM_4,
    GPIO_NUM_5,
    GPIO_NUM_18,
    GPIO_NUM_19,
};

static void set_all_leds(bool level)
{
    for (int i = 0; i < 5; ++i) {
        gpio_set_level(led_pins[i], level ? 1 : 0);
    }
}

static void init_led_pins(void)
{
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 0,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    for (int i = 0; i < 5; ++i) {
        io_conf.pin_bit_mask |= (1ULL << led_pins[i]);
    }
    gpio_config(&io_conf);
    set_all_leds(false);
}

static uint32_t random_delay_ms(uint32_t min_ms, uint32_t max_ms)
{
    uint32_t r = esp_random();
    return min_ms + (r % (max_ms - min_ms + 1));
}

void app_main(void)
{
    init_led_pins();

    while (true) {
        for (int i = 0; i < 5; ++i) {
            gpio_set_level(led_pins[i], 1);
            vTaskDelay(pdMS_TO_TICKS(200));
        }

        // After all LEDs are on, wait a random time before turning them off together.
        vTaskDelay(pdMS_TO_TICKS(random_delay_ms(200, 1000)));
        set_all_leds(false);

        // Wait a short random time before starting the next sequence.
        vTaskDelay(pdMS_TO_TICKS(random_delay_ms(300, 800)));
    }
}
