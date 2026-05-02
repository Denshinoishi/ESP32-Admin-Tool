/*
    Cargar Información de las páginas al Servidor peticiones GET/POST
*/
bool enableCors = true; // Habilitar CORS para permitir solicitudes desde otros orígenes?
////----------------AUTENTICACIÓN------------------
bool requestPreProcess(AsyncWebServerRequest *request, AsyncResponseStream *&response, const char *contentType = "application/json")
{
    if(!request->authenticate(www_username, www_password)) {
        request->requestAuthentication();
        return false;
    }
    response = request->beginResponseStream(contentType);

    if(enableCors)
    {
        response->addHeader("Access-Control-Allow-Origin", "*");
    }
    return true;

}

/*
    Cargar Home o index
*/
void handleHome(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response;
    if(false == requestPreProcess(request, response))
    {
        return;
    }

    File file = SPIFFS.open(F("/index.html"), "r");
    if (file)
    {
        file.setTimeout(100);
        String s = file.readString();
        file.close();

        /*
            Actualiza el Contenido del .html al cargar
        */

        s.replace(F("#id#"), id);
        s.replace(F("#serie#"), device_id);
        // Bloque Inalámbrico
        s.replace(F("#wifi_status#"), WiFi.status() == WL_CONNECTED ? F("<span class='label label-success'>CONECTADO</span>") : F("<span class='label label-success'>DESCONECTADO</span>"));
        s.replace(F("#wifi_ssid#"), WiFi.status() == WL_CONNECTED ? F(wifi_ssid) : F("WiFi desconectado"));
        s.replace(F("#wifi_ip#"), ipStr(WiFi.status() == WL_CONNECTED ? WiFi.localIP() : WiFi.softAPIP()));
        s.replace(F("#wifi_mac#"), String(WiFi.macAddress()));
        s.replace(F("#wifi_dbm#"), WiFi.status() == WL_CONNECTED ? String(WiFi.RSSI()) : F("0"));
        s.replace(F("#wifi_percent#"), WiFi.status() == WL_CONNECTED ? String(getRSSIasQuality(WiFi.RSSI())) : F("0"));
        //  Bloque MQTT
        s.replace(F("#mqtt_status#"), mqttclient.connected() ? F("<span class='label label-success'>CONECTADO</span>") : F("<span class='label label-success'>DESCONECTADO</span>"));
        s.replace(F("#mqtt_server#"), mqttclient.connected() ? F(mqtt_server) : F("Servidor Desconectado"));
        s.replace(F("#mqtt_user#"), F(mqtt_user));
        s.replace(F("#mqtt_id#"), F(mqtt_id));
        //  Información
        s.replace(F("#clientIP#"), ipStr(request->client()->remoteIP()));
        s.replace(F("#firmware#"), currentfirmware);
        s.replace(F("#sdk#"), String(ESP.getSdkVersion()));
        s.replace(F("#hardware#"), F(HW));
        s.replace(F("#cpu_freq#"), String(getCpuFrequencyMhz()));
        s.replace(F("#flash_size#"), String(ESP.getFlashChipSize() / (1024.0 * 1024), 2));
        s.replace(F("#ram_size#"), String(ESP.getHeapSize() / 1024.0, 2));
        s.replace(F("#time_active#"), longTimeStr(millis() / 1000));
        s.replace(F("#bootCount#"), String(bootCount));
        //  General
        s.replace(F("#platform#"), plafform());
        s.replace(F("#mqtt_on#"), mqttclient.connected() ? F("<span class='label btn-metis-2'>Online</span>") : F("<span class='label label-danger'>Offline</span>"));
        s.replace(F("#temp_cpu#"), String(TempCPUValue()));
        //  Progessbar
        s.replace(F("#spiffs_used#"), String((round(SPIFFS.usedBytes() * 100)) / SPIFFS.totalBytes(), 0));
        s.replace(F("#ram_available#"), String(ESP.getFreeHeap() * 100 / ESP.getHeapSize()));
        //  IO
        s.replace(F("#relay1#"), relay_01_status ? "cheked" : "");
        s.replace(F("#relay2#"), relay_02_status ? "cheked" : "");
        s.replace(F("#label1#"), relay_01_status ? "label-primary" : "label-info");
        s.replace(F("#label2#"), relay_02_status ? "label-primary" : "label-info");
        s.replace(F("#Statustext1#"), relay_01_status ? "Activo" : "Apagado");
        s.replace(F("#Statustext2#"), relay_02_status ? "Activo" : "Apagado");
        // Send Data
        request->send(200, "text/html", s);
    }
    else
    {
        request->send(500, "text/plain", "index.html not found, have you flashed the SPIFFS?");
    }
}

void InitServer()
{

    /*
        Cargar todos los Archivos Estáticos del Servidor
    */

    server.serveStatic("/www/bootstrap-switch.css", SPIFFS, "/www/bootstrap-switch.css").setDefaultFile("/www/bootstrap-switch.css").setCacheControl("max-age=600");
    server.serveStatic("/www/bootstrap-switch.min.js", SPIFFS, "/www/bootstrap-switch.min.js").setDefaultFile("/www/bootstrap-switch.min.js").setCacheControl("max-age=600");

    server.serveStatic("/www/bootstrap.css", SPIFFS, "/www/bootstrap.css").setDefaultFile("/www/bootstrap.css").setCacheControl("max-age=600");
    server.serveStatic("/www/bootstrap.min.js", SPIFFS, "/www/bootstrap.min.js").setDefaultFile("/www/bootstrap.min.js").setCacheControl("max-age=600");

    server.serveStatic("/www/esp32.png", SPIFFS, "/www/esp32.png").setDefaultFile("/www/esp32.png").setCacheControl("max-age=600");

    server.serveStatic("/www/font-awesome.css", SPIFFS, "/www/font-awesome.css").setDefaultFile("/www/font-awesome.css").setCacheControl("max-age=600");
    server.serveStatic("/fontawesome-webfont.eot", SPIFFS, "/www/fontawesome-webfont.eot").setDefaultFile("/www/fontawesome-webfont.eot").setCacheControl("max-age=600");
    server.serveStatic("/www/fontawesome-webfont.woff", SPIFFS, "/www/fontawesome-webfont.woff").setDefaultFile("/www/fontawesome-webfont.woff").setCacheControl("max-age=600");

    server.serveStatic("/www/jquery-2.0.3.min.js", SPIFFS, "/www/jquery-2.0.3.min.js").setDefaultFile("/www/jquery-2.0.3.min.js").setCacheControl("max-age=600");

    server.serveStatic("/www/layout2.css", SPIFFS, "/www/layout2.css").setDefaultFile("/www/layout2.css").setCacheControl("max-age=600");

    server.serveStatic("/www/logo.png", SPIFFS, "/www/logo.png").setDefaultFile("/www/logo.png").setCacheControl("max-age=600");

    server.serveStatic("/www/main.css", SPIFFS, "/www/main.css").setDefaultFile("/www/main.css").setCacheControl("max-age=600");

    server.serveStatic("/www/modernizr-2.6.2.min.js", SPIFFS, "/www/modernizr-2.6.2.min.js").setDefaultFile("/www/modernizr-2.6.2.min.js").setCacheControl("max-age=600");

    server.serveStatic("/www/MoneAdmin.css", SPIFFS, "/www/MoneAdmin.css").setDefaultFile("/www/MoneAdmin.css").setCacheControl("max-age=600");

    server.serveStatic("/www/scripts.js", SPIFFS, "/www/scripts.js").setDefaultFile("/www/scripts.js").setCacheControl("max-age=600");

    server.serveStatic("/www/styles.css.js", SPIFFS, "/www/styles.css").setDefaultFile("/www/styles.css").setCacheControl("max-age=600");

    server.serveStatic("/www/styles.css", SPIFFS, "/www/styles.css").setDefaultFile("/www/styles.css").setCacheControl("max-age=600");

    server.serveStatic("/www/theme.css", SPIFFS, "/www/theme.css").setDefaultFile("/www/theme.css").setCacheControl("max-age=600");

    server.serveStatic("/www/sweetalert2.min.css", SPIFFS, "/www/sweetalert2.min.css").setDefaultFile("/www/sweetalert2.min.css").setCacheControl("max-age=600");
    server.serveStatic("/www/sweetalert2.min.js", SPIFFS, "/www/sweetalert2.min.js").setDefaultFile("/www/sweetalert2.min.js").setCacheControl("max-age=600");
    server.serveStatic("/www/error.css", SPIFFS, "/www/error.css").setDefaultFile("/www/error.css").setCacheControl("max-age=600");

    /*
        Cargar Index o Home
    */

    server.on("/", HTTP_GET, handleHome);

    /*
    //Cargar Página Wifi
    // url: /esp-wifi
    // GET
     */
    // Cargar la página html Configuración del WiFi
    server.on("/esp-wifi", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }
                // Cargar la página html Configuración del WiFi
                    File file = SPIFFS.open(F("/wifi.html"), "r");
                  if (file)
                  {
                      file.setTimeout(100);
                      String s = file.readString();
                      file.close();
                      // Sección Cliente
                      s.replace(F("#platform#"), plafform());
                      s.replace(F("#wifi_ssid#"), wifi_ssid);
                      s.replace(F("#wifi_staticIP#"), wifi_staticIp ? "checked" : "");
                      s.replace(F("#wifi_staticIPen#"), wifi_staticIp ? "1" : "0");
                      s.replace(F("#wifi_ip_static#"), ipStr(StrIP(wifi_ip_static)));
                      s.replace(F("#wifi_subnet#"), ipStr(StrIP(wifi_subnet)));
                      s.replace(F("#wifi_gateway#"), ipStr(StrIP(wifi_gateway)));
                      s.replace(F("#wifi_primaryDNS#"), ipStr(StrIP(wifi_primaryDNS)));
                      s.replace(F("#wifi_secondaryDNS#"), ipStr(StrIP(wifi_secondaryDNS)));
                      // Sección AP

                      s.replace(F("#ap_accesPoint#"), ap_accessPoint ? "checked" : "");
                      s.replace(F("#ap_AP_en#"), ap_accessPoint ? "1" : "0");
                      s.replace(F("#ap_nameap#"), String(ap_name));
                      s.replace(F("#ap_canalap#"), String(ap_canal));
                      s.replace(F("#ap_hiddenap#"), ap_hidden ? "checked" : "");
                      s.replace(F("#ap_visibility#"), ap_hidden ? "0" : "1");
                      s.replace(F("#ap_connetap#"), String(ap_connect));
                      // Send data
                      request->send(200, "text/html", s);
                  }
                  else
                  {
                      request->send(500, "text/plain", "wifi.html not found, have you flashed the SPIFFS?");
                  } });

    /*
    //Cargar Página MQTT
    // url: /esp-mqtt
    // GET
    */
    // Cargar la página html Configuración del MQTT
    server.on("/esp-mqtt", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }
                  // Cargar la página html Configuración del MQTT  
                  File file = SPIFFS.open(F("/mqtt.html"), "r");
                  if (file)
                  {
                      file.setTimeout(100);
                      String s = file.readString();
                      file.close();
                      // Sección MQTT
                      s.replace(F("#platform#"), plafform());
                      s.replace(F("#mqtt_enable#"), mqtt_enable ? "checked" : "");
                      s.replace(F("#mqtten#"), mqtt_enable ? "1" : "0");
                      s.replace(F("#mqtt_server#"), mqtt_server);
                      s.replace(F("#mqtt_port#"), String(mqtt_port));
                      s.replace(F("#mqtt_user#"), mqtt_user);
                      s.replace(F("#mqtt_id#"), mqtt_id);
                      s.replace(F("#mqtt_time#"), String(mqtt_time / 60000));
                      // Send data
                      request->send(200, "text/html", s);
                  }
                  else
                  {
                      request->send(500, "text/plain", "mqtt.html not found, have you flashed the SPIFFS?");
                  } });

    /*
    //Cargar Página device.html
    // url: /esp-device
    // GET
    */
    // Cargar la página html Configuración del dispositivo
    server.on("/esp-device", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }
                  // Cargar la página html Configuración del dispositivo
                  File file = SPIFFS.open(F("/device.html"), "r");
                  if (file)
                  {
                      file.setTimeout(100);
                      String s = file.readString();
                      file.close();
                      s.replace(F("#platform#"), plafform());
                      s.replace(F("#id#"), id);
                      s.replace(F("#serie#"), device_id);

                      // Send data
                      request->send(200, "text/html", s);
                  }
                  else
                  {
                      request->send(500, "text/plain", "device.html not found, have you flashed the SPIFFS?");
                  } });
//----------------------------------------------------------
// Guardar La configuración del MQTT
// url: /esp-mqtt
// POST
//----------------------------------------------------------
    server.on("/esp-mqtt", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }
                // Sección MQTT
                if (request->hasArg("mqtten"))
                mqtt_enable = request->arg("mqtten").toInt();
                    if(mqtt_enable == 1){
                        if(request->params() == 7){
                            String s;
                            // ID MQTT Broker
                            if (request->hasArg("mqtt_id")) {
                                s = request->arg("mqtt_id");
                                s.trim();
                                if (s != "") {
                                    strlcpy(mqtt_id, s.c_str(), sizeof(mqtt_id));
                                }
                            }
                            // Usuario MQTT Broker
                            if (request->hasArg("mqtt_user")) {
                                s = request->arg("mqtt_user");
                                s.trim();
                                if (s != "") {
                                    strlcpy(mqtt_user, s.c_str(), sizeof(mqtt_user));
                                }
                            }
                            // Contraseña MQTT Broker
                            if (request->hasArg("mqtt_passw")) {
                                s = request->arg("mqtt_passw");
                                s.trim();
                                if (s != "") {
                                    strlcpy(mqtt_passw, s.c_str(), sizeof(mqtt_passw));
                                }
                            }
                            // Servidor MQTT Broker
                            if (request->hasArg("mqtt_server")) {
                                s = request->arg("mqtt_server");
                                s.trim();
                                if (s != "") {
                                    strlcpy(mqtt_server, s.c_str(), sizeof(mqtt_server));
                                }
                            }
                            // Puerto MQTT Broker
                            if (request->hasArg("mqtt_port")) 
                                mqtt_port = request->arg("mqtt_port").toInt();
                            // Tiempo Reconexion MQTT Broker
                            if (request->hasArg("mqtt_time"))
                                mqtt_time = request->arg("mqtt_time").toInt() * 60000; 
                            // Guardar Configuración
                            leds();
                            if (settinsSaveMQTT()) {
                                request->send(200, "text/html", SweetAlert("MQTT", "Configuración MQTT Guardada", "La configuración MQTT ha sido guardada correctamente", "success", "accion"));
                                return;
                            } else {
                                request->send(200, "text/html", SweetAlert("MQTT", "Error al guardar la configuración", "No se han podido guardar las nuevas configuraciones MQTT", "error", "aviso"));
                                return;
                            }
                        }else{
                            request->send(400, "text/html", SweetAlert("Error", "Error", "Error de Parámetros", "error", "aviso"));
                            return;

                        }

                    } else{
                        //Leds
                        leds();
                        //Guardar Configuración
                        if (settinsSaveMQTT()) {
                            request->send(200, "text/html", SweetAlert("MQTT", "MQTT Desactivado", "La conexión MQTT ha sido desactivada correctamente", "success", "accion"));
                            return;
                        } else {
                            request->send(200, "text/html", SweetAlert("MQTT", "Error al guardar la configuración", "No se han podido guardar las nuevas configuraciones MQTT", "error", "aviso"));
                            return;
                        }

                }
              });

    // --------------------------------------------------------
    // Guardar La configuración del WiFi
    // url: /esp-wifi
    // POST
    // --------------------------------------------------------
    server.on("/esp-wifi", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }
                // IP estática
                if (request->hasArg("wifi_staticIPen"))
                wifi_staticIp = request->arg("wifi_staticIPen").toInt();
                // AP Habilitado
                if (request->hasArg("ap_AP_en"))
                ap_accessPoint = request->arg("ap_AP_en").toInt();

                String s;
                // SSID WiFi
                if (request->hasArg("wifi_ssid")) 
                s = request->arg("wifi_ssid");
                s.trim();
                if (s != "") {
                    strlcpy(wifi_ssid, s.c_str(), sizeof(wifi_ssid));
                }
                s = "";
                // Password WiFi
                if (request->hasArg("wifi_passw"))
                s = request->arg("wifi_passw");
                s.trim();
                if (s != "") {
                    strlcpy(wifi_passw, s.c_str(), sizeof(wifi_passw));
                }
                s = "";

                // IP estática
                if(wifi_staticIp == 1){

                    if(request->hasArg("wifi_ip_static"))
                    s = request->arg("wifi_ip_static");
                    s.trim();
                    if (s != "") {
                        strlcpy(wifi_ip_static, s.c_str(), sizeof(wifi_ip_static));
                    }
                    s = "";
                    // Máscara de Subred
                    if (request->hasArg("wifi_subnet"))
                    s = request->arg("wifi_subnet");
                    s.trim();
                    if (s != "") {
                        strlcpy(wifi_subnet, s.c_str(), sizeof(wifi_subnet));
                    }
                    s = "";
                    // Puerta de Enlace
                    if (request->hasArg("wifi_gateway"))
                    s = request->arg("wifi_gateway");
                    s.trim();
                    if (s != "") {
                        strlcpy(wifi_gateway, s.c_str(), sizeof(wifi_gateway));
                    }
                    s = "";
                    // DNS Primario
                    if (request->hasArg("wifi_primaryDNS"))
                    s = request->arg("wifi_primaryDNS");
                    s.trim();
                    if (s != "") {
                        strlcpy(wifi_primaryDNS, s.c_str(), sizeof(wifi_primaryDNS));
                    }
                    s = "";
                    // DNS Secundario
                    if (request->hasArg("wifi_secondaryDNS"))
                    s = request->arg("wifi_secondaryDNS");
                    s.trim();
                    if (s != "") {
                        strlcpy(wifi_secondaryDNS, s.c_str(), sizeof(wifi_secondaryDNS));
                    }
                    s = "";
                } else if (ap_accessPoint == 1)
                {
                    // Nombre AP
                    if (request->hasArg("ap_nameap"))
                    s = request->arg("ap_nameap");
                    s.trim();
                    if (s != "") {
                        strlcpy(ap_name, s.c_str(), sizeof(ap_name));
                    }
                    s = "";
                    // AP Password
                    if (request->hasArg("ap_passwordap"))
                    s = request->arg("ap_passwordap");
                    s.trim();
                    if (s != "") {
                        strlcpy(ap_passw, s.c_str(), sizeof(ap_passw));
                    }

                    // Canal AP 1 - 13
                    if (request->hasArg("ap_canalap"))
                    ap_canal = request->arg("ap_canalap").toInt();
                    // AP Oculto
                    if (request->hasArg("ap_visibility"))
                    ap_hidden = request->arg("ap_visibility").toInt();
                    // Conexión AP 0 - 8
                    if (request->hasArg("ap_connetap"))
                    ap_connect = request->arg("ap_connetap").toInt();

                } 
                // Leds
                leds();
                // Guardar Configuración
                if (settingsSaveWiFi()) {
                    request->send(200, "text/html", SweetAlert("WiFi", "Configuración WiFi Guardada", "La configuración WiFi ha sido guardada correctamente", "success", "accion"));
                    return;
                } else {
                    request->send(200, "text/html", SweetAlert("WiFi", "Error al guardar la configuración", "No se han podido guardar las nuevas configuraciones WiFi", "error", "aviso"));
                    return;
                }
                
              });
         
    // --------------------------------------------------------
    // Cargar Página restore.html
    // url: /esp-restore
    // GET
    // --------------------------------------------------------
    server.on("/esp-restore", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }

                  File file = SPIFFS.open(F("/restore.html"), "r");
                  if (file)
                  {
                      file.setTimeout(100);
                      String s = file.readString();
                      file.close();
                      s.replace(F("#platform#"), plafform());

                      // Send data
                      request->send(200, "text/html", s);
                  }
                  else
                  {
                      request->send(500, "text/plain", "restore.html not found, have you flashed the SPIFFS?");
                  } });

    // --------------------------------------------------------
    // Cargar Página restart.html
    // url: /esp-restart
    // GET
    // --------------------------------------------------------
    server.on("/esp-restart", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }

                  File file = SPIFFS.open(F("/restart.html"), "r");
                  if (file)
                  {
                      file.setTimeout(100);
                      String s = file.readString();
                      file.close();
                      s.replace(F("#platform#"), plafform());

                      // Send data
                      request->send(200, "text/html", s);
                  }
                  else
                  {
                      request->send(500, "text/plain", "restart.html not found, have you flashed the SPIFFS?");
                  } });

    // --------------------------------------------------------
    // Cargar Página admin.html
    // url: /esp-admin
    // GET
    // --------------------------------------------------------
    server.on("/esp-admin", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }

                  File file = SPIFFS.open(F("/admin.html"), "r");
                  if (file)
                  {
                      file.setTimeout(100);
                      String s = file.readString();
                      file.close();
                      s.replace(F("#platform#"), plafform());

                      // Send data
                      request->send(200, "text/html", s);
                  }
                  else
                  {
                      request->send(500, "text/plain", "admin.html not found, have you flashed the SPIFFS?");
                  } });

    // --------------------------------------------------------
    // Cargar Página admin.html
    // url: /esp-admin
    // POST

    server.on("/esp-admin", HTTP_POST, [](AsyncWebServerRequest *request)
              { 
                    AsyncResponseStream *response;
                    if(false == requestPreProcess(request, response))
                    {
                        return;
                    }
    if (request->params() == 5) {
        String u, p, nu, np, cp;
        if(request->hasArg("www_username")) u = request->arg("www_username");
        if(request->hasArg("www_password")) p = request->arg("www_password");
        if(request->hasArg("new_www_username")) nu = request->arg("new_www_username");
        if(request->hasArg("new_www_password")) np = request->arg("new_www_password");
        if(request->hasArg("c_new_www_password")) cp = request->arg("c_new_www_password");
        u.trim(); p.trim(); nu.trim(); np.trim(); cp.trim();
        log("Info: Datos recibidos - Usuario: " + u + ", Contraseña: " + p + ", Nuevo Usuario: " + nu + ", Nueva Contraseña: " + np + ", Confirmación Nueva Contraseña: " + cp);
        if (u != "" && p != "") {
            // Validar credenciales actuales
            if (u == www_username && p == www_password) {

                // Caso 1: actualizar solo usuario
                if (nu != "" && np == "" && cp == "") {
                    if (nu == www_username) {
                        request->send(200, "text/html", SweetAlert("Información", "Información", "El nuevo usuario no puede ser igual al anterior", "info", "aviso"));
                        return;
                    }
                    strlcpy(www_username, nu.c_str(), sizeof(www_username));
                    leds();
                    if (settinsSaveAdmin()) {
                        request->send(200, "text/html", SweetAlert("Éxito", "Usuario actualizado", "El nuevo usuario ha sido guardado correctamente", "success", "aviso"));
                    } else {
                        request->send(200, "text/html", SweetAlert("Error", "Error al guardar el usuario", "No se han podido guardar las nuevas credenciales", "error", "aviso"));
                    }
                    return;
                }

                // Caso 2: actualizar solo contraseña
                if (nu == "" && np != "" && cp != "" && np == cp) {
                    if (np == www_password) {
                        request->send(200, "text/html", SweetAlert("Información", "Información", "La nueva contraseña no puede ser igual a la anterior", "info", "aviso"));
                        return;
                    }
                    strlcpy(www_password, np.c_str(), sizeof(www_password));
                    leds();
                    if (settinsSaveAdmin()) {
                        request->send(200, "text/html", SweetAlert("Éxito", "Contraseña actualizada", "La nueva contraseña ha sido guardada correctamente", "success", "aviso"));
                    } else {
                        request->send(200, "text/html", SweetAlert("Error", "Error en los datos nuevos", "Contraseña no actualizada, fallo en escritura SPIFF", "error", "aviso"));
                    }
                    return;
                }

                // Caso 3: actualizar usuario y contraseña
                if (nu != "" && np != "" && cp != "" && np == cp) {
                    if (nu == www_username) {
                        request->send(200, "text/html", SweetAlert("Información", "Información", "El nuevo usuario no puede ser igual al anterior", "info", "aviso"));
                        return;
                    }
                    if (np == www_password) {
                        request->send(200, "text/html", SweetAlert("Información", "Información", "La nueva contraseña no puede ser igual a la anterior", "info", "aviso"));
                        return;
                    }
                    strlcpy(www_username, nu.c_str(), sizeof(www_username));
                    strlcpy(www_password, np.c_str(), sizeof(www_password));
                    leds();
                    if (settinsSaveAdmin()) {
                        request->send(200, "text/html", SweetAlert("Éxito", "Credenciales actualizadas", "Las nuevas credenciales han sido guardadas correctamente", "success", "aviso"));
                    } else {
                        request->send(200, "text/html", SweetAlert("Error", "Error al guardar las credenciales", "No se han podido guardar las nuevas credenciales, fallo en escritura SPIFF", "error", "aviso"));
                    }
                    return;
                }

                // Si no entra en ningún caso válido
                request->send(200, "text/html", SweetAlert("Error", "Error en los datos nuevos", "La nueva contraseña no coincide con la confirmación o hay campos vacíos", "error", "aviso"));
                return;

            } else {
                request->send(200, "text/html", SweetAlert("Error", "Credenciales incorrectas", "Verifique sus credenciales", "error", "aviso"));
            }
        } else {
            request->send(200, "text/html", SweetAlert("Error", "Error en los datos recibidos", "No se han recibido los datos necesarios", "error", "aviso"));
        }
    } else {
        request->send(200, "text/html", SweetAlert("Error", "Error de parámetros", "Error", "error", "aviso"));
    } });


    //--------------------------------------------------------
    // Gurdar el ID del dispositivo
    // url: /eps-divice
    // POST
    server.on("/esp-device", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }


            if (request->params() == 1){
                String s;
                if(request->hasArg("id"))
                s = request->arg("id");
                s.trim();
                if (s != "") {
                    strlcpy(id, s.c_str(), sizeof(id));
                    leds();
                    if (settingsSaveWiFi()) {
                        request->send(200, "text/html", SweetAlert("ID", "ID actualizado", "El nuevo ID ha sido guardado correctamente", "success", "aviso"));
                        return;
                    } else {
                        request->send(200, "text/html", SweetAlert("Error", "Error al guardar el ID", "No se han podido guardar el nuevo ID, fallo en escritura SPIFF", "error", "aviso"));
                        return;
                    }
                } else {
                    request->send(200, "text/html", SweetAlert("Warning", "Warning", "No se ha recibido un ID válido", "error", "aviso"));
                    return;
                }

            }else{
                request->send(200, "text/html", SweetAlert("Error", "Error de parámetros", "Error", "error", "aviso"));
                return;
            }
            });

    // --------------------------------------------------------
    // Error página no encontradsa eror_404.html
    // url: "desconocida"
    // GET
    // --------------------------------------------------------
    server.onNotFound([](AsyncWebServerRequest *request)
          {
                    AsyncResponseStream *response;
        if(false == requestPreProcess(request, response))
        {
            return;
        }


        log("Info: Página no encontrada: " + request->url());

        if (request->method() == HTTP_GET){
            log("Info: Método GET");
        } else if (request->method() == HTTP_POST){
            log("Info: Método POST");
        }else if (request->method() == HTTP_DELETE){
            log("Info: Método DELETE"); 
        }else if (request->method() == HTTP_PUT){
            log("Info: Método PUT");
        }else if (request->method() == HTTP_PATCH){
            log("Info: Método PATCH");
        }else if (request->method() == HTTP_HEAD){
            log("Info: Método HEAD");
        }else if (request->method() == HTTP_OPTIONS){
            log("Info: Método OPTIONS");
        }else   {
            log("Info: Método desconocido");
        }
        log("Info: http://" + request->host() + request->url());

        if(request->contentLength()){
            log("Info: _CONTENT_TYPE" + request->contentType());
            log("Info: _CONTENT-LENGTH" + request->contentLength());
        }


        int headers = request->headers();
        int i;
        for(i=0; i<headers; i++){
            AsyncWebHeader* h = request->getHeader(i);
            log("Infor: _HEADER[]" + h->name() + h->value());
        }


        int params = request->params();
        for (i = 0; i < params; i++)
        {
            AsyncWebParameter* p = request->getParam(i);
            if (p->isFile())
            {
                log("Info: _FILE[]: , size: " + p->name() + " : " + p->value() + " : " + p->size());
            }
            else if (p->isPost())
            {
                log("Info: _POST[]" + p->name() + " : " + p->value());
            }
            else
            {
                log("Info: _GET[]" + p->name() + " : " + p->value());
            }
        }
        

                      File file = SPIFFS.open(F("/error_404.html"), "r");
                      if (file)
                      {
                          file.setTimeout(100);
                          String s = file.readString();
                          file.close();
                          
                          request->send(404, "text/html", s);
                      }
                      else
                      {
                          request->send(404, "text/plain", "Error 404 - Página no encontrada");
                      } });

    // --------------------------------------------------------
    //------------------Petición de Salida del Servidor--------
    // url: /esp-logout
    // GET
    // --------------------------------------------------------
    server.on("/esp-logout", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                AsyncResponseStream *response;
                if(false == requestPreProcess(request, response))
                {
                    return;
                }
                request->send(401, "text/html", SweetAlert("Logout", "Logout exitoso", "Has salido del panel de administración", "success", "logout"));
                  });


    /*
        Inicializar el servidor
    */
    server.begin();
    log("Info: Servidor HTTP iniciado");
}