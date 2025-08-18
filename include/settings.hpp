/*
Definiciones
*/

#define RELAY1     27       // GPIO27 para salida de Rele 1
#define RELAY2     26       // GPIO26 para salida de Rele 2
#define WIFILED    18       // GPIO12 para LED INDICADOR WIFI
#define MQTTLED    19       // GPIO13 para LED INDICADOR MQTT

#define RS232_TX   17       // GPIO17 como TX para RS232
#define RS232_RX   16       // GPIO16 como RX para RS232
#define RS232_BAUD 9600     // Velocidad de comunicación RS232

/*
Version de Hardware
*/
#define HW "ADMINESP32 v1 0000000" // Versión del Hardware

/*
Zona WIFI
*/
bool    wifi_staticIp;              //  Uso de IP Estática
char    wifi_ssid[30];              //  Red WIFI
char    wifi_passw[30];             //  Contraseña del WIFI
char    wifi_ip_static[15];         //  IP Estática
char    wifi_gateway[15];           //  Gateway
char    wifi_subnet[15];            //  Subred
char    wifi_primaryDNS[15];        //  DNS primario
char    wifi_secondaryDNS[15];      //  DNS secundario

//Configuraciones Generales
char    id[30];                     //  ID del dispositivo
int     bootCount;                  //  Número de reinicios

/*
Zona AP
*/
bool    ap_accesPoint;               //  Uso Modo AP
char    ap_name[31];                 //  SSID AP
char    ap_passw[63];                //  Pasword AP
int     ap_canal;                    //  Canal AP
int     ap_hidden;                   //  Visibilidad del AP (0 - Visible 1 - Oculto) 
int     ap_connect;                  //  Número de conexiones del AP
 /*
 Otros
 */
uint8_t ip[4];                      // Variabla función convertir char a IP