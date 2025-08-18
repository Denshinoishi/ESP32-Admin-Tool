#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h>



const byte DNSSERVER_PORT = 53;
DNSServer dnsServer;

IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

int wifi_mode = WIFI_STA;

unsigned long previousMillisWIFI = 0;
unsigned long intervalWIFI = 30000;

//hostname for ESPmDNS. Should work at least on Windows. Try http://adminesp32.local
const char *esp_hostname = id;

void starAP(){
    log("Info: Inciando Modo Acces Point");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(1000);
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.setHostname(deviceID().c_str());
    WiFi.softAP(ap_name, ap_passw, ap_canal , ap_hidden, ap_connect);
    log("Info: WiFi AP " + deviceID() + " - IP " + ipStr(WiFi.softAPIP()));
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(DNSSERVER_PORT, "*", apIP);
    wifi_mode = WIFI_AP;
}

void startClient(){
    if (wifi_ip_static)
    {
        if (!WiFi.config(charToIP(wifi_ip_static), charToIP(wifi_gateway), charToIP(wifi_subnet), charToIP(wifi_primaryDNS), charToIP(wifi_secondaryDNS)))
        {
            log("Error: Falló al conectar al modo Estación");
        }
        
    }
    WiFi.hostname(deviceID());
    WiFi.begin(wifi_ssid, wifi_passw);
    log("Info: Conectado WiFi" + String(wifi_ssid));
    delay(50);
    byte b = 0;
    while (WiFi.status() != WL_CONNECTED && b < 60)
    {
        b++;
        log("Advertencia: Intentando Conexión WiFi...");
        delay(500);
        // conmutar led WIFI cuando está conectandose al wifi no bloqueante
        // conmutación simple del led cada 100 ms
        blinkSingle(100, WIFILED);
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        /* WiFi Station Conectado */
        log("Info: WiFi conectado (" + String(WiFi.RSSI()) + ") IP " +  ipStr(WiFi.localIP()));
        //conmutación Ramdon del led
        blinkRandomSingle(10, 100, WIFILED);
        delay(100);
    }else{
        //WiFi Station no Conectado
        log("Erro: WiFi no conectado");
        //conmutación Ramdon del led
        blinkRandomSingle(10, 100, WIFILED);
        delay(100);
    }
    
    
}


void wifi_setup(){
    // Modo AP
    if (ap_accesPoint)
    {
        starAP();
        log("Info: WiFi modo AP");
    }
    // Modo Estación
    else{
        WiFi.mode(WIFI_STA);
        wifi_mode = WIFI_STA;
        startClient();
        log("Info: WiFi modo Estación");

    }
    // Iniciar hostname broadcast en modo STA o AP
    if (wifi_mode == WIFI_STA || wifi_mode == WIFI_AP)
    {
        if (MDNS.begin(esp_hostname))
        {
            MDNS.addService("http", "tcp", 80);

        }
        
    }
    

}


void wifiLoop(){
    unsigned long currenMillis = millis();

    if (WiFi.status() != WL_CONNECTED && (currenMillis - previousMillisWIFI >= intervalWIFI))
    {
        // Para conmutar un led cuando está conectandose al WIFI no bloqueante
        // Conmutación simple cada 100 ms
        blinkSingle(100, WIFILED);
 
        WiFi.disconnect();
        WiFi.reconnect();
        previousMillisWIFI = currenMillis;

    }else{
        // Conmutación del led Tiempo On y Off
        blinkSingleAsy(10, 500, WIFILED);
 
    }
    
}


void wifiAPLopp(){
    // Conmuta con tiempo Variable Transferencia de Datos
    blinkRandomSingle(50, 100, WIFILED);
    dnsServer.processNextRequest();
}