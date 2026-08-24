#include "spi_init.h"
#include "max22200.h"
#include "max22200_regs.h"
#include <unistd.h>
#include "esp_log.h"

spi_device_handle_t spi_handle = NULL;
SemaphoreHandle_t mutex;



esp_err_t max22200_init_hardware(void) {
    //GPIO
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PIN_CS) | (1ULL << PIN_CMD) | (1ULL << PIN_ENABLE)
    };
    esp_err_t ret = gpio_config(&io_conf);
    if(ret != ESP_OK) {
        return ret;
    }

    gpio_set_level(PIN_CS, 1);     
    gpio_set_level(PIN_CMD, 0);    
    gpio_set_level(PIN_ENABLE, 0);
    //SPI
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_MOSI, .miso_io_num = PIN_MISO, .sclk_io_num = PIN_CLK,
        .quadwp_io_num = -1, .quadhd_io_num = -1, .max_transfer_sz = 32
    };
    ret = spi_bus_initialize(SPI3_HOST, &bus_cfg, SPI_DMA_DISABLED);
    if(ret != ESP_OK) {
        return ret;
    }

    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 2000000,
        .mode = 0, 
        .spics_io_num = -1, // CS sterujemy ręcznie
        .queue_size = 1
    };
    ret = spi_bus_add_device(SPI3_HOST, &dev_cfg, &spi_handle);
    if(ret != ESP_OK) {
        return ret;
    }

    ret = max22200_init_procedure();
    if(ret != ESP_OK) {
        return ret;
    }
    
    mutex = xSemaphoreCreateMutex();

    return ret;
}

esp_err_t max22200_init_procedure(void) {
    gpio_set_level(PIN_ENABLE, 1);
    usleep(500);
    uint32_t status_val;
    esp_err_t ret = max22200_read_32bit(MAX22200_ADDR_STATUS, &status_val);
    if(ret != ESP_OK) {
        return ret;
    }

    if(status_val == 0x00000000 || status_val == 0xFFFFFFFF) {
        return ESP_ERR_INVALID_STATE;
    }
    
    MAX22200_board_config_t channel_mode_setup;
    channel_mode_setup.pair_01 = PAIR_MODE_INDEPENDENT;
    channel_mode_setup.pair_23 = PAIR_MODE_INDEPENDENT;
    channel_mode_setup.pair_45 = PAIR_MODE_INDEPENDENT;
    channel_mode_setup.pair_67 = PAIR_MODE_INDEPENDENT; //67
    status_channel_mode_setup(&status_val, &channel_mode_setup);

    status_val = MAX22200_STATUS_ACTIVE;
    ret = max22200_write_32bit(MAX22200_ADDR_STATUS, status_val);
    if(ret != ESP_OK) {
        return ret;
    }

    //ustawic sensowne wartosci, takie jakie beda potrzebne
    channel_setup(MAX22200_ADDR_CH0, 1, 127, 1, 127, 255, 1, 0, 3, 1, 1, 0, 0);
    channel_setup(MAX22200_ADDR_CH1, 1, 127, 1, 127, 255, 1, 0, 3, 1, 1, 0, 0);
    channel_setup(MAX22200_ADDR_CH2, 1, 127, 1, 127, 255, 1, 0, 3, 1, 1, 0, 0);
    channel_setup(MAX22200_ADDR_CH3, 1, 127, 1, 127, 255, 1, 0, 3, 1, 1, 0, 0);
    channel_setup(MAX22200_ADDR_CH4, 1, 127, 1, 127, 255, 1, 0, 3, 1, 1, 0, 0);
    channel_setup(MAX22200_ADDR_CH5, 1, 127, 1, 127, 255, 1, 0, 3, 1, 1, 0, 0);
    channel_setup(MAX22200_ADDR_CH6, 1, 127, 1, 127, 255, 1, 0, 3, 1, 1, 0, 0);
    channel_setup(MAX22200_ADDR_CH7, 1, 127, 1, 127, 255, 1, 0, 3, 1, 1, 0, 0);


    ret = max22200_read_32bit(MAX22200_ADDR_STATUS, &status_val);
    if(ret != ESP_OK) {
        return ret;
    }
    uint8_t last_cmd_status_byte = status_val & 0xFF;
    // Sprawdzenie czy flaga UVM zniknęła oraz czy brak błędu komunikacji COM_ERR (0x04)
    if ((status_val & MAX22200_STATUS_UVM) || (last_cmd_status_byte == 0x04)) {
        ESP_LOGE("BLAD", "Wykryto flaga blędu! UVM=%ld, Status Byte=0x%02X\n",
               (status_val & MAX22200_STATUS_UVM) >> 1, last_cmd_status_byte);
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGE("MAX22200 OK", "Inicjalizacja zakończona SUKCESEM! Uklad jest gotowy.\n");
    return ret;

}
//hfs: 1 - half full scale, 0 - full scale; 
esp_err_t channel_setup(uint8_t channel, bool hfs, uint8_t hold, bool trig_spi, uint8_t hit, uint8_t hit_time, bool current_or_voltage, bool high_or_low_side, uint8_t frequency, bool src_enable, bool ol_enable, bool dpm_enable, bool hhf_enable) {
    if(channel > 8 || channel == 0) {
        return ESP_ERR_INVALID_ARG;
    }
    uint32_t setup_val = (MAX22200_HFS_MASK & hfs) << MAX22200_HFS_POS | (MAX22200_HOLD_MASK & hold) << MAX22200_HOLD_POS | (MAX22200_TRGNSP_IO_MASK & trig_spi) << MAX22200_TRGNSP_IO_POS | (MAX22200_HIT_MASK & hit) << MAX22200_HIT_POS | (MAX22200_HIT_T_MASK & hit_time) << MAX22200_HIT_T_POS | (MAX22200_VDRNCDR_MASK & current_or_voltage) << MAX22200_VDRNCDR_POS | (MAX22200_HSNLS_MASK & high_or_low_side) << MAX22200_HSNLS_POS | (MAX22200_FREQ_CFG_MASK & frequency) << MAX22200_FREQ_CFG_POS | (MAX22200_SRC_MASK & src_enable) << MAX22200_SRC_POS | (MAX22200_OL_EN_MASK & ol_enable) << MAX22200_OL_EN_POS | (MAX22200_DPM_EN_MASK & dpm_enable) << MAX22200_DPM_EN_POS | (MAX22200_HHF_EN_MASK & hhf_enable) << MAX22200_HHF_EN_POS;

    esp_err_t ret = max22200_write_32bit(channel, setup_val);
    if(ret != ESP_OK) {
        ESP_LOGE("blad", "kanal %ld nie zostal ustawiony poprawnie", channel);
        return ret;
    }

    return ret;
}

void status_channel_mode_setup(uint32_t *status_val, MAX22200_board_config_t *config) {
    *status_val |= ((config->pair_01 & 0x03) << 8);
    *status_val |= ((config->pair_23 & 0x03) << 10);
    *status_val |= ((config->pair_45 & 0x03) << 12);
    *status_val |= ((config->pair_67 & 0x03) << 14);
}