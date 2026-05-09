#include <WiFi.h>
#include <ESPAsyncWebServer.h>  // ESP32Async/ESPAsyncWebServer
#include <AsyncTCP.h>            // ESP32Async/AsyncTCP
#include <WiFiManager.h>
#include <Adafruit_INA219.h>
#include <U8g2lib.h>
#include <ElegantOTA.h>
#include "index_html.h"

#define OLED_ADDR 0x3C
#define INA1_ADDR 0x41
#define INA2_ADDR 0x44

U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);
Adafruit_INA219 inaTypeC(INA1_ADDR);
Adafruit_INA219 inaUsb(INA2_ADDR);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
WiFiManager wm;

unsigned long bootMs;
bool apMonitorEnabled = false;

struct MeterData { float v; float a; float w; };

MeterData readIna(Adafruit_INA219 &ina) {
  MeterData d;
  d.v = ina.getBusVoltage_V();
  d.a = ina.getCurrent_mA() / 1000.0f;
  d.w = ina.getPower_mW() / 1000.0f;
  return d;
}

void drawBoot() {
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(0, 12, "USB Charger");
  oled.drawStr(0, 24, "by E-gadget Iasi");
  String wifi = WiFi.isConnected() ? "WiFi: connected" : "WiFi: setup mode";
  oled.drawStr(0, 40, wifi.c_str());
  String ip = WiFi.isConnected() ? WiFi.localIP().toString() : WiFi.softAPIP().toString();
  oled.drawStr(0, 54, ip.c_str());
  oled.sendBuffer();
}

void drawLive(MeterData c1, MeterData c2) {
  char line[32];
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(0, 10, "USB Charger Monitor");
  snprintf(line, sizeof(line), "C: %.2fV %.2fA", c1.v, c1.a);
  oled.drawStr(0, 24, line);
  snprintf(line, sizeof(line), "C: %.2fW", c1.w);
  oled.drawStr(0, 34, line);
  snprintf(line, sizeof(line), "U: %.2fV %.2fA", c2.v, c2.a);
  oled.drawStr(0, 48, line);
  snprintf(line, sizeof(line), "U: %.2fW", c2.w);
  oled.drawStr(0, 60, line);
  oled.sendBuffer();
}

void broadcastData(MeterData c1, MeterData c2) {
  String payload = "{\"c1_v\":" + String(c1.v, 3) + ",\"c1_a\":" + String(c1.a, 3) + ",\"c1_w\":" + String(c1.w, 3) +
                   ",\"c2_v\":" + String(c2.v, 3) + ",\"c2_a\":" + String(c2.a, 3) + ",\"c2_w\":" + String(c2.w, 3) + "}";
  ws.textAll(payload);
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  (void)server;
  (void)client;
  (void)type;
  (void)arg;
  (void)data;
  (void)len;
}

void setupWeb() {
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", INDEX_HTML); });
  ElegantOTA.begin(&server);
  server.begin();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  oled.begin();

  inaTypeC.begin();
  inaUsb.begin();
  // Use a wider current range suitable for typical USB charging loads.
  inaTypeC.setCalibration_32V_2A();
  inaUsb.setCalibration_32V_2A();

  bootMs = millis();
  wm.setConfigPortalTimeout(180);
  wm.autoConnect("ChargerMonitor-Setup");

  drawBoot();
  setupWeb();
}

void loop() {
  MeterData c1 = readIna(inaTypeC);
  MeterData c2 = readIna(inaUsb);
  drawLive(c1, c2);
  broadcastData(c1, c2);

  if (!apMonitorEnabled && WiFi.status() != WL_CONNECTED && millis() - bootMs > 180000UL) {
    WiFi.softAP("ChargerMonitor-AP");
    apMonitorEnabled = true;
  }

  ws.cleanupClients();
  delay(80);
}
