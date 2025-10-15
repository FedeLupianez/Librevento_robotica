#include "Generador.h"
#include "Usuario/Usuario.h"
#include "routes.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

const char* macAddressExample = "4A:3B:2C:1D:0E:5F";

void setup_macaddress(HTTPClient& http_client, WiFiClientSecure& wifi_client, const char* user_email)
{
    String url = config_mac + String(user_email) +  "&macAddress=" + WiFi.macAddress();
    http_client.begin(wifi_client, url);
    Serial.println("client connected");
    int httpCode = http_client.sendRequest("PATCH");
    Serial.println("Code : " + String(httpCode));

    if (httpCode == 400) {
        Serial.println("Mac ya configurada");
        return;
    }

    if (httpCode != HTTP_CODE_OK) {
        Serial.println("Error al configurar la mac");
        String payload = http_client.getString();
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        Serial.println(error.c_str());
        return;
    }

    Serial.println("Mac configurada exitosamente");
    http_client.end();
    return;
}

void upload_medition(HTTPClient& http_client, WiFiClientSecure& wifi_client, float voltage){
    http_client.addHeader("Content-Type", "application/json");
    http_client.begin(wifi_client, create_medition);
    String json = "{\"id_generador\": 1, \"voltage\":" + String(voltage) + ", \"consumo\": 0, \"velocidad\": 0, \"direccion_viento:\": 0, \"humedad\": 0, \"temperatura\": 0, \"}";
    Serial.println("client connected");
    int httpCode = http_client.sendRequest("POST");
    Serial.println("Code : " + String(httpCode));
}
