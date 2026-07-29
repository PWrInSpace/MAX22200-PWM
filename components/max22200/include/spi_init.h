#ifndef MAX22000_SPI
#define MAX22000_SPI

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
void max22200_init_procedure(void);

#endif //MAX22000_SPI