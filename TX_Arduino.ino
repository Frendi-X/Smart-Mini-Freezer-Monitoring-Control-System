#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// -------- PIN SETUP --------
#define ONE_WIRE_BUS 4
#define BTN_INC 7
#define BTN_DEC 6
#define FAN_PIN 9
#define BUZZER_PIN 5
#define RX_PIN 255
#define TX_PIN 8

// -------- OBJECTS --------
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial sendESP(RX_PIN, TX_PIN);

// -------- VARS --------
bool modeSimulation = false;
float sensorTemp = 0.0;
float displayedTemp = 0.0;

const float tempOffset = -28.0;
const float SIM_MIN = -20.0;
const float SIM_MAX = 10.0;
const float AUTO_MAX = 10.0;

unsigned long incPressTime = 0;
unsigned long decPressTime = 0;
bool incWasPressed = false;
bool decWasPressed = false;
const unsigned long LONG_PRESS = 5000; // 5 detik

unsigned long lastBuzz = 0;
bool buzzerState = false;

unsigned long lastLCD = 0;
unsigned long lastSend = 0;
unsigned long lastSensorReq = 0;

unsigned long beepStart = 0;
bool beepActive = false;

void setup() {
  Serial.begin(9600); // komunikasi ke ESP32
  sendESP.begin(9600);
  pinMode(BTN_INC, INPUT_PULLUP);
  pinMode(BTN_DEC, INPUT_PULLUP);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(FAN_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  sensors.begin();
  sensors.setWaitForConversion(false); // non-blocking mode

  lcd.init();
  lcd.backlight();
}

void loop() {
  unsigned long now = millis();

  // -------- SENSOR NON-BLOCKING --------
  if (now - lastSensorReq > 200) {
    sensors.requestTemperatures();
    sensorTemp = sensors.getTempCByIndex(0);
    sensorTemp = sensorTemp + tempOffset;
    lastSensorReq = now;
  }

  // -------- HANDLE BUTTON --------
  handleButton(BTN_INC, incWasPressed, incPressTime, true, now);
  handleButton(BTN_DEC, decWasPressed, decPressTime, false, now);

  // -------- SUHU DISPLAY --------
  if (modeSimulation) {
    if (displayedTemp < SIM_MIN) displayedTemp = SIM_MIN;
    if (displayedTemp > SIM_MAX) displayedTemp = SIM_MAX;
  } else {
    displayedTemp = sensorTemp;
    if (displayedTemp > AUTO_MAX) displayedTemp = AUTO_MAX;
  }

  // -------- FAN & BUZZER --------
  bool fanOn = false;
  if (displayedTemp > 8) {
    digitalWrite(FAN_PIN, LOW);
    handleBuzzer(now);
    fanOn = true;
  } else {
    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    buzzerState = false;
  }

  // -------- LCD UPDATE (setiap 100ms, no clear) --------
  if (now - lastLCD > 100) {
    // Baris 0 -> Mode
    lcd.setCursor(0, 0);
    lcd.print(modeSimulation ? "MODE: SIMULASI " : "MODE: AUTO     ");

    // Baris 1 -> Temp
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    if (isnan(displayedTemp)) {
      lcd.print("--.-");
    } else {
      lcd.print(displayedTemp, 1); // tampilkan 1 angka di belakang koma
    }
    lcd.print((char)223); // simbol derajat °
    lcd.print("C ");
    lcd.print(fanOn ? "ON " : "OFF");

    lastLCD = now;
  }


  // -------- KIRIM DATA SERIAL --------
  if (now - lastSend > 200) {
    sendDataToESP32(displayedTemp, modeSimulation, fanOn);
    lastSend = now;
  }

  // -------- HANDLE BEEP MODE SWITCH --------
  if (beepActive && now - beepStart > 150) {
    digitalWrite(BUZZER_PIN, LOW);
    beepActive = false;
  }
}

void handleButton(int pin, bool &wasPressed, unsigned long &pressTime, bool inc, unsigned long now) {
  bool pressed = (digitalRead(pin) == LOW);

  if (pressed && !wasPressed) {
    wasPressed = true;
    pressTime = now;
  } else if (!pressed && wasPressed) {
    unsigned long dur = now - pressTime;
    if (dur >= LONG_PRESS) {
      modeSimulation = !modeSimulation;
      if (modeSimulation) displayedTemp = sensorTemp;
      beepOnce(now); // beep non-blocking
    } else {
      if (modeSimulation) {
        if (inc) displayedTemp += 0.2;
        else displayedTemp -= 0.2;
      }
    }
    wasPressed = false;
  }
}

void handleBuzzer(unsigned long now) {
  if (now - lastBuzz >= 100) {
    buzzerState = !buzzerState;
    digitalWrite(BUZZER_PIN, buzzerState);
    lastBuzz = now;
  }
}

// --- fungsi beep sekali non-blocking ---
void beepOnce(unsigned long now) {
  digitalWrite(BUZZER_PIN, HIGH);
  beepStart = now;
  beepActive = true;
}

// --- fungsi kirim data ---
void sendDataToESP32(float temp, bool sim, bool fan) {
  sendESP.print("{\"temp\":");
  sendESP.print(temp, 1);
  sendESP.print(",\"mode\":\"");
  sendESP.print(sim ? "SIMULASI" : "AUTO");
  sendESP.print("\",\"fan\":");
  sendESP.print(fan ? 1 : 0);
  sendESP.println("}");
}
