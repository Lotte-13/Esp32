#pragma once
#include "esp_lcd_panel_ops.h"

void snake_game(esp_lcd_panel_handle_t panel);
void draw_game_over(esp_lcd_panel_handle_t panel, int score);