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

void max22200_set_channel_state(uint8_t channel, bool enable) {
    if (channel > 7) {
        ESP_LOGE("BLAD", "niepoprawny kanal");
        return;
    }
    uint32_t status_val = max22200_read(MAX22200_ADDR_STATUS);

    if(enable) {
        status_val |= (0x01 << (24 + channel));
    } else {
        status_val &= ~(0x01 << (24 + channel));
    }

    max22200_write(MAX22200_ADDR_STATUS, status_val);
}
//write = 1, read = 0, 8bit = 1, 32bit = 0
uint8_t build_cmd_byte(bool write_or_read, uint8_t ch_addr, bool spi_size) {
    return ((write_or_read ? MAX22200_CMD_WRITE  : MAX22200_CMD_READ) | (ch_addr & 0x0F) << MAX22200_CMD_ADDR_POS | (spi_size ? MAX22200_CMD_8BIT_MODE   : MAX22200_CMD_32BIT_MODE ));
}

void max22200_write_command(uint8_t cmd_byte) {
    gpio_set_level(PIN_CMD, 1);
    spi_transaction_t t = { .length = 8, .tx_buffer = &cmd_byte, .rx_buffer = NULL};
    spi_device_polling_transmit(spi_handle, &t);
    gpio_set_level(PIN_CMD, 0);
}