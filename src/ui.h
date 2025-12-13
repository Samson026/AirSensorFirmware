#pragma once
#include <lvgl.h>

void ui_init(char* location);
void update_ui(float humid, float temp, float outside_temp, float rainfall);
