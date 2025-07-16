#include "Generador/Generador.h"
#include "HTTPClient.h"
#include "WiFi.h"
#include <Arduino.h>

const char *email_usuario = "johndoe@gmail.com";
const char *ssid = "WIFI_EJEMPLO";
const char *password = "12345678";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  HTTPClient http;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWifi conectado");
  setup_macaddress(http);
}

void loop() {}
