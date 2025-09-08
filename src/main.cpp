
#include "Generador/Generador.h"
#include "HTTPClient.h"
#include "NTPClient.h"
#include "WiFi.h"
#include "WiFiUdp.h"
#include <Adafruit_INA219.h>
#include <Arduino.h>

const char* email_usuario = "johndoe@gmail.com";
const char* ssid = "Manuel de Falla";
const char* password = "Fallaalumno";

Adafruit_INA219 ina219;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800, 60000);

void setup()
{
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    HTTPClient http;

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWifi conectado");
    timeClient.begin();
    if (!ina219.begin()) {
        Serial.println("Error al iniciar INA219");
        while (true) {
            delay(10);
        }
    }
    Serial.println("INA219 iniciado");
    ina219.setCalibration_32V_1A();
    // setup_macaddress(http);
}

enum States {
    normal,
    reading,
};

States actual_state = normal;
int last_minute = -1;

States get_new_state(States previus_state)
{
    float current_minute = timeClient.getMinutes();
    if (current_minute == 0
        && current_minute != last_minute) {

        last_minute = current_minute;
        return States::reading;
    }

    last_minute = current_minute;
    return States::normal;
}

void loop()
{
    timeClient.update();
    actual_state = get_new_state(actual_state);
    if (actual_state != States::normal) {
        if (actual_state == States::reading) {
            // Tomar la medición cada hora
            float voltaje = ina219.getBusVoltage_V();
            Serial.println("Voltaje: " + String(voltaje));
        }
    }
}
