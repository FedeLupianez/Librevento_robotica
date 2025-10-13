
#include "Generador/Generador.h"
#include "HTTPClient.h"
#include "NTPClient.h"
#include "WiFi.h"
#include "WiFiType.h"
#include "WiFiUdp.h"
#include "esp32-hal.h"
#include <Adafruit_INA219.h>
#include <Arduino.h>
#include <WiFiClientSecure.h>

const char* user_email = "fedeAdmin@gmail.com";
const char* password = "federico";
const char* ssid = "Lupianez";

Adafruit_INA219 ina219;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800, 60000);

void connect_wifi()
{
    WiFi.begin(ssid);
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 60000) {
        Serial.print(".");
        delay(500);
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Conectado!");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("No se pudo conectar");
    }
}

HTTPClient http;
WiFiClientSecure wifi_client;
void setup()
{
    Serial.begin(115200);
    delay(500);
    connect_wifi();
    // timeClient.begin();
    // if (!ina219.begin()) {
    //     Serial.println("Error al iniciar INA219");
    //     while (true) {
    //         delay(10);
    //     }
    // }
    if (WiFi.status() == WL_CONNECTED) {
        setup_macaddress(http, wifi_client, user_email);
        Serial.println("setup terminado");
    } else {
        Serial.println("No se completó el setup");
    }
}

enum States {
    normal,
    reading,
};

States actual_state = normal;
int last_minute = -1;

States get_new_state(States previus_state)
{
    // float current_minute = timeClient.getMinutes();
    // if (current_minute == 0
    //     && current_minute != last_minute) {
    //
    //     last_minute = current_minute;
    //     return States::reading;
    // }

    // last_minute = current_minute;
    return States::normal;
}

void loop()
{
    // timeClient.update();
    // actual_state = get_new_state(actual_state);
    // if (actual_state != States::normal) {
    //     if (actual_state == States::reading) {
    //         // Tomar la medición cada hora
    //         float voltaje = ina219.getBusVoltage_V();
    //         Serial.println("Voltaje: " + String(voltaje));
    //     }
    // }
}
