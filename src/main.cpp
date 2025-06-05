#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Urls.h"

const char* ssid = "INTERNET";
const char* password = "PASSWORD";

String requestGet(HTTPClient &client, String Url){
  // función para realizar una request
  client.begin(Url);
  int responseCode = client.GET();

  if (responseCode <= 0){
    return ("Error");
  }

  String response = client.getString();
  client.end(); // Elimino los recursos utilizados
  return (response);
}


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println("Esperando conexión ... ");
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.println("...");
  }
  Serial.println("Conectado a internet");

  HTTPClient http; // Defino un objeto http
  // Defino la url a la que apunta
  // En este caso hago que apunte al endpoint para obtener 
  // el id del generador
  String response = requestGet(http, GetIdUrl);
  Serial.println(response);
}

void loop() {
}