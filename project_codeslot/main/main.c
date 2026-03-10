#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "myPWM.h"
#include "myGPIO.h"


#define CHANNEL ADC_CHANNEL_5
#define LED_PIN 2
#define KNOP_PIN 32
int waarde = 0;


void app_main(void)
{
    //myGPIO_KNOP_Setup(KNOP_PIN);
    myGPIO_LED_Setup(LED_PIN);
    myPWM_Initialize(18);

    static esp_adc_cal_characteristics_t *adc_chars;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(CHANNEL, ADC_ATTEN_DB_12);
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 1100, adc_chars);
    
    int i = 0;
    int code = 0;

    while (1)
    {
        int potWaarde = esp_adc_cal_raw_to_voltage(adc1_get_raw((adc1_channel_t)CHANNEL), adc_chars);

        if (140 < potWaarde && potWaarde < 442) {
            waarde = 1;
        }
        else if (442 < potWaarde && potWaarde < 784) {
            waarde = 2;
        }
       else if (784 < potWaarde && potWaarde < 1026) {
            waarde = 3;
        }
        else if (1026 < potWaarde && potWaarde < 1368) {
            waarde = 4;
        }
        else if (1368 < potWaarde && potWaarde < 1610) {
            waarde = 5;
        }
        else if (1610 < potWaarde && potWaarde < 1952) {
            waarde = 6;
        }
        else if (1952 < potWaarde && potWaarde < 2294) {
            waarde = 7;
        }
        else if (2294 < potWaarde && potWaarde < 2536) {
            waarde = 8;
        }
        else if (2536 < potWaarde && potWaarde < 2895) {
            waarde = 9;
        }
        else if (potWaarde > 2895) {
            waarde = 0;
        }
        
        if (KNOP_PIN == 1){
            i++;
            int j = waarde;
            
            if (i == 1){
                code += j * 1000;
            }
            else if (i == 2){
                code += j * 100;
            }
            else if (i == 3){
                code += j * 10;
            }
            else if (i == 4){
                code += j;
                i = 0;
            }
        }

        if (code == 1234){
            myGPIO_LED_On(LED_PIN);
            vTaskDelay(1000);
            myGPIO_LED_Off(LED_PIN);
        }
        else{
            myGPIO_LED_Off(LED_PIN);
        }

        

        printf("Potentiometer waarde: %d mV\n", potWaarde);
        printf("Waarde: %d\n", waarde);
        printf("code: %d\n", code);
        vTaskDelay(100);    
    }
}

