#include "spi_init.h"
#include "max22200.h"
#include "max22200_regs.h"
#include <unistd.h>
#include "esp_log.h"

static spi_device_handle_t spi_handle;

void max22200_init_hardware(void) {
    //GPIO
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PIN_CS) | (1ULL << PIN_CMD) | (1ULL << PIN_ENABLE)
    };
    gpio_config(&io_conf);

    gpio_set_level(PIN_CS, 1);     
    gpio_set_level(PIN_CMD, 0);    
    gpio_set_level(PIN_ENABLE, 0);
    //SPI
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_MOSI, .miso_io_num = PIN_MISO, .sclk_io_num = PIN_CLK,
        .quadwp_io_num = -1, .quadhd_io_num = -1, .max_transfer_sz = 32
    };
    spi_bus_initialize(SPI3_HOST, &bus_cfg, SPI_DMA_DISABLED);

    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 2000000,
        .mode = 0, 
        .spics_io_num = -1, // CS sterujemy ręcznie
        .queue_size = 1
    };
    spi_bus_add_device(SPI3_HOST, &dev_cfg, &spi_handle);
    max22200_init_procedure();
}

void max22200_init_procedure(void) {
    gpio_set_level(PIN_ENABLE, 1);
    usleep(500);

    uint32_t status_val = max22200_read(MAX22200_ADDR_STATUS);

    if(status_val == 0x00000000 || status_val == 0xFFFFFFFF) {
        ESP_LOGE("BLAD", "STATUS niepoprawny. Sprawdź kable i zasilanie!\n");
    }

    status_val = MAX22200_STATUS_ACTIVE;
    max22200_write(MAX22200_ADDR_STATUS, status_val);

    //KROK 3 do zrobienia

    status_val = max22200_read(MAX22200_ADDR_STATUS);
    uint8_t last_cmd_status_byte = status_val & 0xFF;
    // Sprawdzenie czy flaga UVM zniknęła oraz czy brak błędu komunikacji COM_ERR (0x04)
    if ((status_val & MAX22200_STATUS_UVM) || (last_cmd_status_byte == 0x04)) {
        ESP_LOGE("BLAD", "Wykryto flaga błędu! UVM=%ld, Status Byte=0x%02X\n",
               (status_val & MAX22200_STATUS_UVM) >> 1, last_cmd_status_byte);
    }

    ESP_LOGE("MAX22200 OK", "Inicjalizacja zakończona SUKCESEM! Układ jest gotowy.\n");

}