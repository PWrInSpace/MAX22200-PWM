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

typedef enum {
    PAIR_MODE_INDEPENDENT = 0,
    PAIR_MODE_PARALLEL = 1,
    PAIR_MODE_FULL_BRIDGE = 2
} channel_pair_mode_t;

typedef struct {
    channel_pair_mode_t pair_01; // Konfiguracja kanałów 0 i 1
    channel_pair_mode_t pair_23; // Konfiguracja kanałów 2 i 3
    channel_pair_mode_t pair_45; // Konfiguracja kanałów 4 i 5
    channel_pair_mode_t pair_67; // Konfiguracja kanałów 6 i 7
} MAX22200_board_config_t;

esp_err_t max22200_init_hardware(void);
esp_err_t max22200_init_procedure(void);
esp_err_t channel_setup(uint8_t channel, bool hfs, uint8_t hold, bool trig_spi, uint8_t hit, uint8_t hit_time, bool current_or_voltage, bool high_or_low_side, uint8_t frequency, bool src_enable, bool ol_enable, bool dpm_enable, bool hhf_enable);
void status_channel_mode_setup(uint32_t* status_val, MAX22200_board_config_t* config);

#endif //MAX22000_SPI