#include "snake.h"
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"
#include "driver/gpio.h"

#define BTN_UP      13
#define BTN_DOWN    12
#define BTN_LEFT    27
#define BTN_RIGHT   15

#define LCD_WIDTH  135
#define LCD_HEIGHT 240

#define CELL_SIZE   10
#define GRID_W     (LCD_WIDTH  / CELL_SIZE)
#define GRID_H     (LCD_HEIGHT / CELL_SIZE)
#define MAX_SNAKE  (GRID_W * GRID_H)

#define COLOR_BG     0xFFFF //wit
#define COLOR_SNAKE  0xE007 //groen
#define COLOR_FOOD   0x00F8 //rood

typedef struct { int x, y; } Point;
typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

static void draw_cell(esp_lcd_panel_handle_t panel, int x, int y, uint16_t color) {
    uint16_t buf[CELL_SIZE * CELL_SIZE];
    for (int i = 0; i < CELL_SIZE * CELL_SIZE; i++) buf[i] = color;
    esp_lcd_panel_draw_bitmap(panel,
        x * CELL_SIZE, y * CELL_SIZE,
        x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE + CELL_SIZE,
        buf);
}

static void fill_screen(esp_lcd_panel_handle_t panel, uint16_t color) {
    uint16_t *buf = malloc(LCD_WIDTH * LCD_HEIGHT * 2);
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) buf[i] = color;
    esp_lcd_panel_draw_bitmap(panel, 0, 0, LCD_WIDTH, LCD_HEIGHT, buf);
    free(buf);
}

static const uint8_t font5x7[][5] = {
    [' '] = {0x00,0x00,0x00,0x00,0x00},
    ['0'] = {0x3E,0x51,0x49,0x45,0x3E},
    ['1'] = {0x00,0x42,0x7F,0x40,0x00},
    ['2'] = {0x42,0x61,0x51,0x49,0x46},
    ['3'] = {0x21,0x41,0x45,0x4B,0x31},
    ['4'] = {0x18,0x14,0x12,0x7F,0x10},
    ['5'] = {0x27,0x45,0x45,0x45,0x39},
    ['6'] = {0x3C,0x4A,0x49,0x49,0x30},
    ['7'] = {0x01,0x71,0x09,0x05,0x03},
    ['8'] = {0x36,0x49,0x49,0x49,0x36},
    ['9'] = {0x06,0x49,0x49,0x29,0x1E},
    ['G'] = {0x3E,0x41,0x49,0x49,0x3A},
    ['A'] = {0x7E,0x09,0x09,0x09,0x7E},
    ['M'] = {0x7F,0x02,0x0C,0x02,0x7F},
    ['E'] = {0x7F,0x49,0x49,0x49,0x41},
    ['O'] = {0x3E,0x41,0x41,0x41,0x3E},
    ['V'] = {0x60,0x18,0x07,0x18,0x60},
    ['R'] = {0x7F,0x09,0x19,0x29,0x46},
};

static void draw_char_big(esp_lcd_panel_handle_t panel, int x, int y, char c, uint16_t color, uint16_t bg, int scale) {
    const uint8_t *bitmap = font5x7[(uint8_t)c];
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 7; row++) {
            uint16_t pixel = (bitmap[col] >> row) & 1 ? color : bg;
            uint16_t buf[scale * scale];
            for (int i = 0; i < scale * scale; i++) buf[i] = pixel;
            esp_lcd_panel_draw_bitmap(panel,
                x + col * scale, y + row * scale,
                x + col * scale + scale, y + row * scale + scale,
                buf);
        }
    }
}

void draw_game_over(esp_lcd_panel_handle_t panel, int score) {
    fill_screen(panel, COLOR_BG);

    int scale = 3;
    int char_w = (5 + 1) * scale;
    int char_h = 7 * scale;

    char score_str[12];
    snprintf(score_str, sizeof(score_str), "%d", score);

    const char *strings[] = {"GAME", "OVER", score_str};
    int num_strings = 3;

    // Bereken totale hoogte om te centreren
    int total_h = num_strings * char_h + (num_strings - 1) * 5;
    int start_y = (LCD_HEIGHT + total_h) / 2;

    for (int si = 0; si < num_strings; si++) {
        const char *str = strings[si];
        int slen = strlen(str);
        int str_w = slen * char_w;
        int str_x = (LCD_WIDTH - str_w) / 2;
        int str_y = start_y - si * (char_h + 5);

        for (int s = 0; s < slen; s++) {
            uint8_t c = (uint8_t)str[s];
            if (c >= sizeof(font5x7) / sizeof(font5x7[0])) continue;
            const uint8_t *bitmap = font5x7[c];
            for (int col = 0; col < 5; col++) {
                for (int row = 0; row < 7; row++) {
                    uint16_t pixel = (bitmap[col] >> row) & 1 ? COLOR_FOOD : COLOR_BG;
                    uint16_t buf[scale * scale];
                    for (int i = 0; i < scale * scale; i++) buf[i] = pixel;
                    // 180° rotatie
                    int rx = str_x + str_w - (s * char_w + col * scale) - scale;
                    int ry = str_y - row * scale - scale;
                    if (rx >= 0 && rx + scale <= LCD_WIDTH &&
                        ry >= 0 && ry + scale <= LCD_HEIGHT)
                        esp_lcd_panel_draw_bitmap(panel, rx, ry, rx + scale, ry + scale, buf);
                }
            }
        }
    }
}

void snake_game(esp_lcd_panel_handle_t panel) {
    Point snake[MAX_SNAKE];
    int length = 3;
    Direction dir = DIR_RIGHT;
    Direction next_dir = DIR_RIGHT;
    Point food;
    int score = 0;

    for (int i = 0; i < length; i++) {
        snake[i].x = GRID_W / 2 - i;
        snake[i].y = GRID_H / 2;
    }

    srand(esp_random());
    food.x = rand() % GRID_W;
    food.y = rand() % GRID_H;

    while (1) {
        if (gpio_get_level(BTN_UP) == 0    && dir != DIR_DOWN)  next_dir = DIR_UP;
        if (gpio_get_level(BTN_DOWN) == 0  && dir != DIR_UP)    next_dir = DIR_DOWN;
        if (gpio_get_level(BTN_LEFT) == 0  && dir != DIR_RIGHT) next_dir = DIR_LEFT;
        if (gpio_get_level(BTN_RIGHT) == 0 && dir != DIR_LEFT)  next_dir = DIR_RIGHT;
        dir = next_dir;

        Point new_head = snake[0];
        if (dir == DIR_UP)    new_head.y--;
        if (dir == DIR_DOWN)  new_head.y++;
        if (dir == DIR_LEFT)  new_head.x--;
        if (dir == DIR_RIGHT) new_head.x++;

        if (new_head.x < 0 || new_head.x >= GRID_W ||
            new_head.y < 0 || new_head.y >= GRID_H) break;

        for (int i = 0; i < length; i++)
            if (snake[i].x == new_head.x && snake[i].y == new_head.y) goto game_over;

        bool ate = (new_head.x == food.x && new_head.y == food.y);

        // Verschuif
        for (int i = length - 1; i > 0; i--) snake[i] = snake[i - 1];
        snake[0] = new_head;

        if (ate) {
            length++;
            score++;
            food.x = rand() % GRID_W;
            food.y = rand() % GRID_H;
        }

        // Wis scherm en teken alles opnieuw
        fill_screen(panel, COLOR_BG);
        draw_cell(panel, food.x, food.y, COLOR_FOOD);
        for (int i = 0; i < length; i++)
            draw_cell(panel, snake[i].x, snake[i].y, COLOR_SNAKE);

        int delay = 400 - (score * 5);
        if (delay < 150) delay = 150;
        vTaskDelay(pdMS_TO_TICKS(delay));
    }

game_over:
    draw_game_over(panel, score);
    vTaskDelay(pdMS_TO_TICKS(3000));
}