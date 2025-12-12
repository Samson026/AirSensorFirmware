#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

extern WebServer server;

void handleRoot();
void handleSave();
void saveWifiConfig(String& ssid, String& pass);
void startConfigPortal();
bool loadWifiConfig(String& ssidOut, String& passOut);
void wifi_connect(String ssid, String pass);