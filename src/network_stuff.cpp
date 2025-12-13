#include <ArduinoJson.h>
#include "network_stuff.h"

WebServer server(80);
const String AP_NETWORK_ID = "KAPUKAPU";
Preferences prefs;



void handleRoot() {
  String page =
    "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width,initial-scale=1' />"
    "<title>WiFi Setup</title></head><body>"
    "<h2>WiFi Setup</h2>"
    "<form action='/save' method='POST'>"
    "SSID:<br><input name='ssid'><br><br>"
    "Password:<br><input name='pass' type='password'><br><br>"
    "<input type='submit' value='Save'>"
    "</form>"
    "</body></html>";
  server.send(200, "text/html", page);
}

void handleSave() {
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");

  if (ssid.length() == 0) {
    server.send(400, "text/plain", "SSID cannot be empty");
    return;
  }

  saveWifiConfig(ssid, pass);

  server.send(200, "text/plain", "Saved! Rebooting and connecting to WiFi...");
  delay(1000);
  ESP.restart();
}

void startConfigPortal() {
  Serial.println("Starting config portal...");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_NETWORK_ID, "");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);      // usually 192.168.4.1

  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();

  Serial.println("Config portal started. Connect to:");
  Serial.print("  SSID: "); Serial.println(AP_NETWORK_ID);
  Serial.println("Then open http://192.168.4.1/ in a browser");
}

void saveWifiConfig(String& ssid, String& pass) {
  prefs.begin("wifi", false);
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.end();
}

bool loadWifiConfig(String& ssidOut, String& passOut) {
  prefs.begin("wifi", true);
  String ssid = prefs.getString("ssid", "");
  String pass = prefs.getString("pass", "");
  prefs.end();

  if (ssid.length() == 0) return false;
  ssidOut = ssid;
  passOut = pass;
  return true;
}

void wifi_connect(String ssid, String pass) {
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("connecting to WiFi...");
        delay(1000);
    }

    Serial.println("WiFi connected");
}

String get_location() {
  HTTPClient http;
  String ip = "";

  http.begin("http://api.ipify.org");
  int code = http.GET();

  if (code > 0) {
    Serial.print("HTTP ");
    Serial.println(code);
    ip = http.getString();
  } else {
    Serial.println("POST failed");
  }

  http.end();

  String url = "http://ip-api.com/json/" + ip;
  String resp = "";
  http.begin(url);

  code = http.GET();

  if (code > 0) {
    Serial.print("Location HTTP ");
    Serial.println(code);
    resp = http.getString();
  } else {
    Serial.println("POST failed");
  }

  return (resp);
}

String get_weather(float lat, float lon) {
  HTTPClient http;
  String url = "https://api.open-meteo.com/v1/forecast?";
  url += "latitude=" + String(lat, 6);
  url += "&longitude=" + String(lon, 6);
  url += "&current_weather=true";
  url += "&daily=temperature_2m_max,temperature_2m_min";
  url += "&daily=precipitation_sum";

  http.begin(url);

  int code = http.GET();

  if (code > 0) {
    Serial.print("Weather HTTP ");
    Serial.println(code);
  }

  String resp = http.getString();
  return resp;
}

