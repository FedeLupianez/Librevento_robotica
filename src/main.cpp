
#include "HTTPClient.h"
#include "WiFi.h"
#include "Generador/Generador.h"
#include <Arduino.h>
#include "WiFiUdp.h"
#include "NTPClient.h"
#include <Adafruit_INA219.h>

const char *email_usuario = "johndoe@gmail.com";
const char *ssid = "Manuel de Falla";
const char *password = "Fallaalumno";
const int voltage_input_pin = 36;

Adafruit_INA219 ina219;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

const char* meditions_hours[] = {"00:00", "01:00", "02:00", "03:00", "04:00", "05:00", "06:00", "07:00", "08:00", "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00", "18:00", "19:00", "20:00", "21:00", "22:00", "23:00"};
const int meditions_cant = 24;

// bool is_time_to_measure(int current_hour, int current_minute){
//   for (int i = 0; i < meditions_cant; i++){
//     int med_hour = atoi(meditions_hours[i].substr(0, 2).c_str());
//     int med_minute = atoi(meditions_hours[i].substr(3, 5).c_str());
//     if (current_hour == med_hour && current_minute == med_minute){
//       return true;
//     }
//   }
//   return false;
// }

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  HTTPClient http;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWifi conectado");
  timeClient.begin();
  if (!ina219.begin()){
    Serial.println("Error al iniciar INA219");
    while (1){ delay(10); }
  }
  Serial.println("INA219 iniciado");
  ina219.setCalibration_32V_1A();
  //setup_macaddress(http);
}

void loop() {
  float voltaje = ina219.getBusVoltage_V();
  Serial.println("Voltaje: " + String(voltaje));
}
