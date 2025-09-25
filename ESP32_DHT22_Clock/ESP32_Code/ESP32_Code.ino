#include <Wire.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>
#include <DHT.h>
#include <WiFi.h>
#include "time.h"


const char* ssid = "Prateek";
const char* password = "12345@#12345";

// NTP server parameters
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

#define MAX72XX_CS_PIN 25
#define MAX72XX_NUM_DEVICES 4

MD_Parola disp = MD_Parola(MD_MAX72XX::FC16_HW, MAX72XX_CS_PIN, MAX72XX_NUM_DEVICES);

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float temp, hum;
String tempString, humString;
char message[60] = "";

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  disp.begin();
  disp.displayText("NT TRONIX", PA_LEFT, 40, 40, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  while (!disp.displayAnimate())
    ;

  dispTemp();
  dispHum();
  dispDateTime();

  disp.displayClear();
}

void dispTemp() {
  temp = dht.readTemperature(true);
  temp = (temp - 32) * 5 / 9;
  tempString = "Temp=" + String(temp, 1) + "C";
  tempString.toCharArray(message, 30);
  disp.displayText(message, PA_LEFT, 50, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!disp.displayAnimate())
    ;
}

void dispHum() {
  hum = dht.readHumidity();
  humString = "Hum=" + String(hum, 1) + "%";
  humString.toCharArray(message, 30);
  disp.displayText(message, PA_LEFT, 50, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!disp.displayAnimate())
    ;
}

void dispDateTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  char timeStr[20];
  // Format: YYYY-MM-DD HH:MM:SS
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);

  String dtString = String(timeStr);
  dtString.toCharArray(message, 60);
  disp.displayText(message, PA_LEFT, 50, 4000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!disp.displayAnimate())
    ;
}
