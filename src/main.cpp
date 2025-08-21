#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

#include "settings.hpp"
#include "functions.hpp"
#include "settingsReset.hpp"
#include "settingsRead.hpp"
#include "settingsSave.hpp"
#include "esp32_wifi.hpp"
#include "esp32_mqtt.hpp"

void setup()
{
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
  if (!SPIFFS.begin(true))
  {
    log(F("Error: Falló al inicialización del SPIFFS"));
    while (true)
      ;
  }
  // Lee la configuración del WiFi
  settingsReadWifi();
  
  // Lee la configuración de Salidas
  settingsReadRelays();
  // Configuración IO
  setOnOff(RELAY1, relay_01_status);
  setOnOff(RELAY2, relay_02_status);
  // Configuración del WiFi
  WiFi.disconnect(true);
  delay(1000);
  // Setup WiFi
  wifi_setup();
  // Lee la configuración del MQTT
  settingsReadMQTT();
}

void loop()
{
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

  /*
      MQTT
  */

  if ((WiFi.status() == WL_CONNECTED) && (wifi_mode == WIFI_STA))
  {
    if (mqtt_server != 0)
    {
      mqttloop();
      if (mqttclient.connected())
      {
        if ((millis() - lastMsg) > mqtt_time)
        {
          lastMsg = millis();
          mqtt_publish();
        }
      }
    }
  }





}