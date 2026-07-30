#ifndef MAX22200_H
#define MAX22200_H

#include "esp_err.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


void max22200_write(uint8_t channel, uint32_t val);
uint32_t max22200_read(uint8_t channel);
void max22200_set_channel_state(uint8_t channel, bool enable);

#endif //MAX22200_H