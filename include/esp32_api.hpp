

 #include "ESPAsyncWebServer.h"


 /*
    Instancia de AsyncWebServer puerto 80
 */


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");
