#include "Generador.h"
#include "Usuario/Usuario.h"
#include <ArduinoJson.h>
#include <WiFi.h>

const char *macAddressExample = "4A:3B:2C:1D:0E:5F";
const char *user_email = "johndoe@gmail.com";

void setup_macaddress(HTTPClient &client) {
  const int user_id = get_user_id(client, user_email);
  String url = "http://localhost:8000/generador/config_mac?id_usuario=" +
               String(user_id) + "&macAddress=" + WiFi.macAddress();
  client.begin(url);
  int httpCode = client.sendRequest("PATCH");

  if (httpCode == 400) {
    Serial.println("Mac ya configurada");
    return;
  }

  if (httpCode != HTTP_CODE_OK) {
    Serial.println("Error al configurar la mac");
    String payload = client.getString();
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);
    Serial.println(error.c_str());
    return;
  }

  Serial.println("Mac configurada exitosamente");
  client.end();
  return;
}
