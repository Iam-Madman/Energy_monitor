#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/select.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"

#include"atm_uart.h"

int debug(int ch)
{
    if (ch == '\n') {
        const char cr = '\r';
        uart_tx_chars(UART_NUM_1, &cr, 1);
    }
    char c = ch;
    uart_tx_chars(UART_NUM_1, &c, 1);
    return ch;
}

void app_main()
{
    esp_log_set_putchar(debug);
    ESP_LOGI("DEBUG", "This will go out through UART1 TX (GPIO2)");

}