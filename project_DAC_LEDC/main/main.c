#include <stdio.h>
#include "driver/ledc.h"

ledc_timer_config_t myconfig =
{
    .timer_num = LEDC_TIMER_0,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_13_BIT,
    .freq_hz = 50,
    .clk_cfg = LEDC_AUTO_CLK,
};

ledc_channel_config_t mychannel =
{
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .intr_type = LEDC_INTR_DISABLE,
    .gpio_num = 25,
    .duty = 0,
    .hpoint = 0,
};

void app_main(void)
{
    //setup

    ledc_timer_config(&myconfig);
    ledc_channel_config(&mychannel);

    //esp_err_t ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty)
    //esp_err_t ledc_update_duty(ledc_mode_t speed_mode, ledc_channel_t channel)

    
}
