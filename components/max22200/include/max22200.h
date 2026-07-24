#ifndef MAX22200_H
#define MAX22200_H

#include "esp_err.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


void max22200_write(uint8_t channel, uint32_t val);
uint32_t max22200_read(uint8_t channel);


#endif //MAX22200_H