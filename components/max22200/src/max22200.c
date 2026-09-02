#include "max22200.h"
#include "max22200_regs.h"
#include "spi_init.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define MAX22200_TIMEOUT 100

//32bit

esp_err_t max22200_write_32bit(uint8_t channel, uint32_t val) {
    if(channel > 10) {
        return ESP_ERR_INVALID_ARG;
    }

    if(xSemaphoreTake(mutex, pdMS_TO_TICKS(MAX22200_TIMEOUT)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    uint8_t cmd = build_cmd_byte(true, channel, false);
    uint8_t data[4] = { (val >> 24) & 0xFF, (val >> 16) & 0xFF, (val >> 8) & 0xFF, val & 0xFF };

    esp_err_t ret = max22200_write_command(cmd);
    if(ret != ESP_OK) {
        xSemaphoreGive(mutex);
        return ret;
    }

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 32, .tx_buffer = data };
    ret = spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);

    xSemaphoreGive(mutex);
    return ret;
}

esp_err_t max22200_read_32bit(uint8_t channel, uint32_t *output) {
    if(channel > 10 || output == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if(xSemaphoreTake(mutex, pdMS_TO_TICKS(MAX22200_TIMEOUT)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }
    uint8_t cmd = build_cmd_byte(false, channel, false);
    uint8_t rx[4] = {0};

    esp_err_t ret = max22200_write_command(cmd);
    if(ret != ESP_OK) {
        xSemaphoreGive(mutex);
        return ret;
    }

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 32, .rx_buffer = rx };
    ret = spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);

    *output = ((rx[0] << 24) & 0xFF) | ((rx[1] << 16) & 0xFF) | ((rx[2] << 8) & 0xFF) | (rx[3] & 0xFF);

    xSemaphoreGive(mutex);
    return ret;
}

//8bit

esp_err_t max22200_write_8bit(uint8_t channel, uint8_t val) {
    if(channel > 10) {
        return ESP_ERR_INVALID_ARG;
    }

    if(xSemaphoreTake(mutex, pdMS_TO_TICKS(MAX22200_TIMEOUT)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }
    uint8_t cmd = build_cmd_byte(true, channel, true);
    uint8_t data[1] = { val & 0xFF };

    esp_err_t ret = max22200_write_command(cmd);
    if(ret != ESP_OK) {
        xSemaphoreGive(mutex);
        return ret;
    }

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 8, .tx_buffer = data };
    ret = spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);

    xSemaphoreGive(mutex);
    return ret;
}

esp_err_t max22200_read_8bit(uint8_t channel, uint8_t *output) {
    if(channel > 10 || output == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if(xSemaphoreTake(mutex, pdMS_TO_TICKS(MAX22200_TIMEOUT)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }
    uint8_t cmd = build_cmd_byte(false, channel, true);
    uint8_t rx[1] = {0};

    esp_err_t ret = max22200_write_command(cmd);
    if(ret != ESP_OK) {
        xSemaphoreGive(mutex);
        return ret;
    }

    gpio_set_level(PIN_CMD, 0);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t2 = { .length = 8, .rx_buffer = rx };
    ret = spi_device_polling_transmit(spi_handle, &t2);
    gpio_set_level(PIN_CS, 1);

    *output = rx[0] & 0xFF;

    xSemaphoreGive(mutex);
    return ret;
}

void max22200_set_channel_state(uint8_t channel, bool enable) {
    if (channel > 7) {
        ESP_LOGE("BLAD", "niepoprawny kanal");
        return;
    }
    uint32_t status_val;
    esp_err_t err = max22200_read_32bit(MAX22200_ADDR_STATUS, &status_val);

    if(enable) {
        status_val |= (0x01 << (24 + channel));
    } else {
        status_val &= ~(0x01 << (24 + channel));
    }

    max22200_write_32bit(MAX22200_ADDR_STATUS, status_val);
}
//write = 1, read = 0, 8bit = 1, 32bit = 0
uint8_t build_cmd_byte(bool write_or_read, uint8_t ch_addr, bool spi_size) {
    return ((write_or_read ? MAX22200_CMD_WRITE  : MAX22200_CMD_READ) | (ch_addr & 0x0F) << MAX22200_CMD_ADDR_POS | (spi_size ? MAX22200_CMD_8BIT_MODE   : MAX22200_CMD_32BIT_MODE ));
}

esp_err_t max22200_write_command(uint8_t cmd_byte) {
    gpio_set_level(PIN_CMD, 1);
    gpio_set_level(PIN_CS, 0);
    spi_transaction_t t = { .length = 8, .tx_buffer = &cmd_byte, .rx_buffer = NULL};
    esp_err_t ret = spi_device_polling_transmit(spi_handle, &t);
    gpio_set_level(PIN_CS, 1);
    gpio_set_level(PIN_CMD, 0);
    return ret;
}

esp_err_t max22200_change_channel_settings(uint8_t channel, uint8_t hit, uint8_t hit_time, uint8_t hold, uint8_t frequency, bool current_or_voltage, bool high_or_low_side) {
    uint32_t channel_value;
    esp_err_t ret = max22200_read_32bit(channel, &channel_value);
    if(ret != ESP_OK) {
        return ret;
    }

    channel_value |= (MAX22200_HIT_MASK & hit) << MAX22200_HIT_POS;
    channel_value |= (MAX22200_HIT_T_MASK & hit_time) << MAX22200_HIT_T_POS;
    channel_value |= (MAX22200_HOLD_MASK & hold) << MAX22200_HOLD_POS;
    channel_value |= (MAX22200_FREQ_CFG_MASK & frequency) << MAX22200_FREQ_CFG_POS;
    channel_value |= (MAX22200_VDRNCDR_MASK & current_or_voltage) << MAX22200_VDRNCDR_POS;
    channel_value |= (MAX22200_HSNLS_MASK & high_or_low_side) << MAX22200_HSNLS_POS;

    ret = max22200_write_32bit(channel, channel_value);
    if(ret != ESP_OK) {
        return ret;
    }

    return ret;
}

MAX22200_Status_flags_t max22200_status_flag_read(bool log_flag_states) {
    uint32_t status_val;
    max22200_read_32bit(MAX22200_ADDR_STATUS, &status_val);

    MAX22200_Status_flags_t flags = {
        .uvm = (status_val >> 1) & 0x01,
        .comer = (status_val >> 2) & 0x01,
        .dpm = (status_val >> 3) & 0x01,
        .hhf  = (status_val >> 4) & 0x01,
        .olf  = (status_val >> 5) & 0x01,
        .ocp  = (status_val >> 6) & 0x01,
        .ovt = (status_val >> 7) & 0x01
    };

    if (log_flag_states) {
        if(flags.uvm) {
            ESP_LOGW("WARNING", "UVM FLAG, VM UVLO event has been detected");
        } else {
            ESP_LOGI("OK", "UVM FLAG, Normal operation");
        }

        if(flags.comer) {
            ESP_LOGW("WARNING", "COMER FLAG, SPI Write comunication error detected");
        } else {
            ESP_LOGI("OK", "COMER FLAG, No SPI error detected");
        }
        
        if(flags.dpm) {
            ESP_LOGW("WARNING", "DPM FLAG, At least 1 channel has detected a Detection of Plunger Movement Fault");
        } else {
            ESP_LOGI("OK", "DPM FLAG, Normal operation");
        }
        
        if(flags.hhf) {
            ESP_LOGW("WARNING", "HHF FLAG, At least 1 channel has detected a HIT current fault");
        } else {
            ESP_LOGI("OK", "HHF FLAG, Normal operation");
        }
        
        if(flags.olf) {
            ESP_LOGW("WARNING", "OLF FLAG, At least 1 channel has detected an open load fault");
        } else {
            ESP_LOGI("OK", "OLF FLAG, Normal operation");
        }
        
        if(flags.ocp) {
            ESP_LOGW("WARNING", "OCP FLAG, At least 1 channel has detected an overcurrent event");
        } else {
            ESP_LOGI("OK", "OCP FLAG, Normal operation");
        }
        
        if(flags.ovt) {
            ESP_LOGW("WARNING", "OVT FLAG, Chip is in thermal protection");    
        } else {
            ESP_LOGI("OK", "OVT FLAG, Normal operation");
        }

    }

    return flags;
}

static MAX22200_Channel_flags_t get_channel_fault_flags(uint8_t channel, uint32_t fault_val) {
    if(channel == 0 || channel > 7) {
        ESP_LOGE("ERROR", "Wrong channel");
        return (MAX22200_Channel_flags_t) {
            .ocp = 0x01,
            .hhf = 0x01,  
            .olf = 0x01,
            .dpm = 0x01};
    }

    MAX22200_Channel_flags_t flags = {
        .ocp = (fault_val >> (channel + 24)) & 0x01,
        .hhf = (fault_val >> (channel + 16)) & 0x01,  
        .olf = (fault_val >> (channel + 8)) & 0x01,
        .dpm = (fault_val >> channel) & 0x01,
    };

    return flags;
}

MAX22200_Channel_flags_t max22200_diagnose_channel(uint8_t channel) {
    uint32_t fault_val;
    esp_err_t err = max22200_read_32bit(MAX22200_ADDR_FAULT, &fault_val);
    if(err != ESP_OK) {
        ESP_LOGE("ERROR", "Error while reading fault registry");
        return (MAX22200_Channel_flags_t) {
            .ocp = 0x01,
            .hhf = 0x01,  
            .olf = 0x01,
            .dpm = 0x01};
    }
    MAX22200_Channel_flags_t flags = get_channel_fault_flags(channel, fault_val);

    ESP_LOGE("FLAG CHECK", "Flags for channel %ld", channel - 1);

    if(flags.ocp) {
        ESP_LOGW("WARNING", "OCP FLAG, Overcurrent Protection");
    } else {
        ESP_LOGI("OK", "OCP FLAG, Normal operation");
    }    
    
    if(flags.hhf) {
        ESP_LOGW("WARNING", "HHF FLAG, HIT Current Not Reached");
    } else {
        ESP_LOGI("OK", "HHF FLAG, Normal operation");
    }
        
    if(flags.olf) {
        ESP_LOGW("WARNING", "OLF FLAG, Open-Load Detection");
    } else {
        ESP_LOGI("OK", "OLF FLAG, Normal operation");
    }
        
    if(flags.dpm) {
        ESP_LOGW("WARNING", "DPM FLAG, Detection of Plunger Movement");
    } else {
        ESP_LOGI("OK", "DPM FLAG, Normal operation");
    }

    return flags;
}

MAX22200_Fault_flags_t max22200_get_all_fault_flags() {
    uint32_t fault_val;
    MAX22200_Fault_flags_t flags;
    esp_err_t err = max22200_read_32bit(MAX22200_ADDR_FAULT, &fault_val);

    if(err != ESP_OK) {
        ESP_LOGE("ERROR", "Error reading fault registry");
        flags.channel_0 = (MAX22200_Channel_flags_t) {.ocp = 0x01,.hhf = 0x01,  .olf = 0x01,.dpm = 0x01};
        flags.channel_1 = (MAX22200_Channel_flags_t) {.ocp = 0x01,.hhf = 0x01,  .olf = 0x01,.dpm = 0x01};
        flags.channel_2 = (MAX22200_Channel_flags_t) {.ocp = 0x01,.hhf = 0x01,  .olf = 0x01,.dpm = 0x01};
        flags.channel_3 = (MAX22200_Channel_flags_t) {.ocp = 0x01,.hhf = 0x01,  .olf = 0x01,.dpm = 0x01};
        flags.channel_4 = (MAX22200_Channel_flags_t) {.ocp = 0x01,.hhf = 0x01,  .olf = 0x01,.dpm = 0x01};
        flags.channel_5 = (MAX22200_Channel_flags_t) {.ocp = 0x01,.hhf = 0x01,  .olf = 0x01,.dpm = 0x01};
        flags.channel_6 = (MAX22200_Channel_flags_t) {.ocp = 0x01,.hhf = 0x01,  .olf = 0x01,.dpm = 0x01};
        flags.channel_7 = (MAX22200_Channel_flags_t) {.ocp = 0x01,.hhf = 0x01,  .olf = 0x01,.dpm = 0x01};
        return flags;
    }

    flags.channel_0 = get_channel_fault_flags(MAX22200_ADDR_CH0, fault_val);
    flags.channel_1 = get_channel_fault_flags(MAX22200_ADDR_CH1, fault_val);
    flags.channel_2 = get_channel_fault_flags(MAX22200_ADDR_CH2, fault_val);
    flags.channel_3 = get_channel_fault_flags(MAX22200_ADDR_CH3, fault_val);
    flags.channel_4 = get_channel_fault_flags(MAX22200_ADDR_CH4, fault_val);
    flags.channel_5 = get_channel_fault_flags(MAX22200_ADDR_CH5, fault_val);
    flags.channel_6 = get_channel_fault_flags(MAX22200_ADDR_CH6, fault_val);
    flags.channel_7 = get_channel_fault_flags(MAX22200_ADDR_CH7, fault_val);

    return flags;
}