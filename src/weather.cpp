#include "weather.h"

const char* ssid = "NetComm 2601";
const char* password = "Dunulurobe";

const char* URL = "https://api.open-meteo.com/v1/forecast?latitude=-33.8688&longitude=151.2093&daily=temperature_2m_max,temperature_2m_min,precipitation_probability_max,cloudcover_mean&timezone=auto";

void wifi_connect() {
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("connecting to wifi...");
        delay(1000);
    }
}

void get_update() {
    HTTPClient http;

    http.begin(URL);

    int code = http.GET();

    if (code > 0) {
        String body = http.getString();
        Serial.println(body);
    } else {
        Serial.println("Request failed");
    }

    http.end();
}