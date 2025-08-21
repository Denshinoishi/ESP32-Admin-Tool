



void SettingResetWiFi(){
                /*General*/
        strlcpy(id, "adminesp32", sizeof(id));
        bootCount = 0;
            /*Cliente*/
        wifi_staticIp = false;
        strlcpy(wifi_ssid, "Java34", sizeof(wifi_ssid));
        strlcpy(wifi_passw, "monom1r11ll0d3gogo", sizeof(wifi_passw));
        strlcpy(wifi_ip_static, "192.168.0.150", sizeof(wifi_ip_static));
        strlcpy(wifi_gateway, "192.168.0.1", sizeof(wifi_gateway));
        strlcpy(wifi_subnet, "255.255.255.0", sizeof(wifi_subnet));
        strlcpy(wifi_primaryDNS, "8.8.8.8", sizeof(wifi_primaryDNS));
        strlcpy(wifi_secondaryDNS, "8.8.4.4", sizeof(wifi_secondaryDNS));

            /*AP*/
        ap_accesPoint = false;
        strlcpy(ap_name, deviceID().c_str(), sizeof(ap_name));
        strlcpy(ap_passw, "adminesp32", sizeof(ap_passw));
        ap_canal = 9;
        ap_hidden = false;
        ap_connect = 4;
        
}

/*
Valores por defecto MQTT
*/
void setingsResetMQTT(){
    //Define configuración por defecto del equipo conexión MQTT
    strlcpy(mqtt_user, "", sizeof(mqtt_user));
    strlcpy(mqtt_passw, "", sizeof(mqtt_passw));
    strlcpy(mqtt_server, "192.168.0.168", sizeof(mqtt_server));
    strlcpy(mqtt_id, deviceID().c_str(), sizeof(mqtt_id));
    mqtt_time = 60000;
    mqtt_port = 1883;
    mqtt_enable = true;
}

/*
Valores default IO
*/

void settingsResettRelay(){
    relay_01_status = LOW;
    relay_02_status = LOW;

}