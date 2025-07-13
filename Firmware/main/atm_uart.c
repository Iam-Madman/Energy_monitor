#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/select.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"

#include"atm_uart.h"

void write_reg(uint8_t reg, uint16_t value)
{
    uint8_t tx[6];
    tx[0] = FE_HEADER;
    tx[1] = reg & 0x7F; // Write operation (MSB = 0)
    tx[2] = (value >> 8) & 0xFF; // MSB
    tx[3] = value & 0xFF;        // LSB
    tx[4] = tx[1] + tx[2] + tx[3]; // Checksum = reg + MSB + LSB

    for (int i = 0; i < 5; i++) {
        uart_write_bytes(UART_NUM_0, (const char *)&tx[i], 1);
    }
}

uint16_t read_reg(uint8_t reg, uint16_t *value)
{
    uint8_t tx[3];
    uint8_t rx[3];

    tx[0] = FE_HEADER;
    tx[1] = reg | 0x80; // Read operation (MSB = 1)
    tx[2] = tx[1]; // Checksum = reg
    uart_write_bytes(UART_NUM_0, (const char *)tx, 3);

    uart_read_bytes(UART_NUM_0, rx, 3, TickType_t 5);

    
    if (rx[0] == FE_HEADER && (rx[1] & 0x7F) == reg && checksum == tx[2]) {
        *value = (rx[2] << 8) | rx[3]; // Combine MSB and LSB
    } else {
        ESP_LOGE("UART", "Read error or invalid response");
    }
}


static void uart_begin(void *pvParameters)
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0 
    };

    uart_enable_swap(); 
    uart_param_config(UART_NUM_0, &uart_config);   
    uart_driver_install(UART_NUM_0, 2*1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_driver_install(UART_NUM_1, 2*1024, 0, 0, NULL, 0);
    esp_vfs_dev_uart_use_driver(0);
    esp_vfs_dev_uart_use_driver(1);
    vTaskDelete(NULL);
}