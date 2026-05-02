
 #include "ESPAsyncWebServer.h"


 /*
    Instancia de AsyncWebServer puerto 80
 */


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

//--------------------------------------
// Declaración de Funciones
void ProccesRequest(AsyncWebSocketClient *client, String request);
void WsMessage(String icon, String texto);
//--------------------------------------
// Declaración de Variables Globales
long lastWsSend = 0;    // Variable para controlar el tiempo entre envíos por WebSocket

//--------------------------------------
// Manejar Ordenes del Websocket
//--------------------------------------
void ProccesRequest(AsyncWebSocketClient *client, String request)
{
    String command = request;

    if(command == "restore"){
        log("Info: Comando por WS =>" + command);
        SettingResetWiFi();
        settingsResettRelay();
        setingsResetMQTT();
        settingsResetAdmin();
        settingsResettRelay();
        if(settinsSaveMQTT() && settinsSaveAdmin() && settingsSaveWiFi() && settingsSaveRelays()){
            WsMessage("success", "Restauración de Configuración Exitosa. Reiniciando...");
        }else{
            WsMessage("error", "Equipo no restaurado...");
        }    
        delay(5000);
        ESP.restart(); 
    } else if(command == "restart"){
        log("Info: Comando por WS =>" + command);
        WsMessage("success", "Reinicio del Equipo...");
        delay(5000);
        ESP.restart(); 
    } else{
        log("Info: Comando por WS =>" + command);
    }

    if(command != "restore"){
        OnOffRelays(command);
    }
    


}

//--------------------------------------
// Enviar JSON al Websocket
//--------------------------------------
void WsMessage(String icon, String texto){

    if(texto != ""){
        String response;
        StaticJsonDocument<300> doc;
        doc["icon"] = icon;
        doc["texto"] = texto;
        doc["type"] = "info";
        serializeJson(doc, response);
        ws.textAll(response);
    }else{
        ws.textAll(icon);
    }
    
}

//--------------------------------------
// Eventos del Websocket
//--------------------------------------
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *jsonDoc, size_t len)
{
  switch (type)
    {
    case WS_EVT_CONNECT:{
        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        client->printf("{\"ClientID\": %u }", client->id());
        client->ping();
        //log("Info: Cliente Conectado al Websocket: " + String(client->id()));
        break;
    }
    case WS_EVT_DISCONNECT:{
        //Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());    
        //log("Info: Cliente Desconectado del Websocket: " + String(client->id()));
        break;
    }
    case WS_EVT_DATA:{
        AwsFrameInfo * info = (AwsFrameInfo *)arg;
        String msg = "";
        if(info->final && info->index == 0 && info->len == len){
            //the whole message is in a single frame and we got all of it's jsonDoc
            if(info->opcode == WS_TEXT){
                for(size_t i=0; i < info->len; i++){
                    msg += (char) jsonDoc[i];
                }
            }
            else{
                char buf[3];
                for(size_t i=0; i < info->len; i++){
                    sprintf(buf, "%02x ", jsonDoc[i]);
                    msg += buf;
                }
            }

            if(info->opcode == WS_TEXT)
                ProccesRequest(client, msg);
        
        } else {
            //message is comprised of multiple frames or the frame is split into multiple packets
            if(info->opcode == WS_TEXT){
                for(size_t i=0; i < len; i++){
                    msg += (char) jsonDoc[i];
                }
            }
            else{
                char buf[3];
                for(size_t i=0; i < len; i++){
                    sprintf(buf, "%02x ", jsonDoc[i]);
                    msg += buf;
                }
            }
            Serial.printf("%s\n", msg.c_str());

            if(info->index + len == info->len){
                //all jsonDoc received
                if(info->opcode == WS_TEXT)
                ProccesRequest(client, msg);
                (client, msg);
            }
        }


        //log("Info: Datos Recibidos del Websocket: " + String((char *)jsonDoc));
        break;
    }
    case WS_EVT_PONG:{
        //Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)jsonDoc : "");
        break;  
    }
    case WS_EVT_ERROR:{
        //Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), len, (len) ? (char *)jsonDoc : "");
    break;

    } 
    
    }
}

//--------------------------------------
// Inicializar Websocket
//--------------------------------------

void initWebSockets()
{
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  log("Info: Websocket Inicializado");
}

//-------------------------------------------
// Empaquetar JSON y para envio por Websocket
//-------------------------------------------

String GetJson(){
    String response;
    DynamicJsonDocument jsonDoc(3000);
    jsonDoc["type"] = "data";
    jsonDoc["wifi_status"] = WiFi.status() == WL_CONNECTED ? F("<span class='label label-success'>Conectado</span>") : F("<span class='label label-danger'>Desconectado</span>");
    jsonDoc["wifi_dbm"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
    jsonDoc["wifi_percent"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0;
    jsonDoc["temp_cpu"] = String(round(TempCPUValue()), 2);
    jsonDoc["ram_available"] = ESP.getFreeHeap() * 100 / ESP.getHeapSize();
    jsonDoc["spiffs_used"] = SPIFFS.usedBytes() * 100 / SPIFFS.totalBytes();
    jsonDoc["mqtt_status"] = mqttclient.connected() ? F("<span class='label label-success'>Conectado</span>") : F("<span class='label label-danger'>Desconectado</span>");
    jsonDoc["mqtt_on"] = mqttclient.connected() ? F("<span class='label label-success'>Online</span>") : F("<span class='label label-danger'>Offline</span>");
    jsonDoc["mqtt_server"] = mqttclient.connected() ? F(mqtt_server) : F("Server No Conectado");
    jsonDoc["time_active"] = longTimeStr(millis() / 1000);
    jsonDoc["relay_01_status"] = relay_01_status ? true : false;
    jsonDoc["relay_02_status"] = relay_02_status ? true : false;

    serializeJson(jsonDoc, response);
    return response;

}