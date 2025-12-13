#include <Arduino.h>
#include <Adafruit_SHT4x.h>
#include <SPI.h>
#include <lvgl.h>
#include <ArduinoJson.h>
#include "ui.h"
#include "weather.h"
#include "network_stuff.h"

#define BOOT

Adafruit_SHT4x sht41;

// consts
const unsigned long room_refresh = 1000;
const unsigned long weather_refresh = 360000;

// data
sensors_event_t humid, temp;
StaticJsonDocument<256> locationDoc;
StaticJsonDocument<4096> weatherDoc;
float lat;
float lon;

// variables
unsigned long last_update_room = 0;
unsigned long last_update_weather = 0;

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

    //  connect to wifi
    String ssid, password;

    if (!loadWifiConfig(ssid, password)) {
        startConfigPortal();
    }

    else {
        wifi_connect(ssid, password);
    }
    String resp = get_location();

    deserializeJson(locationDoc, resp);

    resp = get_weather(locationDoc["lat"], locationDoc["lon"]);

    deserializeJson(weatherDoc, resp);

    const char* city = locationDoc["city"].as<const char*>();
    char buf[16];
    strncpy(buf, city, sizeof(buf));

    ui_init(buf);
}

void loop() {

    if (WiFi.getMode() == WIFI_AP) {
        server.handleClient();
    }

    // update room data and UI
    if (millis() - last_update_room >= room_refresh) {
        update_data();
        float outTemp = weatherDoc["current_weather"]["temperature"].as<float>();
        float rainfall = weatherDoc["daily"]["precipitation_sum"][0].as<float>();
        update_ui(humid.relative_humidity, temp.temperature, outTemp, rainfall);
        last_update_room = millis();
    }

    // update weather data
    if (millis() - last_update_weather >= weather_refresh) {
        String resp = get_weather(locationDoc["lat"], locationDoc["lon"]);
        deserializeJson(weatherDoc, resp);
        last_update_weather = millis();
    }

    
    lv_timer_handler();
    delay(5);
}

void update_data() {
    sht41.getEvent(&humid, &temp);
}
