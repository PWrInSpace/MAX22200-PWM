#include "max22200.h"
#include "max22200_regs.h"
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
    gpio_set_level(PIN_ENABLE, 1);
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
}
