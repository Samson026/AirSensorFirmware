#pragma once
#include <lvgl.h>

enum UI_PAGE {
    UI_WIFI_CONNECT = 0,
    UI_DASHBOARD = 1,
    UI_WIFI_AP = 2
};

class UIManager {
    public:
        UI_PAGE currentPage;

        void ui_init();
        void update_ui(float humid, float temp, float voc, float outside_temp, float rainfall);
        void ui_load_page(UI_PAGE page);
        void set_location(char* location);
    
    private:
        // top
        lv_obj_t *ui_city;
        
        //hero
        lv_obj_t *ui_hero_value;
        lv_obj_t *ui_hero_label;

        // small stats (bottom grid)
        lv_obj_t *ui_humid;
        lv_obj_t *ui_outside_temp;
        lv_obj_t *ui_rainfall;
        lv_obj_t *ui_voc;

        // screens
        lv_obj_t *screen_dashboard;
        lv_obj_t *screen_wifi_connect;
        lv_obj_t *screen_wifi_ap;
        lv_obj_t *screens[3];


        void ui_init_dashboard();
        void ui_init_wifi_connect();
        void ui_init_wifi_ap_mode();
};

extern UIManager uiManager;




