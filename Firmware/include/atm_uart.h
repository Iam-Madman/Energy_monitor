#ifndef atm_uart_h
#define atm_uart_h

#define FE_HEADER 0xFE

void write_reg(uint8_t reg, uint16_t value);
uint16_t read_reg(uint8_t reg, uint16_t *value);
static void uart_begin(void *pvParameters);


#endif