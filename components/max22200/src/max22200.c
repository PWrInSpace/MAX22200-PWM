#include "max22200.h"
#include "max22200_regs.h"
#include "spi_init.h"
#include "esp_log.h"

static spi_device_handle_t spi_handle;

void max22200_write(uint8_t channel, uint32_t val) {
    uint8_t cmd = (1 << 7) | (channel << 1); 
    uint8_t data[4] = { (val >> 24) & 0xFF, (val >> 16) & 0xFF, (val >> 8) & 0xFF, val & 0xFF };

    gpio_set_level(PIN_CMD, 1);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t1 = { .length = 8, .tx_buffer = &cmd };
    spi_device_polling_transmit(spi_handle, &t1);
    gpio_set_level(PIN_CS, 1);

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 32, .tx_buffer = data };
    spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);
}

uint32_t max22200_read(uint8_t channel) {
    uint8_t cmd = (channel << 1); 
    uint8_t rx[4] = {0};

    gpio_set_level(PIN_CMD, 1);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t1 = { .length = 8, .tx_buffer = &cmd };
    spi_device_polling_transmit(spi_handle, &t1);
    gpio_set_level(PIN_CS, 1);

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 32, .rx_buffer = rx };
    spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);

    return ((uint32_t)rx[0] << 24) | ((uint32_t)rx[1] << 16) | ((uint32_t)rx[2] << 8) | rx[3];
}