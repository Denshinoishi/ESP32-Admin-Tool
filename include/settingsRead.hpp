/*
Leer configuración parámetros WIFI
*/

bool settingsReadWifi()
{
    StaticJsonDocument<capacityWifi> jsonConfig;
    File file = SPIFFS.open("/settingwifi.json", "r");

    if (!file)
    {
        log("Error: No se pudo abrir el archivo de configuración WiFi");
        SettingResetWiFi();
        return false;
    }

    DeserializationError error = deserializeJson(jsonConfig, file);
    file.close();

    if (error)
    {
        log("Error: Falló la deserialización del JSON WiFi → " + String(error.c_str()));
        SettingResetWiFi();
        return false;
    }

    // General
    if (jsonConfig["id"])
        strlcpy(id, jsonConfig["id"], sizeof(id));
    if (jsonConfig["boot"].is<int>())
        bootCount = jsonConfig["boot"].as<int>();

    // Cliente
    if (jsonConfig["wifi_staticIP"].is<bool>())
        wifi_staticIp = jsonConfig["wifi_staticIP"].as<bool>();
    if (jsonConfig["wifi_ssid"])
        strlcpy(wifi_ssid, jsonConfig["wifi_ssid"], sizeof(wifi_ssid));
    if (jsonConfig["wifi_passw"])
        strlcpy(wifi_passw, jsonConfig["wifi_passw"], sizeof(wifi_passw));
    if (jsonConfig["wifi_ip_static"])
        strlcpy(wifi_ip_static, jsonConfig["wifi_ip_static"], sizeof(wifi_ip_static));
    if (jsonConfig["wifi_gateway"])
        strlcpy(wifi_gateway, jsonConfig["wifi_gateway"], sizeof(wifi_gateway));
    if (jsonConfig["wifi_subnet"])
        strlcpy(wifi_subnet, jsonConfig["wifi_subnet"], sizeof(wifi_subnet));
    if (jsonConfig["wifi_primaryDNS"])
        strlcpy(wifi_primaryDNS, jsonConfig["wifi_primaryDNS"], sizeof(wifi_primaryDNS));
    if (jsonConfig["wifi_secondaryDNS"])
        strlcpy(wifi_secondaryDNS, jsonConfig["wifi_secondaryDNS"], sizeof(wifi_secondaryDNS));

    // AP
    if (jsonConfig["ap_accessPoint"].is<bool>())
        ap_accessPoint = jsonConfig["ap_accessPoint"].as<bool>();
    if (jsonConfig["ap_name"])
        strlcpy(ap_name, jsonConfig["ap_name"], sizeof(ap_name));
    if (jsonConfig["ap_passw"])
        strlcpy(ap_passw, jsonConfig["ap_passw"], sizeof(ap_passw));
    if (jsonConfig["ap_canal"].is<int>())
        ap_canal = jsonConfig["ap_canal"].as<int>();
    if (jsonConfig["ap_hidden"].is<bool>())
        ap_hidden = jsonConfig["ap_hidden"].as<bool>();
    if (jsonConfig["ap_connect"].is<bool>())
        ap_connect = jsonConfig["ap_connect"].as<bool>();

    log("Info: Lectura de configuración WiFi completada correctamente");
    return true;
}

/*
    Lee la configuración MQTT
*/

boolean settingsReadMQTT() {
    StaticJsonDocument<capacityMQTT> jsonConfig;
    File file = SPIFFS.open(F("/settingmqtt.json"), "r");
    DeserializationError err = deserializeJson(jsonConfig, file);
    file.close();

    if (err) {
        setingsResetMQTT();
        log(F("Error: Falló la lectura de la configuración MQTT, tomando valores por defecto"));
        return false;
    }

    if (jsonConfig.containsKey("mqtt_user"))
        strlcpy(mqtt_user, jsonConfig["mqtt_user"], sizeof(mqtt_user));
    if (jsonConfig.containsKey("mqtt_passw"))
        strlcpy(mqtt_passw, jsonConfig["mqtt_passw"], sizeof(mqtt_passw));
    if (jsonConfig.containsKey("mqtt_server"))
        strlcpy(mqtt_server, jsonConfig["mqtt_server"], sizeof(mqtt_server));
    if (jsonConfig.containsKey("mqtt_id"))
        strlcpy(mqtt_id, jsonConfig["mqtt_id"], sizeof(mqtt_id));

    mqtt_time   = jsonConfig["mqtt_time"]   | 0;   // valor por defecto 0
    mqtt_port   = jsonConfig["mqtt_port"]   | 1883; // valor por defecto 1883
    mqtt_enable = jsonConfig["mqtt_enable"] | false;

    log(F("Info: Lectura de configuración MQTT correcta"));
    return true;
}
/*
    Lectura configuración IO
*/

boolean settingsReadRelays()
{
    StaticJsonDocument<capacityRelays> jsonConfig;
    // StaticJsonDocument<96> jsonConfig;
    File file = SPIFFS.open("/settingrelay.json", "r");
    if (deserializeJson(jsonConfig, file))
    {
        // Si falla la lectura carga valores por defecto
        settingsResettRelay();
        log("Error: Falló lectura de la configuración de los relevos, tomando valores por defecto");
        return false;
    }
    else
    {
        relay_01_status = jsonConfig["relay_01_status"];
        relay_02_status = jsonConfig["relay_02_status"];
        file.close();
        log("Info: Lectura de la configuración IO Correcta");
        return true;
    }
}
//-----------------------------------------------------------
// Leer Credenciales de acceso al panel de administración
//-----------------------------------------------------------
boolean settingsReadAdmin()
{
    StaticJsonDocument<capacityAdmin> jsonConfig;
    File file = SPIFFS.open("/settingsadmin.json", "r");
    if (deserializeJson(jsonConfig, file))
    {
        // Si falla la lectura carga valores por defecto
        settingsResetAdmin();
        log("Error: Falló lectura de la configuración del panel de administración, tomando valores por defecto");
        return false;
    }
    else
    {
        strlcpy(www_username, jsonConfig["www_username"], sizeof(www_username));
        strlcpy(www_password, jsonConfig["www_password"], sizeof(www_password));
        file.close();
        log("Info: Lectura de la configuración del panel de administración correcta");
        return true;
    }
}