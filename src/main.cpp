#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>




#include "settings.hpp"
#include "functions.hpp"
#include "settingsReset.hpp"
#include "settingsRead.hpp"
#include "settingsSave.hpp"
#include "esp32_wifi.hpp"



void setup() {
  // baudrate
  Serial.begin(115200);
  // RS232
  Serial2.begin(RS232_BAUD, SERIAL_8N1, RS232_RX, RS232_TX);
  // CPU Freq
  setCpuFrequencyMhz(240);
  // Iniciar Monitor
  log("\nInfo: Iniciando Setup");
  // Configuración de PINOUT
  setupPinOut();
  // Configuración de RS232
  setupSerialPort();
  // SPIFFS
  if (!SPIFFS.begin(true)){
    log(F("Error: Falló al inicialización del SPIFFS"));
    while (true);
  }
  // Configuraicón de WiFi
  settingsReadWifi();
  WiFi.disconnect(true);
  delay(1000);
  wifi_setup();
  
}





void loop() {
  yield();
  // WIFI
  if (wifi_mode == WIFI_STA)
  {
    wifiLoop();
  }
  if (wifi_mode == WIFI_AP)
  {
    wifiAPLopp();
  }
  

}