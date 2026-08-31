#ifndef MAX22200_H
#define MAX22200_H

#include "esp_err.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

typedef struct {
    bool uvm;
    bool comer;
    bool dpm;
    bool hhf;
    bool olf;
    bool ocp;
    bool ovt;
} MAX22200_Status_flags_t;

esp_err_t max22200_write_32bit(uint8_t channel, uint32_t val);
esp_err_t max22200_read_32bit(uint8_t channel, uint32_t *output);
esp_err_t max22200_write_8bit(uint8_t channel, uint8_t val);
esp_err_t max22200_read_8bit(uint8_t channel, uint8_t *output);
void max22200_set_channel_state(uint8_t channel, bool enable);
uint8_t build_cmd_byte(bool write_or_read, uint8_t ch_addr, bool spi_size);
esp_err_t max22200_write_command(uint8_t cmd_byte);
esp_err_t max22200_change_channel_settings(uint8_t channel, uint8_t hit, uint8_t hit_time, uint8_t hold, uint8_t frequency, bool current_or_voltage, bool high_or_low_side);
MAX22200_Status_flags_t max22200_status_flag_read(bool log_flag_states);

#endif //MAX22200_H