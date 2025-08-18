/*
Leer configuración parámetros WIFI
*/

bool settingsReadWifi(){
    StaticJsonDocument<500> jsonConfig;
    File file = SPIFFS.open("/SettingWifi.json", "r");
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