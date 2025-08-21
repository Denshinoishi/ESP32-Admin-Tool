#include <PubSubClient.h>



WiFiClient espClient;
PubSubClient mqttclient(espClient);


char topico[150];

String mqtt_data = "";
long lastMqttReconnectAttempt = 0;
long lastMsg = 0;

void callback(char *topic, byte *payload, unsigned int length);
String Json();

/*
    MQTT Connection
*/
boolean mqtt_connect(){

    mqttclient.setServer(mqtt_server, mqtt_port);
    mqttclient.setCallback(callback);
    log("Info: Intentando conexión al Broker MQTT...");
    //  Conexión al Servidor MQTT, ClienteID, Usuario, Password.
    //  Ver documentación => https://pubsubclient.knolleary.net/api.html
    if (mqttclient.connect(mqtt_id, mqtt_user, mqtt_passw))         // Attempt to Connect
    {
        log("Info: Conectando al Broker MQTT");
        // Nos suscribimos a comandos Topico: usuario/dispositivo/comando
       String topico_subscribe;
        if (String(mqtt_user).length() > 0) {
            topico_subscribe = String(mqtt_user) + "/" + mqtt_id + "/command";
        } else {
            topico_subscribe = String(mqtt_id) + "/command";
        }
        topico_subscribe.toCharArray(topico, 150);
        mqttclient.subscribe(topico);

        topico_subscribe.toCharArray(topico, 25);
        mqttclient.publish(topico, "OK"); // Publicar Respuesta por MQTT
        
    }else{
        log("Error: failed, rc= " + mqttclient.state()) ;
        return(0);
    }

    return(1);
    
}


// Manejo de Mensajes Entrantes
void callback(char *topic, byte *payload, unsigned int lenght){
    String mensaje = "";
    String str_topic(topic);
    for (uint16_t i = 0; i < lenght; i++)
    {
        mensaje  += (char)payload[i];
        mqttRX();
    }
    mensaje.trim();
    DynamicJsonDocument JsonDoc(300);

    log("Mensaje Crudo Recibido: ");
    log(mensaje);
    deserializeJson(JsonDoc, mensaje);
    // TODO: MANEJO JSON
    if (JsonDoc["RELAY1"] == "on")
    {
       setOnSingle(RELAY1);
       relay_01_status = HIGH;
       settingsSaveRelays();
    }else if (JsonDoc["RELAY1"] == "off")
    {
        setOffSingle(RELAY1);
        relay_01_status = LOW;
        settingsSaveRelays();
    }else if (JsonDoc["RELAY2"] == "on")
    {
        setOnSingle(RELAY2);
        relay_02_status = LOW;
        settingsSaveRelays();
    }else if (JsonDoc["RELAY2"] == "off")
    {
        setOffSingle(RELAY2);
        relay_02_status = LOW;
        settingsSaveRelays();
    }
    
    



    log("Info: Topico -->" + str_topic);
    log("Info: Mensaje -->" + mensaje);
    serializeJsonPretty(JsonDoc, Serial);

}

/*
Manejo de los mensajes salientes
*/
void mqtt_publish(){
    
String topic = String(mqtt_user) + "/" + mqtt_id + "/values";
mqtt_data = Json();
mqttclient.publish(topic.c_str(), mqtt_data.c_str());
mqtt_data = "";

mqttTX();

}

String Json(){
    String response;
    DynamicJsonDocument jsonDoc(3000);
    jsonDoc["WFDMB"] = WiFi.status() == WL_CONNECTED ? String(WiFi.RSSI()) : F("0");
    jsonDoc["WFPRC"] = WiFi.status() == WL_CONNECTED ? String(getRSSIasQuality(WiFi.RSSI())) : F("0");
    jsonDoc["TempCPU"] = String(TempCPUValue());
    jsonDoc["RAMD"] = String(ESP.getFreeHeap() * 100 / ESP.getHeapSize());
    jsonDoc["FLASHU"] = String(round(SPIFFS.usedBytes() * 100 / SPIFFS.totalBytes()), 0);
    jsonDoc["relay_01_status"] = String(relay_01_status ? "true" : "false");
    jsonDoc["relay_02_status"] = String(relay_02_status ? "true" : "false");
    
    serializeJson(jsonDoc, response);
    return response;

}

/*
    MQTT Principal
*/
void mqttloop(){
    if (mqtt_enable)
    {
        if (!mqttclient.connected())
        {
            long now = millis();
            // try an reconnect continuosly for first 60 s then try again once every 120 s
            if ((now < 60000) || ((now - lastMqttReconnectAttempt) > 120000) )
            {
                lastMqttReconnectAttempt = now;
                if (mqtt_connect())
                {
                    lastMqttReconnectAttempt = 0;
                }
                setOnSingle(MQTTLED);
            }
            
        }else{
        // if MQTT connected
        mqttclient.loop();
        setOffSingle(MQTTLED);
        
    }
    }
    

}