#include "Usuario.h"
#include "ArduinoJson/Deserialization/DeserializationError.hpp"
#include "WString.h"
#include "routes.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

String get_user_id(HTTPClient& client, WiFiClientSecure& wifi_client, const char* user_email)
{
    String url = user_id_url + String(user_email);
    client.begin(wifi_client, url);
    Serial.println("client connected to " + url);
    int httpCode = client.GET();

    Serial.println("Result code : " + String(httpCode));
    if (httpCode != 200) {
        client.end();
        return ("");
    }

    String payload = client.getString();
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        Serial.println("Error al encontrar el id del usuario");
        Serial.println(error.c_str());
        return ("");
    }
    String id = doc["id"];
    Serial.println("Id del usuario : " + id);

    client.end();
    return id;
}
