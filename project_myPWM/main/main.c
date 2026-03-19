#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "components/myPWM.h"

#define CHANNEL ADC_CHANNEL_0

void app_main(void)
{
    myPWM_Initialize(18);
    
    //adc setting 
    int potWaarde = 0;
    static esp_adc_cal_characteristics_t *adc_chars;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(CHANNEL, ADC_ATTEN_DB_12);
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 1100, adc_chars);

    while(1)
    {
        //Potmeter inlezen
        potWaarde = esp_adc_cal_raw_to_voltage(adc1_get_raw((adc1_channel_t)CHANNEL), adc_chars);
        printf("Potentiometer waarde: %d mV\n", potWaarde);

        myPWM_SetDuty(potWaarde / 4); // Schaal de potentiometerwaarde naar een bereik van 0-255
    }

}