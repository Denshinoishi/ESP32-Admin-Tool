/* -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2022
 * Sitio WEB: https://electroniciot.com
 * Correo: admim@electroniciot.com
 * Cel_WSP: +591 71243395
 * Plataforma: ESP32
 * Framework:  Arduino
 * Proyecto Admin Panel Tool para el ESP32 con HTNL, JavaScript, CSS
 * ESP32 Admin Tool v2.0
 * -------------------------------------------------------------------
 */

// -------------------------------------------------------------------
// Librerias
// -------------------------------------------------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <TimeLib.h>
#include <WiFi.h>
// Librería del RTC del ESP32
#include <ESP32Time.h>
// Librería para NTP
#include <NTPClient.h>
// -------------------------------------------------------------------
// Archivos *.hpp - Fragmentar el Código
// -------------------------------------------------------------------
#include "settings.hpp"
#include "functions.hpp"
#include "settingsReset.hpp"
#include "settingsRead.hpp"
#include "settingsSave.hpp"
#include "esp32_wifi.hpp"
#include "esp32_mqtt.hpp"
#include "esp32_websocket.hpp"
#include "esp32_server.hpp"
#include "esp32_pinRestore.hpp"
#include "esp32_uartRS232.hpp"

// -------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------
void setup()
{
    // Baudrate
    Serial.begin(115200);
    // Inicializar UART RS232 en Serial2
    // Serial2.begin(RS232_BAUD, SERIAL_8N1, RS232_RX, RS232_TX);
    // log("Info: Puerto RS232 inicializado a " + String(RS232_BAUD) + " baudios");
    initRS232();
    xTaskCreate(rs232Task, "rs232Task", 4096, NULL, 1, NULL);
    log("Info: Puerto RS232 inicializado a " + String(RS232_BAUD) + " baudios");
    // CPU Freq
    setCpuFrequencyMhz(240);
    // Inicio del Log por serial
    log("\nInfo: Iniciando Setup");
    // Configurar los Pines
    settingPines();
    // Inicio del SPIFFS
    if (!SPIFFS.begin(true))
    {
        log(F("Error: Falló la inicialización del SPIFFS"));
        while (true)
            ;
    }
    // Lee los estados de los Relays
    if (!settingsReadRelays())
    {
        // salvar estados de los relays
        settingsSaveRelays();
    }
    // Paso estados a los pines de los Relays
    setOnOffSingle(RELAY1, Relay01_status);
    setOnOffSingle(RELAY2, Relay02_status);
    // Lee la Configuración WiFi
    if (!settingsReadWiFi())
    {
        // Salvar las configuraciones del WIFI
        settingsSaveWiFi();
    }
    // Incrementar el contador de reinicios
    bootCount++;
    // Salvar las configuraciones del WIFI + incremento del bootCount
    settingsSaveWiFi();
    // Configuracion WIFI
    WiFi.disconnect(true);
    delay(1000);
    // Setup del WiFI
    wifi_setup();
    // Lee la Configuración MQTT
    if (!settingsReadMQTT())
    {
        // Salvar la configuracion del MQTT
        settingsSaveMQTT();
    }
    // leer www_username/password
    if (!settingsReadAdmin())
    {
        // Salvar el usuario y Contraseña
        settingsSaveAdmin();
    }
    // Leer Timer
    if (!settingsReadTimer())
    {
        settinsSaveTimer();
    }
    // Inicializar el Timer
    TimerSetup();

    // Inicializar el WebSockets
    InitWebSockets();
    // Inicializar el Servidor
    InitServer();
    // Nos devuelve la lista de carpetas y archivos del SPIFFS ONLYDEBUG
    // listDir(SPIFFS, "/", 0);
    setupPinRestore();
    log("Info: Setup completado");
}


// -------------------------------------------------------------------
// Loop Pincipal Nucleo 0
// -------------------------------------------------------------------
void loop() {
    yield();

    // -------------------------------------------------------------------
    // WIFI
    // -------------------------------------------------------------------
    if (wifi_mode == WIFI_STA) {
        wifiLoop();
    } else if (wifi_mode == WIFI_AP) {
        wifiAPLoop();
    }

    // -------------------------------------------------------------------
    // Reiniciar servidor si fue marcado
    // -------------------------------------------------------------------
    if (server_needs_restart) {
        log("Info: Cerrando conexiones antes de reiniciar servidor");

        server.end();       // Cerrar servidor HTTP
        ws.closeAll();      // Cerrar clientes WebSocket
        delay(500);         // Dar tiempo a liberar recursos

        InitWebSockets();
        InitServer();

        log("Info: Servidor y WebSockets reiniciados correctamente");
        server_needs_restart = false;
    }

    // -------------------------------------------------------------------
    // MQTT
    // -------------------------------------------------------------------
    if ((WiFi.status() == WL_CONNECTED) && (wifi_mode == WIFI_STA)) {
        if (mqtt_server != 0) {
            mqttLoop();
            if (mqttclient.connected()) {
                if (millis() - lastMsg > mqtt_time) {
                    lastMsg = millis();
                    mqtt_publish();
                }
            }
        }
    }

    // -------------------------------------------------------------------
    // Enviar JSON completo por WS cada 5 segundos
    // -------------------------------------------------------------------
    if (millis() - lastWsSend > 5000) {
        lastWsSend = millis();
        WsMessage(GetJson(), "", "");
    }

    // -------------------------------------------------------------------
    // Enviar solo la hora por WS cada 1 segundo (desde RTC)
    // -------------------------------------------------------------------
    if (millis() - lasTime > 1000) {
        lasTime = millis();
        WsMessage(getSendJson(getDateTime(), "time"), "", "");
    }

    // -------------------------------------------------------------------
    // Monitorear la Interrupción del Pin 33
    // -------------------------------------------------------------------
    //restoreIntLoop();
}
