#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_st7789.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "snake.h"

#define LCD_MOSI    19
#define LCD_SCLK    18
#define LCD_CS       5
#define LCD_DC      16
#define LCD_RST     23
#define LCD_BL       4
#define LCD_WIDTH  135
#define LCD_HEIGHT 240

#define BTN_UP      13
#define BTN_DOWN    12
#define BTN_LEFT    27
#define BTN_RIGHT   15


void app_main(void) {
    // Backlight
    gpio_set_direction(LCD_BL, GPIO_MODE_OUTPUT);
    gpio_set_level(LCD_BL, 1);

    // Knoppen
    gpio_config_t btn_cfg = {
        .pin_bit_mask = (1ULL << BTN_UP) | (1ULL << BTN_DOWN) |
                        (1ULL << BTN_LEFT) | (1ULL << BTN_RIGHT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&btn_cfg);

    // SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = LCD_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = LCD_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_WIDTH * LCD_HEIGHT * 2,
    };
    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

    // LCD IO
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_DC,
        .cs_gpio_num = LCD_CS,
        .pclk_hz = 27 * 1000 * 1000,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    esp_lcd_new_panel_io_spi(SPI2_HOST, &io_config, &io_handle);

    // ST7789 panel
    esp_lcd_panel_handle_t panel = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RST,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = 16,
    };
    esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel);

    esp_lcd_panel_reset(panel);
    esp_lcd_panel_init(panel);
    esp_lcd_panel_invert_color(panel, true);
    esp_lcd_panel_set_gap(panel, 52, 40);
    esp_lcd_panel_disp_on_off(panel, true);

    // Game
    snake_game(panel);
    draw_game_over(panel, 0);

}