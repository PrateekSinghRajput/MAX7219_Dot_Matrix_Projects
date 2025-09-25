

#include <Wire.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>
#include <DHT.h>

#define MAX72XX_CS_PIN 15
#define MAX72XX_NUM_DEVICES 8

MD_Parola disp = MD_Parola(MD_MAX72XX::FC16_HW, MAX72XX_CS_PIN, MAX72XX_NUM_DEVICES);

#define DHTPIN 14
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

float temp, hum;
String tempString, humString;
char message[60] = "";

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  disp.begin();
  dht.begin();

  disp.displayText("JustDoElectronics", PA_LEFT, 40, 40, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  while (!disp.displayAnimate())
    ;
  dispTemp();
  dispHum();
  disp.displayClear();
}

void dispTemp() {
  temp = dht.readTemperature(true);
  temp = (temp - 32) * 5 / 9;
  tempString = "Temp = " + String(temp) + "C";
  tempString.toCharArray(message, 30);
  disp.displayText(message, PA_LEFT, 50, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!disp.displayAnimate())
    ;
}

void dispHum() {
  hum = dht.readHumidity();
  humString = "Humi = " + String(hum) + "%";
  humString.toCharArray(message, 30);
  disp.displayText(message, PA_LEFT, 50, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!disp.displayAnimate())
    ;
}
