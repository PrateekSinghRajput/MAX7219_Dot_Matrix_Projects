#include <Wire.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>
#include <DHT.h>
#include <RTClib.h>

#define MAX72XX_CS_PIN 4
#define MAX72XX_NUM_DEVICES 8

MD_Parola disp = MD_Parola(MD_MAX72XX::FC16_HW, MAX72XX_CS_PIN, MAX72XX_NUM_DEVICES);

#define DHTPIN 14
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;

char message[60] = "";

enum DisplayState { SHOW_TEMP, SHOW_HUM, SHOW_DATE, SHOW_TIME };
DisplayState currentState = SHOW_TEMP;

void setup() {
  Serial.begin(9600);

  disp.begin();
  dht.begin();

  if (!rtc.begin()) {
    disp.displayText("RTC Error", PA_CENTER, 50, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    while (true);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time manually...");
   // rtc.adjust(DateTime(2025, 9, 25, 22, 50, 0));  
  }

  disp.displayText("JustDoElectronics", PA_LEFT, 40, 40, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  if (disp.displayAnimate()) {
    disp.displayReset();

    switch (currentState) {
      case SHOW_TEMP:
        dispTemp();
        currentState = SHOW_HUM;
        break;
      case SHOW_HUM:
        dispHum();
        currentState = SHOW_DATE;
        break;
      case SHOW_DATE:
        dispDate();
        currentState = SHOW_TIME;
        break;
      case SHOW_TIME:
        dispTime();
        currentState = SHOW_TEMP;
        break;
    }
  }
}

void dispTemp() {
  float temp = dht.readTemperature(false);
  if (isnan(temp)) {
    strcpy(message, "Temp Error");
  } else {
    sprintf(message, "Temp = %.1fC", temp);
  }
  disp.displayText(message, PA_LEFT, 50, 3000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void dispHum() {
  float hum = dht.readHumidity();
  if (isnan(hum)) {
    strcpy(message, "Hum Error");
  } else {
    sprintf(message, "Humi = %.1f%%", hum);
  }
  disp.displayText(message, PA_LEFT, 50, 3000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void dispDate() {
  DateTime now = rtc.now();
  sprintf(message, "%04d-%02d-%02d", now.year(), now.month(), now.day());
  disp.displayText(message, PA_LEFT, 50, 3000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void dispTime() {
  DateTime now = rtc.now();
  sprintf(message, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  disp.displayText(message, PA_LEFT, 50, 3000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}
