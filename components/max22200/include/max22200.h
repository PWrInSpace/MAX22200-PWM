#ifndef MAX22200_H
#define MAX22200_H

#include "esp_err.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

//piny, dopasować przed uruchomieniem
#define PIN_MISO   19
#define PIN_MOSI   23
#define PIN_CLK    18
#define PIN_CS     5
#define PIN_CMD    4
#define PIN_ENABLE 2

void max22200_init_hardware(void);
void max22200_write(uint8_t channel, uint32_t val);
uint32_t max22200_read(uint8_t channel);


#endif //MAX22200_H