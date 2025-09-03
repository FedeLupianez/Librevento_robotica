#include "Usuario.h"
#include "ArduinoJson/Deserialization/DeserializationError.hpp"
#include <ArduinoJson.h>

int get_user_id(HTTPClient &client, const char *user_email) {
  String url = "http://localhost:8000/usuario/obtener_id?email_usuario=" +
               String(user_email);
  client.begin(url);
  int httpCode = client.GET();

  if (httpCode != HTTP_CODE_OK) {
    return (-1);
  }

  String payload = client.getString();
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println("Error al encontrar el id del usuario");
    Serial.println(error.c_str());
    return (-1);
  }
  String id = doc["id"];
  Serial.println("Id del usuario : " + id);

  client.end();
  return id.toInt();
}
