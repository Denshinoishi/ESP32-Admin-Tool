#include "ledBlink.hpp"
#include <esp32-hal.h>






/*
Genera un log en el puerto serial
*/

void log(String s){
    Serial.println(s);
}


/* Definir la Plataforma*/
String plafform(){
    #ifdef ARDUINO_ESP32_DEV
        return "ESP32";
    #endif
}


/*
    Convierte un char a IP
*/

IPAddress charToIP(const char *str){
    sscanf(str, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
    return  IPAddress(ip[0], ip[1], ip[2], ip[3]);
}

/*
    Retorna Segundos como "d:hh:mm:ss"
*/

String longTimeStr(const time_t &t){
    String s = String(t / SECS_PER_DAY) + ':';
    if (hour(t) < 10)
    {
        s += '0';
    }
    s += String(hour(t)) + ':';
    if (minute(t) < 10)
    {
        s += '0';
    }
    s += String(minute(t)) + ':';
    if (second(t) < 10)
    {
        s += '0';
    }
    s += String(second(t));
    return s;    
}




/* Retorna IPAddres en formato "n.n.n.n" de IP a String*/
String ipStr(const IPAddress &ip){
    String sFn = "";
    for (byte bFn = 0; bFn < 3; bFn++)
    {
        sFn += String((ip >> (8 * bFn)) & 0xFF) + ".";
    }
    sFn += String(((ip >> 8 * 3)) & 0xFF);
    return sFn;
}




/* Hex to String*/
String hexStr(const unsigned long &h, const byte &l =  8){
    String s;
    s = String(h, HEX);
    s.toUpperCase();
    s = ("00000000" + s).substring(s.length() + 8 - l);
    return s;
}


/*Crea un ID único desde la diredcción MAC*/

String idUnique(){
    // Retorna los últimos 4 Bytes del MAC rotados
    char idunique[15];
    uint64_t chipid = ESP.getEfuseMac();
    uint16_t chip = (uint16_t)(chipid >> 32);
    sniprintf(idunique, 15, "%04X", chip);
    return idunique;
}

/*
    ID Del dispositivo para la base de datos
*/
const String device_id = hexStr(ESP.getEfuseMac())  + "CE" + String(idUnique());



String deviceID(){
    return String(plafform()) + hexStr(ESP.getEfuseMac()) + String(idUnique());
}

/* Pin Out*/

void setupPinOut(){

    pinMode(WIFILED, OUTPUT);
    pinMode(MQTTLED, OUTPUT);
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);

    setOffSingle(WIFILED);
    setOffSingle(MQTTLED);
    setOffSingle(RELAY1);
    setOffSingle(RELAY2);
}










/*  RS232   */

void setupSerialPort() {
    Serial2.begin(9600, SERIAL_8N1, RS232_RX, RS232_TX);
}


/*
Calidad WIFI
*/

int getRSSIasQuality(int RSSI){
    int quality = 0;
    if (RSSI <= -100)
    {
        quality = 0;
    }else if (RSSI >= -50)
    {
        quality = 100;
    }else{
        quality = 2 * (RSSI + 100);
    }
    
    return quality;
}



/*
    Temperatura CPU
*/

float TempCPUValue(){
    return temperatureRead(); // ya viene definido
}


/*
    Retorna el listado de todos los archivos en el SPIFFS
*/

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("List Directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        log(" - not a directory");
        return;
    }
    File file = root.openNextFile();
    while (file)
    {
        if(file.isDirectory()){
            log(" DIR : " + String(file.name()));

            if (levels)
            {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            log(" FILE : " + String(file.name()));
            log("\tSIZE : " + String(file.size()));
        }
        file = root.openNextFile();
    }
    log(" Total Space: " + String(SPIFFS.totalBytes()));
    log(" Total Space used: " + String(SPIFFS.usedBytes()));

    
}


// Convierte un char en IP

IPAddress StrIP(const char *str){
    sscanf(str, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
    return IPAddress(ip[0], ip[1], ip[2], ip[3]);
}


//---------------------------------------------------------
/********** FUNCION PARA RETORNO DE SWEETALERT2 **********/
String SweetAlert(String TitleWeb, String SweetTitle, String SweetText, String SweetIcon, String type){
    String SweetAlert;
    if (type == "aviso")
    {
        SweetAlert =         "<html>"
        "<head>"
        "<meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<link rel='icon' href='/www/esp32.png' type='image/x-icon'>"
        "<link rel='stylesheet' href='/www/bootstrap.css'>"
        "<link rel='stylesheet' href='/www/main.css'>"
        "<link rel='stylesheet' href='/www/MoneAdmin.css'>"
        "<link rel='stylesheet' href='/www/font-awesome.css'>"
        "<link rel='stylesheet' href='/www/sweetalert2.min.css'>"
        "<script src='/www/jquery-2.0.3.min.js'></script>"
        "<script src='/www/bootstrap.min.js'></script>"
        "<script src='/www/modernizr-2.6.2.min.js'></script>"
        "<script src='/www/sweetalert2.min.js'></script>"
        "<title>"+ TitleWeb +"</title>"
        "</head>"
        "<body>"
        "<script>"
        "Swal.fire({"
            "icon: '"+ SweetIcon +"',"
            "title: '"+ SweetTitle +"',"
            "text: '"+ SweetText +"',"
            "confirmButtonText: 'Cerrar'"
        "}).then((result) => {"
            "if (result.isConfirmed) {"
                "history.back();"
            "}"
        "});"
        "</script>"
        "</body>"
        "</html>";
    } else if (type == "accion") {
    SweetAlert =
        "<html>"
        "<head>"
        "<meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<link rel='icon' href='/www/esp32.png' type='image/x-icon'>"
        "<link rel='stylesheet' href='/www/bootstrap.css'>"
        "<link rel='stylesheet' href='/www/main.css'>"
        "<link rel='stylesheet' href='/www/MoneAdmin.css'>"
        "<link rel='stylesheet' href='/www/font-awesome.css'>"
        "<link rel='stylesheet' href='/www/sweetalert2.min.css'>"
        "<script src='/www/jquery-2.0.3.min.js'></script>"
        "<script src='/www/bootstrap.min.js'></script>"
        "<script src='/www/modernizr-2.6.2.min.js'></script>"
        "<script src='/www/sweetalert2.min.js'></script>"
        "<title>" + TitleWeb + "</title>"
        "</head>"
        "<body>"
        "<script>"
        "Swal.fire({"
            "title: '" + SweetTitle + "!',"
            "text: '" + SweetText + "',"
            "icon: '" + SweetIcon + "',"
            "showCancelButton: true,"
            "confirmButtonColor: '#3085d6',"
            "cancelButtonColor: '#d33',"
            "confirmButtonText: 'Sí, reiniciar!',"
            "cancelButtonText: 'Cancelar',"
            "reverseButtons: true"
        "}).then((result) => {"
            "if (result.isConfirmed) {"
                "window.location.href = 'esp-restart';"
            "} else if (result.dismiss === Swal.DismissReason.cancel) {"
                "history.back();"
            "}"
        "});"
        "</script>"
        "</body>"
        "</html>";
} else {
    SweetAlert =
        "<html>"
        "<head>"
        "<meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<link rel='icon' href='/www/esp32.png' type='image/x-icon'>"
        "<link rel='stylesheet' href='/www/bootstrap.css'>"
        "<link rel='stylesheet' href='/www/main.css'>"
        "<link rel='stylesheet' href='/www/MoneAdmin.css'>"
        "<link rel='stylesheet' href='/www/font-awesome.css'>"
        "<link rel='stylesheet' href='/www/sweetalert2.min.css'>"
        "<script src='/www/jquery-2.0.3.min.js'></script>"
        "<script src='/www/bootstrap.min.js'></script>"
        "<script src='/www/modernizr-2.6.2.min.js'></script>"
        "<script src='/www/sweetalert2.min.js'></script>"
        "<title>" + TitleWeb + "</title>"
        "</head>"
        "<body>"
        "<script>"
        "Swal.fire({"
            "icon: '" + SweetIcon + "',"
            "title: '" + SweetTitle + "!',"
            "text: '" + SweetText + "',"
            "confirmButtonText: 'Cerrar'"
        "}).then((result) => {"
            "if (result.isConfirmed) {"
                "window.location.href = '/';"
            "}"
        "});"
        "</script>"
        "</body>"
        "</html>";
}
return SweetAlert;
}

//---------------------------------------------------------
//Control IO desde Websocket/MQTT
//----------------------------------------------------------
boolean settingsSaveRelays();

void OnOffRelays(String command){

    DynamicJsonDocument JsonDoc(1024);
    deserializeJson(JsonDoc, command);

    if(JsonDoc["protocol"] == "WS"){
        log("Info: Procesando Orden del Websocket: " + command);
    } else if (JsonDoc["protocol"] == "MQTT") {
        log("Info: Procesando Orden del MQTT: " + command);
    } else {
        log("Info: Procesando Orden Desconocida: " + command);
    }

    serializeJsonPretty(JsonDoc, Serial);

     if (JsonDoc["command"]){
        digitalWrite(JsonDoc["output"] == "RELAY1" ? RELAY1 : RELAY2, HIGH);
        JsonDoc["output"] == "RELAY1" ? relay_01_status = HIGH : relay_02_status = HIGH;
        
     } else {
        digitalWrite(JsonDoc["output"] == "RELAY1" ? RELAY1 : RELAY2, LOW);
        JsonDoc["output"] == "RELAY1" ? relay_01_status = LOW : relay_02_status = LOW;
        
     }

        settingsSaveRelays();
  

}