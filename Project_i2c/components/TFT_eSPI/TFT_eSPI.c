#include "TFT_eSPI.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS    5
#define TFT_DC   16
#define TFT_RST  23

static spi_device_handle_t spi;

static void send_cmd(uint8_t cmd)
{
    gpio_set_level(TFT_DC, 0);

    spi_transaction_t t = {0};
    t.length = 8;
    t.tx_buffer = &cmd;

    spi_device_transmit(spi, &t);
}

static void send_data(const uint8_t *data, int len)
{
    gpio_set_level(TFT_DC, 1);

    spi_transaction_t t = {0};
    t.length = len * 8;
    t.tx_buffer = data;

    spi_device_transmit(spi, &t);
}

void tft_init(void)
{
    gpio_set_direction(TFT_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(TFT_RST, GPIO_MODE_OUTPUT);

    spi_bus_config_t buscfg = {
        .mosi_io_num = TFT_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = TFT_SCLK,
    };

    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 40000000,
        .mode = 0,
        .spics_io_num = TFT_CS,
        .queue_size = 7,
    };

    spi_bus_add_device(SPI2_HOST, &devcfg, &spi);

    // reset
    gpio_set_level(TFT_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));

    gpio_set_level(TFT_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    // software reset
    send_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(150));

    // sleep out
    send_cmd(0x11);
    vTaskDelay(pdMS_TO_TICKS(150));

    // display on
    send_cmd(0x29);
}

void tft_fill_screen(uint16_t color)
{
    // later invullen
}

void tft_draw_pixel(int x, int y, uint16_t color)
{
    // later invullen
}