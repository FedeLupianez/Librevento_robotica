
#include "Generador/Generador.h"
#include "HTTPClient.h"
#include "HardwareSerial.h"
#include "WiFi.h"
#include "WiFiType.h"
#include "esp32-hal.h"
#include <Adafruit_INA219.h>
#include <Arduino.h>
#include <WiFiClientSecure.h>

// Variables para el wifi y en envio de datos
const char* user_email = "fedeAdmin@gmail.com";
const char* password = "Fallaalumno";
const char* ssid = "Manuel de Falla";

// Estados
enum States {
    normal,
    reading,
    sending
};
States actual_state = normal;
// Configuraciones del tiempo
unsigned long last_reading = millis();
unsigned long last_send = millis();
const int reading_delay = 10000;
const int send_delay = 60000;
float accumulate_voltage = 0;

HTTPClient *http = new HTTPClient();
WiFiClientSecure *wifi_client = new WiFiClientSecure();

Adafruit_INA219 ina219;

void connect_wifi()
{
    WiFi.begin(ssid, password);
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

void setup()
{
    wifi_client->setInsecure();
    Serial.begin(115200);
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(500);
    connect_wifi();
    Serial.print("Buscando INA219...");
    while (true) {
        if (ina219.begin())
            break;
        Serial.print(".");
        delay(10);
    }
    Serial.println("INA219 encontrado");
    Serial.println("Configurando hora...");
     configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        now = time(nullptr);
    }
    Serial.println("Hora configurada");
    if (WiFi.status() == WL_CONNECTED) {
        setup_macaddress(*http, *wifi_client, user_email);
        Serial.println("setup terminado");
    } else {
        Serial.println("No se completó el setup");
    }

   
}

float get_avg_voltage()
{
    return accumulate_voltage / 50;
}

States get_new_state(States previus_state)
{
    Serial.println("[ GET_NEW_STATE ] " + String(previus_state));
    if (millis() - last_reading >= reading_delay) {
        last_reading = millis();
        float avg_voltage = get_avg_voltage();
        Serial.println("[ SENDING ] AVG voltage : " + String(avg_voltage));
        return States::reading;
    }

    if (millis() - last_send >= send_delay) {
        last_send = millis();
        return States::reading;
    }
    return States::normal;
}

void loop()
{
    for (int i = 0; i < 50; i++) {
        Serial.println();
    }
    actual_state = get_new_state(actual_state);
    Serial.println("[ LOOP ] Actual state -> " + String(actual_state));
    if (actual_state != States::normal) {
        if (actual_state == States::reading) {
            // Tomar la medición cada hora
            Serial.println("[ LOOP ] Leyendo voltaje");
            float voltage = ina219.getBusVoltage_V();
            accumulate_voltage += voltage;
            Serial.println("[ LOOP ] Voltage -> " + String(voltage));
            actual_state = States::normal;
        }

        if (actual_state == States::sending) {
            Serial.println("[ LOOP ] Enviando medición");
            upload_medition(*http, *wifi_client, get_avg_voltage());
            actual_state = States::normal;
        }
    }
    delay(1000);
}
