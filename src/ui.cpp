#include <Arduino.h>
#include "ui.h"


lv_obj_t *screen;
lv_obj_t *ui_humid;
lv_obj_t *ui_temp;

void create_and_assign_label(lv_obj_t*& label_ref, lv_obj_t* parent, const char* text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, const lv_font_t *font) {
    label_ref = lv_label_create(parent);
    lv_label_set_text(label_ref, text);
    lv_obj_align(label_ref, align, x_ofs, y_ofs);
    lv_obj_set_style_text_font(label_ref, font, 0);
    lv_obj_set_style_text_color(label_ref, lv_color_white(), 0);
}

void ui_init() {
    screen = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);

    create_and_assign_label(ui_humid, screen, "Humidity: --%", LV_ALIGN_TOP_LEFT, 50, 50, &lv_font_montserrat_20);
    create_and_assign_label(ui_temp, screen, "Temperature: --%", LV_ALIGN_TOP_LEFT, 50, 80, &lv_font_montserrat_20);

    lv_screen_load(screen);
}

void update_ui(float humid, float temp) {
    char buf[32];

    snprintf(buf, sizeof(buf), "Humidity: %.1f %", humid);
    lv_label_set_text(ui_humid, buf);

    snprintf(buf, sizeof(buf), "Temperature: %.1f C", temp);
    lv_label_set_text(ui_temp, buf);
}