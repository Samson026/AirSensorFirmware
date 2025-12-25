#include <Arduino.h>
#include "ui.h"

LV_FONT_DECLARE(notosans_20);
LV_FONT_DECLARE(notosans_18);
const String AP_NETWORK_ID = "KAPUKAPU";

UIManager uiManager;


// helper: create a small stat cell (label + value)
static void create_stat(
    lv_obj_t* parent,
    const char* label_text,
    lv_obj_t** value_label,
    lv_coord_t x,
    lv_coord_t y,
    lv_coord_t w,
    const char* sign_text
) {
    // container just for positioning (no visible box)
    lv_obj_t* c = lv_obj_create(parent);
    lv_obj_set_size(c, w, 30);
    lv_obj_align(c, LV_ALIGN_TOP_LEFT, x, y);

    lv_obj_set_style_bg_opa(c, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(c, 0, 0);
    lv_obj_set_style_pad_all(c, 0, 0);

    lv_obj_t* label = lv_label_create(c);
    lv_label_set_text(label, label_text);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_color(label, lv_palette_lighten(LV_PALETTE_GREY, 2), 0);
    lv_obj_set_style_text_font(label, &notosans_18, 0);

    *value_label = lv_label_create(c);
    lv_label_set_text(*value_label, "----");
    lv_obj_align(*value_label, LV_ALIGN_LEFT_MID, 60, 0);
    lv_obj_set_style_text_color(*value_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(*value_label, &lv_font_montserrat_20, 0);

    lv_coord_t sign_x = lv_obj_get_width(*value_label);

    lv_obj_t* sign = lv_label_create(c);
     lv_label_set_text(sign, sign_text);
    lv_obj_align(sign, LV_ALIGN_LEFT_MID, 60+45, 0);
    lv_obj_set_style_text_color(sign, lv_palette_lighten(LV_PALETTE_GREY, 2), 0);
    lv_obj_set_style_text_font(sign, &lv_font_montserrat_14, 0);

}

void UIManager::ui_init_dashboard() {
    lv_obj_set_style_bg_color(screen_dashboard, lv_color_black(), 0);

    // === Top: City name ===
    ui_city = lv_label_create(screen_dashboard);
    lv_label_set_text(ui_city, "---");
    lv_obj_align(ui_city, LV_ALIGN_TOP_MID, 0, 8);
    lv_obj_set_style_text_font(ui_city, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(ui_city, lv_color_white(), 0);

    // === Hero: big room temp ===
    ui_hero_value = lv_label_create(screen_dashboard);
    lv_label_set_text(ui_hero_value, "--.-°C");
    lv_obj_align(ui_hero_value, LV_ALIGN_TOP_MID, 0, 48);
    lv_obj_set_style_text_font(ui_hero_value, &lv_font_montserrat_40, 0);
    lv_obj_set_style_text_color(ui_hero_value, lv_color_white(), 0);

    ui_hero_label = lv_label_create(screen_dashboard);
    lv_label_set_text(ui_hero_label, "室温");
    lv_obj_align_to(ui_hero_label, ui_hero_value, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
    lv_obj_set_style_text_font(ui_hero_label, &notosans_20, 0);
    lv_obj_set_style_text_color(ui_hero_label, lv_palette_lighten(LV_PALETTE_GREY, 2), 0);

    // === Bottom stats grid (2 columns x 2 rows) ===
    const lv_coord_t pad = 10;
    const lv_coord_t gap = 30;
    const lv_coord_t col_w = (320 - pad*2 - gap) / 2; // ~141
    const lv_coord_t row1_y = 140;
    const lv_coord_t row2_y = 178;

    const lv_coord_t xL = pad;
    const lv_coord_t xR = pad + col_w + gap;

    create_stat(screen_dashboard, "湿度",   &ui_humid,        xL, row1_y, col_w, "%");
    create_stat(screen_dashboard, "外気温", &ui_outside_temp, xR, row1_y, col_w, "°C");
    create_stat(screen_dashboard, "降水量", &ui_rainfall,     xL, row2_y, col_w, "mm");
    create_stat(screen_dashboard, "VOC",    &ui_voc,          xR, row2_y, col_w, "");
}

void UIManager::ui_init_wifi_connect() {
    lv_obj_set_style_bg_color(screen_wifi_connect, lv_color_black(), 0);

    lv_obj_t *wifi_text = lv_label_create(screen_wifi_connect);
    lv_label_set_text(wifi_text, "Connecting to WiFi...");
    lv_obj_align(wifi_text, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(wifi_text, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(wifi_text, lv_color_white(), 0);
}

void UIManager::ui_init_wifi_ap_mode() {
    lv_obj_set_style_bg_color(screen_wifi_ap, lv_color_black(), 0);

    lv_obj_t *ap_text = lv_label_create(screen_wifi_ap);
    lv_obj_t *ap_text_2 = lv_label_create(screen_wifi_ap);

    char buf[128];
    snprintf(buf, sizeof(buf), "Please connect to %s",
        AP_NETWORK_ID);
    lv_label_set_text(ap_text, buf);
    lv_obj_align(ap_text, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(ap_text, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(ap_text, lv_color_white(), 0);

    lv_label_set_text(ap_text_2, "open http://192.168.4.1/");
    lv_obj_align(ap_text_2, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_style_text_font(ap_text_2, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(ap_text_2, lv_color_white(), 0);
}

void UIManager::ui_init() {
    for (int i = 0; i < 3; i++) {
        screens[i] = lv_obj_create(NULL);
    }

    screen_dashboard = screens[UI_DASHBOARD];
    screen_wifi_connect = screens[UI_WIFI_CONNECT];
    screen_wifi_ap = screens[UI_WIFI_AP];

    ui_init_wifi_connect();
    ui_init_dashboard();
    ui_init_wifi_ap_mode();
}

// Updated signature suggestion (optional): add voc parameter later
void UIManager::update_ui(float humid, float temp, float voc, float outside_temp, float rainfall) {
    char buf[20];

    // Hero (room temp)
    snprintf(buf, sizeof(buf), "%.1f°C", temp);
    lv_label_set_text(ui_hero_value, buf);

    // Bottom stats
    snprintf(buf, sizeof(buf), "%.1f", humid);
    lv_label_set_text(ui_humid, buf);

    snprintf(buf, sizeof(buf), "%.1f", outside_temp);
    lv_label_set_text(ui_outside_temp, buf);

    snprintf(buf, sizeof(buf), "%.1f", rainfall);
    lv_label_set_text(ui_rainfall, buf);

    // placeholder until you add voc logic
    snprintf(buf, sizeof(buf), "%.1f", voc);
    lv_label_set_text(ui_voc, buf);
}

void UIManager::ui_load_page(UI_PAGE page) {
    lv_scr_load(screens[page]);
}

void UIManager::set_location(char* location) {
    lv_label_set_text(ui_city, location);
}
