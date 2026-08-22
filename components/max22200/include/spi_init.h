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

extern spi_device_handle_t spi_handle;
extern SemaphoreHandle_t mutex;

esp_err_t max22200_init_hardware(void);
esp_err_t max22200_init_procedure(void);
esp_err_t channel_setup(uint8_t channel, bool hfs, uint8_t hold, bool trig_spi, uint8_t hit, uint8_t hit_time, bool current_voltage, uint8_t frequency, bool src_enable, bool ol_enable, bool dpm_enable, bool hhf_enable);

#endif //MAX22000_SPI