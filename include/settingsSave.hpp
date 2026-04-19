/*
    Guardar Estados IO
*/


boolean settingsSaveRelays(){
    //  Guarda la configuración de IO
    StaticJsonDocument<capacityRelays> jsonConfig;
    File file = SPIFFS.open(F("/settingrelay.json"), "w+");
    if (file)
    {
        jsonConfig["relay_01_status"] = relay_01_status;
        jsonConfig["relay_02_status"] = relay_02_status;
        serializeJsonPretty(jsonConfig, file);
        file.close();
        log(F("Info: Estado de IO Guardado"));
        serializeJsonPretty(jsonConfig, Serial);
        return true;
    }else{
        log(F("Error: Falló el guardado de Estado IO"));
        return false;
    }
}

bool settingsSaveWiFi() {
    StaticJsonDocument<capacityWifi> jsonConfig;
    File file = SPIFFS.open("/settingwifi.json", "w");

    if (!file) {
        log("Error: No se pudo abrir el archivo para guardar configuración WiFi");
        return false;
    }

    // General
    jsonConfig["id"] = id;
    jsonConfig["boot"] = bootCount;

    // Cliente
    jsonConfig["wifi_staticIP"] = wifi_staticIp;
    jsonConfig["wifi_ssid"] = wifi_ssid;
    jsonConfig["wifi_passw"] = wifi_passw;
    jsonConfig["wifi_ip_static"] = wifi_ip_static;
    jsonConfig["wifi_gateway"] = wifi_gateway;
    jsonConfig["wifi_subnet"] = wifi_subnet;
    jsonConfig["wifi_primaryDNS"] = wifi_primaryDNS;
    jsonConfig["wifi_secondaryDNS"] = wifi_secondaryDNS;

    // AP
    jsonConfig["ap_accessPoint"] = ap_accessPoint;
    jsonConfig["ap_name"] = ap_name;
    jsonConfig["ap_passw"] = ap_passw;
    jsonConfig["ap_canal"] = ap_canal;
    jsonConfig["ap_hidden"] = ap_hidden;
    jsonConfig["ap_connect"] = ap_connect;

    if (serializeJsonPretty(jsonConfig, file) == 0) {
        log("Error: Falló la escritura del JSON en el archivo");
        file.close();
        return false;
    }

    file.close();
    log("Info: Configuración WiFi guardada correctamente");
    serializeJsonPretty(jsonConfig, Serial);  // Para depuración
    return true;
}
//-------------------------------------------
// Guardar Credenciales en Sistema de Archivos SPIFFS
//--------------------------------------------
boolean settinsSaveAdmin(){
    // Guarda las credenciales de acceso al panel de administración

    StaticJsonDocument<capacityAdmin> jsonConfig;
    File file = SPIFFS.open(F("/settingsadmin.json"), "w+");
    if (file)
    {
        jsonConfig["www_username"] = www_username;
        jsonConfig["www_password"] = www_password;
        serializeJsonPretty(jsonConfig, file);
        file.close();
        log(F("Info: Credenciales de acceso guardadas"));
        //serializeJsonPretty(jsonConfig, Serial);
        return true;
    }else{
        log(F("Error: Falló el guardado de credenciales de acceso"));
        return false;
    }
}
//---------------------------------------------------------
// Guardar Configuración MQTT en Sistema de Archivos SPIFFS
//---------------------------------------------------------
boolean settinsSaveMQTT() {
    StaticJsonDocument<capacityMQTT> jsonConfig;
    File file = SPIFFS.open(F("/settingmqtt.json"), "w");
    if (file) {
        jsonConfig["mqtt_enable"] = mqtt_enable;
        jsonConfig["mqtt_server"] = mqtt_server;
        jsonConfig["mqtt_port"]   = mqtt_port;
        jsonConfig["mqtt_user"]   = mqtt_user;
        jsonConfig["mqtt_passw"]  = mqtt_passw;
        jsonConfig["mqtt_id"]     = mqtt_id;
        jsonConfig["mqtt_time"]   = mqtt_time;

        serializeJsonPretty(jsonConfig, file);
        file.close();

        log(F("Info: Configuración MQTT guardada"));
        //serializeJsonPretty(jsonConfig, Serial);
        return true;
    } else {
        log(F("Error: Falló el guardado de configuración MQTT"));
        return false;
    }
}

