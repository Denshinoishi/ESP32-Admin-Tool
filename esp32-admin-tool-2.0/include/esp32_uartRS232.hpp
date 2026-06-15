// esp32_uartRS232.hpp
#include <Arduino.h>
#include "driver/uart.h"


#define UART_NUM UART_NUM_2
#define BUF_SIZE 1024

void initRS232() {
    uart_config_t uart_config = {
        .baud_rate = RS232_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, RS232_TX, RS232_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, BUF_SIZE, 0, 0, NULL, 0);
}

// Tarea que lee datos del buffer UART
void rs232Task(void *pvParameters) {
    uint8_t data[BUF_SIZE];
    while (true) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE, 20 / portTICK_RATE_MS);
        if (len > 0) {
            String hexPayload;
            for (int i = 0; i < len; i++) {
                char buf[4];
                sprintf(buf, "%02X ", data[i]); // convierte a hex con espacio
                hexPayload += buf;
            }

            // Construir JSON
            StaticJsonDocument<256> doc;
            doc["type"] = "rs232";
            doc["msg"]  = hexPayload;
            doc["icon"] = "terminal";

            String jsonStr;
            serializeJson(doc, jsonStr);

            // Log para depuración
            log("RS232 JSON => " + jsonStr);

            // Enviar por WebSocket
            ws.textAll(jsonStr);
        }
    }
}