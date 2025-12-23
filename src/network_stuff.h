#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

extern WebServer server;

void handleRoot();
void handleSave();
void saveWifiConfig(String& ssid, String& pass);
void startConfigPortal();
bool loadWifiConfig(String& ssidOut, String& passOut);
bool wifi_connect(String ssid, String pass);
String get_location();
String get_weather(float lat, float lon);