#include <Arduino.h>
#include "ui.h"

LV_FONT_DECLARE(notosans_20);
LV_FONT_DECLARE(notosans_18);

lv_obj_t *screen;

// values
lv_obj_t *ui_room_temp;
lv_obj_t *ui_humid;
lv_obj_t *ui_outside_temp;
lv_obj_t *ui_rainfall;
lv_obj_t *ui_voc;

static lv_obj_t* create_metric(
    lv_obj_t* parent,
    const char* label_text,
    lv_obj_t** value_label,
    lv_coord_t x,
    lv_coord_t y,
    lv_coord_t w,
    lv_coord_t h
) {
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, w, h);
    lv_obj_align(container, LV_ALIGN_TOP_LEFT, x, y);

    // subtle card look (still dark)
    lv_obj_set_style_bg_color(container, lv_color_make(18,18,18), 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(container, 10, 0);
    lv_obj_set_style_border_width(container, 1, 0);
    lv_obj_set_style_border_color(container, lv_color_make(40,40,40), 0);
    lv_obj_set_style_pad_all(container, 8, 0);

    lv_obj_t* label = lv_label_create(container);
    lv_label_set_text(label, label_text);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_color(label, lv_palette_lighten(LV_PALETTE_GREY, 2), 0);
    lv_obj_set_style_text_font(label, &notosans_18, 0);

    *value_label = lv_label_create(container);
    lv_label_set_text(*value_label, "--");
    lv_obj_align(*value_label, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_text_color(*value_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(*value_label, &lv_font_montserrat_22, 0);

    return container;
}

void ui_init() {
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);

    // Layout constants for 320x240
    const lv_coord_t pad = 10;
    const lv_coord_t title_h = 28;
    const lv_coord_t gap = 8;

    const lv_coord_t card_w = (320 - pad*2 - gap) / 2;        // ~146
    const lv_coord_t card_h = (240 - pad*2 - title_h - gap*2) / 3; // fits 3 rows

    // Title
    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "Tempatron");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, pad);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);

    lv_coord_t y0 = pad + title_h;
    lv_coord_t xL = pad;
    lv_coord_t xR = pad + card_w + gap;

    // Row 1
    create_metric(screen, "室温",   &ui_room_temp,   xL, y0 + (card_h + gap) * 0, card_w, card_h);
    create_metric(screen, "湿度",   &ui_humid,       xR, y0 + (card_h + gap) * 0, card_w, card_h);

    // Row 2
    create_metric(screen, "外気温", &ui_outside_temp,xL, y0 + (card_h + gap) * 1, card_w, card_h);
    create_metric(screen, "降水量", &ui_rainfall,    xR, y0 + (card_h + gap) * 1, card_w, card_h);

    // Row 3
    create_metric(screen, "VOC",    &ui_voc,         xL + (xL + xR) /2, y0 + (card_h + gap) * 2, card_w, card_h);

    lv_screen_load(screen);
}

void update_ui(float humid, float temp, float outside_temp, float rainfall) {
    char buf[16];

    snprintf(buf, sizeof(buf), "%.1f°C", temp);
    lv_label_set_text(ui_room_temp, buf);

    snprintf(buf, sizeof(buf), "%.1f%%", humid);
    lv_label_set_text(ui_humid, buf);

    snprintf(buf, sizeof(buf), "%.1f°C", outside_temp);
    lv_label_set_text(ui_outside_temp, buf);

    snprintf(buf, sizeof(buf), "%.1fmm", rainfall);
    lv_label_set_text(ui_rainfall, buf);

    // placeholder until logic exists
    lv_label_set_text(ui_voc, "--");
}
