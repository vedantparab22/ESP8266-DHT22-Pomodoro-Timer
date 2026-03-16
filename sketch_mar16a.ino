#include <U8g2lib.h>
#include <SPI.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define DHTPIN D2
#define DHTTYPE DHT22
#define TOUCH_PIN D1

DHT dht(DHTPIN, DHTTYPE);

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D8, /* dc=*/ D3, /* reset=*/ D4);

// WIFI
const char* ssid = "RAUT_5G";
const char* password = "9821212153";

// API
String apiKey = "34336b98569d4f4c37af1ca177f910d2";
String city = "Mumbai";

float apiTemp = 0;
float apiHum = 0;

void setup() {

  pinMode(TOUCH_PIN, INPUT);

  dht.begin();
  u8g2.begin();

  WiFi.begin(ssid, password);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,20,"Connecting WiFi...");
  u8g2.sendBuffer();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

}

void getWeather()
{
  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;

    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

    http.begin(client, url);

    int httpCode = http.GET();

    if (httpCode > 0) {

      String payload = http.getString();

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      apiTemp = doc["main"]["temp"];
      apiHum = doc["main"]["humidity"];

    }

    http.end();
  }
}

void loop() {

  int touchState = digitalRead(TOUCH_PIN);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  if (touchState == HIGH) {

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    u8g2.drawStr(0,15,"DHT22 SENSOR");

    char tempStr[20];
    sprintf(tempStr,"Temp: %.1f C", temperature);
    u8g2.drawStr(0,35,tempStr);

    char humStr[20];
    sprintf(humStr,"Hum: %.1f %%", humidity);
    u8g2.drawStr(0,50,humStr);

  }
  else {

    getWeather();

    u8g2.drawStr(0,15,"MUMBAI WEATHER");

    char tempStr[20];
    sprintf(tempStr,"Temp: %.1f C", apiTemp);
    u8g2.drawStr(0,35,tempStr);

    char humStr[20];
    sprintf(humStr,"Hum: %.1f %%", apiHum);
    u8g2.drawStr(0,50,humStr);

  }

  u8g2.sendBuffer();

  delay(5000);
}