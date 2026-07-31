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
// Valores por defecto Parametros WIFI
// -------------------------------------------------------------------
void settingsResetWiFi(){
    /* ------------------- GENERAL -------------------- */
    strlcpy(id, "adminesp32", sizeof(id));
    bootCount = 0;
    /* ------------------- CLIENTE -------------------- */
    wifi_staticIP = true;
    strlcpy(wifi_ssid, "Java34", sizeof(wifi_ssid));
    strlcpy(wifi_passw, "monom1r11ll0d3gogo", sizeof(wifi_passw));
    strlcpy(wifi_ip_static, "192.168.0.150", sizeof(wifi_ip_static));
    strlcpy(wifi_gateway, "192.168.0.1", sizeof(wifi_gateway));
    strlcpy(wifi_subnet, "255.255.255.0", sizeof(wifi_subnet));
    strlcpy(wifi_primaryDNS, "8.8.8.8", sizeof(wifi_primaryDNS));
    strlcpy(wifi_secondaryDNS, "8.8.4.4", sizeof(wifi_secondaryDNS));

    /* ------------------- AP ------------------------- */
    ap_accessPoint = false;
    strlcpy(ap_nameap, deviceID().c_str(), sizeof(ap_nameap));
    strlcpy(ap_passwordap, "adminesp32", sizeof(ap_passwordap));
    ap_canalap = 9;         
    ap_hiddenap = false;        
    ap_connetap = 4;
}
// -------------------------------------------------------------------
// Valores por defecto Parametros MQTT
// -------------------------------------------------------------------
void settingsResetMQTT(){
    // Define configuración por defecto del equipo conexión MQTT
    strlcpy(mqtt_user, "cuba_iot", sizeof(mqtt_user));
    strlcpy(mqtt_passw, "cuba_iot", sizeof(mqtt_passw));
    strlcpy(mqtt_server, "cubaelectronica.com", sizeof(mqtt_server));
    strlcpy(mqtt_id, device_id.c_str(), sizeof(mqtt_id));
    mqtt_time = 60000;  // Un Minuto
    mqtt_port = 1883;
    mqtt_enable = false;
}
// -------------------------------------------------------------------
// Valores por defecto Relays
// -------------------------------------------------------------------
void settingsResetRelays(){
    Relay01_status = LOW;
    Relay02_status = LOW;
}
// -------------------------------------------------------------------
// Valores por defecto www_username/password
// -------------------------------------------------------------------
void settingsResetAdmin(){
    // Define configuración por defecto Usuario y Contraseña WWW
    strlcpy(www_username, "admin", sizeof(www_username));
    strlcpy(www_password, "admin", sizeof(www_password));
}

void settingsResetTimer(){
    // Define configuración Timer
    time_ajuste = 0;
    strlcpy(time_date, "01-01-2022", sizeof(time_date));
    time_hr = 0;
    time_mn = 0;
    time_sc = 0;
    setDyMsYr();
    time_zhoraria = -18000;                                         // Zona Horaria  GMT -5 * -3600 = ?
    strlcpy(time_server, "time.nist.gov", sizeof(time_server));
}