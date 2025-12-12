#include <Arduino.h>
#include <Adafruit_SHT4x.h>
#include <SPI.h>
#include <lvgl.h>
#include "ui.h"
#include "weather.h"
#include "network_stuff.h"

#define BOOT

Adafruit_SHT4x sht41;

// data
sensors_event_t humid, temp;

// variables
unsigned long last_update = 0;

#define BUF_SIZE 320 * 50
uint8_t lv_buffer[BUF_SIZE];

void update_data();

/* Return the elapsed milliseconds since startup.
 * It needs to be implemented by the user */
uint32_t my_get_millis(void)
{
    return millis();
}

void setup() {
    Wire.begin(5, 6);
    Serial.begin(115200);
    delay(1000);
    Serial.println("hello");

    delay(1000);
    // setup sht41
    if (!sht41.begin(&Wire)) {
        Serial.println("Couldn't find SHT4x");
        while (1) delay(1);
    }

    lv_init();
    
    lv_tick_set_cb(my_get_millis);

    lv_lovyan_gfx_create(320, 240, lv_buffer, BUF_SIZE, true);
    
    ui_init();

    //  connect to wifi
    String ssid, password;

    if (!loadWifiConfig(ssid, password)) {
        startConfigPortal();
    }

    else {
        wifi_connect(ssid, password);
    }
}

void loop() {

    if (WiFi.getMode() == WIFI_AP) {
        server.handleClient();
    }

    if (millis() - last_update >= 1000) {
        update_data();
        update_ui(humid.relative_humidity, temp.temperature);
        last_update = millis();
        // get_update();
    }
    
    lv_timer_handler();
    delay(5);
}

void update_data() {
    sht41.getEvent(&humid, &temp);
}
