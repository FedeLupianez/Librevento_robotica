#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Urls.h"

const char* ssid = "INTERNET";
const char* password = "PASSWORD";


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
  http.begin(GetIdUrl); 

  int responseCode = http.GET(); // Hago un get y obtengo el codigo de respuesta

  if (responseCode > 0){
    String response = http.getString(); // Obtengo la respuesta
    Serial.println("Id del generador :");
    Serial.println(response);
  }else {
    Serial.println("Error en la request");
    Serial.println(http.errorToString(responseCode)); // Transformo el codigo de error a string
  }
  http.end(); // Libera los recursos
}

void loop() {
}