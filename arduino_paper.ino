#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#include "secrets.h"

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=15*/ 13, /*DC=4*/ 12, /*RST=2*/ 4, /*BUSY=5*/ 2));

void setup() {
  display.init();
  Serial.begin(115200);
  wifiConnect();
}

const char* ssid = SSID;
const char* password = PASSWORD;
const char* url = TEMP_URL;

// delay 20 min
const unsigned long postingInterval = 20L * 60L * 1000L; // delay between updates, in milliseconds
unsigned long lastConnectionTime = -postingInterval;

void loop() {
  Serial.println("Starting request");

  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  } else {
    delay(5 * 60 * 1000L); // wait 5 min and try again
  }
}

void wifiConnect() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void writeMessage(const char message[]) {
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(message, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(message);
  }
  while (display.nextPage());
  display.hibernate();
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    HTTPClient http;

    http.begin(url, null); //Specify the URL
    int httpCode = http.GET();                                        //Make the request

    if (httpCode > 0) { //Check for the returning code

      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);

      JSONVar doc = JSON.parse(payload);

      writeMessage(doc["data"]);
      Serial.print("Temp measured at: ");
      Serial.println(doc["created_at"]);

      lastConnectionTime = millis();
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
}
