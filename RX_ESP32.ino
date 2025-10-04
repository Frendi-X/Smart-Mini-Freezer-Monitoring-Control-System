#define BLYNK_TEMPLATE_ID "TMPL6izRVEytJ"
#define BLYNK_TEMPLATE_NAME "MINI FREEZER"
#define BLYNK_AUTH_TOKEN "k7T_Tl57v0ZZj1Ug-7tzTvnaG8xniYa4"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// ------ Konfigurasi WiFi ------
char ssid[] = "Android AP";
char pass[] = "12345678";

// ------ Variabel Data ------
float tempValue = 0.0;
String modeValue = "AUTO";
int fanValue = 0;

// ------ Serial ke Arduino ------
#define RX_Pin 18
#define TX_Pin 19

SoftwareSerial mySerial(RX_Pin, TX_Pin);
WidgetLCD lcdWidget(V3);   // LCD Widget di Blynk pakai Virtual Pin V3

// Buffer aman untuk serial
char serialBuffer[256];
uint8_t bufferIndex = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  readArduinoData();
}

// -------- Baca data dari Arduino --------
void readArduinoData() {
  while (mySerial.available()) {
    char c = mySerial.read();

    if (c == '\n') {
      serialBuffer[bufferIndex] = '\0';  // akhiri string
      parseData(serialBuffer);
      bufferIndex = 0;                   // reset buffer
    } else {
      if (bufferIndex < sizeof(serialBuffer) - 1) {
        serialBuffer[bufferIndex++] = c;
      } else {
        bufferIndex = 0; // reset kalau overflow
      }
    }
  }
}

// -------- Parse JSON dari Arduino --------
void parseData(const char *data) {
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, data);

  if (error) {
    Serial.print("JSON Error: ");
    Serial.println(error.c_str());
    return;
  }

  tempValue = doc["temp"];
  modeValue = (const char*)doc["mode"];
  fanValue  = doc["fan"];

  Serial.print("Temp: "); Serial.println(tempValue);
  Serial.print("Mode: "); Serial.println(modeValue);
  Serial.print("Fan : "); Serial.println(fanValue);

  // --- Kirim ke Blynk Widgets ---
  Blynk.virtualWrite(V0, tempValue);          // Gauge / Value Display
  Blynk.virtualWrite(V1, modeValue);          // Label untuk mode
  Blynk.virtualWrite(V2, fanValue ? 1 : 0);   // LED Widget kipas

  // --- LCD Widget ---
  lcdWidget.clear();
  lcdWidget.print(0, 0, "Mode: " + modeValue + "    ");

  char buf[32];
  snprintf(buf, sizeof(buf), "Temp: %.1f C %s",
           tempValue,
           fanValue ? "ON " : "OFF");
  lcdWidget.print(0, 1, buf);
}
