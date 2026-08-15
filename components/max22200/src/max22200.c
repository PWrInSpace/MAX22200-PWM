#include "max22200.h"
#include "max22200_regs.h"
#include "spi_init.h"
#include "esp_log.h"

static spi_device_handle_t spi_handle;

//32bit

void max22200_write_32bit(uint8_t channel, uint32_t val) {
    uint8_t cmd = build_cmd_byte(true, channel, false);
    uint8_t data[4] = { (val >> 24) & 0xFF, (val >> 16) & 0xFF, (val >> 8) & 0xFF, val & 0xFF };

    max22200_write_command(cmd);

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 32, .tx_buffer = data };
    spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);
}

uint32_t max22200_read_32bit(uint8_t channel) {
    uint8_t cmd = build_cmd_byte(false, channel, false);
    uint8_t rx[4] = {0};

    max22200_write_command(cmd);

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 32, .rx_buffer = rx };
    spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);

    return ((uint32_t)rx[0] << 24) | ((uint32_t)rx[1] << 16) | ((uint32_t)rx[2] << 8) | rx[3];
}

//8bit

void max22200_write_8bit(uint8_t channel, uint8_t val) {
    uint8_t cmd = build_cmd_byte(true, channel, false);
    uint8_t data[1] = { val & 0xFF };

    max22200_write_command(cmd);

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 8, .tx_buffer = data };
    spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);
}

uint8_t max22200_read_8bit(uint8_t channel) {
    uint8_t cmd = build_cmd_byte(false, channel, false);
    uint8_t rx[1] = {0};

    max22200_write_command(cmd);

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 8, .rx_buffer = rx };
    spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);

    return rx[0];
}

void max22200_set_channel_state(uint8_t channel, bool enable) {
    if (channel > 7) {
        ESP_LOGE("BLAD", "niepoprawny kanal");
        return;
    }
    uint32_t status_val = max22200_read_32bit(MAX22200_ADDR_STATUS);

    if(enable) {
        status_val |= (0x01 << (24 + channel));
    } else {
        status_val &= ~(0x01 << (24 + channel));
    }

    max22200_write_32bit(MAX22200_ADDR_STATUS, status_val);
}
//write = 1, read = 0, 8bit = 1, 32bit = 0
uint8_t build_cmd_byte(bool write_or_read, uint8_t ch_addr, bool spi_size) {
    return ((write_or_read ? MAX22200_CMD_WRITE  : MAX22200_CMD_READ) | (ch_addr & 0x0F) << MAX22200_CMD_ADDR_POS | (spi_size ? MAX22200_CMD_8BIT_MODE   : MAX22200_CMD_32BIT_MODE ));
}

void max22200_write_command(uint8_t cmd_byte) {
    gpio_set_level(PIN_CMD, 1);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t = { .length = 8, .tx_buffer = &cmd_byte, .rx_buffer = NULL};
    spi_device_polling_transmit(spi_handle, &t);
    gpio_set_level(PIN_CS, 1);
    gpio_set_level(PIN_CMD, 0);
}