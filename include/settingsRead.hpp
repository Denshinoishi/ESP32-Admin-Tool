/*
Leer configuración parámetros WIFI
*/

bool settingsReadWifi(){
    StaticJsonDocument<500> jsonConfig;
    File file = SPIFFS.open("/settingwifi.json", "r");
    if(deserializeJson(jsonConfig, file)){
        // Si falla la lectura inicial de valores por defecto
        SettingResetWiFi();
        log("Error: Falló la lectura de la configuración WiFi, tomando valores por defecto");
        return false;
    }else{

            /*General*/
        strlcpy(id, jsonConfig["id"], sizeof(id));
        bootCount = jsonConfig["boot"];
            /*Cliente*/
        wifi_staticIp = jsonConfig["wifi_saticIP"];
        strlcpy(wifi_ssid, jsonConfig["wifi_ssid"], sizeof(wifi_ssid));
        strlcpy(wifi_passw, jsonConfig["wifi_passw"], sizeof(wifi_passw));
        strlcpy(wifi_ip_static, jsonConfig["wifi_ip_static"], sizeof(wifi_ip_static));
        strlcpy(wifi_gateway, jsonConfig["wifi_gateway"], sizeof(wifi_gateway));
        strlcpy(wifi_subnet, jsonConfig["wifi_subnet"], sizeof(wifi_subnet));
        strlcpy(wifi_primaryDNS, jsonConfig["wifi_primaryDNS"], sizeof(wifi_primaryDNS));
        strlcpy(wifi_secondaryDNS, jsonConfig["wifi_secondaryDNS"], sizeof(wifi_secondaryDNS));

            /*AP*/
        ap_accesPoint = jsonConfig["ap_accessPoint"];
        strlcpy(ap_name, jsonConfig["ap_accesPoint"], sizeof(ap_name));
        strlcpy(ap_passw, jsonConfig["ap_passw"], sizeof(ap_passw));
        ap_canal = jsonConfig["ap_cannal"];
        ap_hidden = jsonConfig["ap_hidden"];
        ap_connect = jsonConfig["ap_connect"];
        file.close();
        log("Info: Lectura configuración WiFi correcta");
        return true;
    }
}


/*
    Lee la configuración MQTT
*/


boolean settingsReadMQTT(){

    StaticJsonDocument<500> jsonConfig;
    File file = SPIFFS.open(F("/settingmqtt.json"), "r");
    if (deserializeJson(jsonConfig, file))
    {
        //Si falla la lectura asume valores por defecto
        setingsResetMQTT();
        log(F("Error: Falló la lectura de la configuración MQTT, tomando valores por defecto"));
        return false;
    }else{
        // Si lee el archivo
    }
    strlcpy(mqtt_user, jsonConfig["mqtt_user"], sizeof(mqtt_user));
    strlcpy(mqtt_passw, jsonConfig["mqtt_passw"], sizeof(mqtt_passw));
    strlcpy(mqtt_server, jsonConfig["mqtt_user"], sizeof(mqtt_user));
    strlcpy(mqtt_id, jsonConfig["mqtt_id"], sizeof(mqtt_id));
    mqtt_time = jsonConfig["mqtt_time"];
    mqtt_port = jsonConfig["mqtt_port"];
    mqtt_enable = jsonConfig["mqtt_enable"];
    file.close();
    log(F("Info: Lectura de configuración MQTT correcta"));
    return true;
    

}
/*
    Lectura configuración IO
*/

 boolean settingsReadRelays(){
    StaticJsonDocument<200> jsonConfig;
    File file = SPIFFS.open("/settingrelay.json", "r");
    if (deserializeJson(jsonConfig, file))
    {
        // Si falla la lectura carga valores por defecto
        settingsResettRelay();
        log("Error: Falló lectura de la configuración de los relevos, tomando valores por defecto");
        return false;
    }else{
        relay_01_status = jsonConfig["relay_01_status"];
        relay_02_status = jsonConfig["relay_02_status"];
        file.close();
        log("Info: Lectura de la configuración IO Correcta");
        return true;
    }

 }