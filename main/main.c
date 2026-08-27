#include "max22200.h"
#include "max22200_regs.h"
#include "spi_init.h"
#include "esp_log.h"

void app_main(void) {
    //inicjalizacja
    max22200_init_hardware();
    vTaskDelay(pdMS_TO_TICKS(10));
    
    max22200_test_channel(MAX22200_ADDR_CH0, 10);
}