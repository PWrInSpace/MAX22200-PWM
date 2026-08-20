#include "max22200.h"
#include "max22200_regs.h"
#include "spi_init.h"
#include "esp_log.h"

void app_main(void) {
    //inicjalizacja
    max22200_init_hardware();
    vTaskDelay(pdMS_TO_TICKS(10));
    
    //write->read-back test
    //uint32_t test_pattern = 0x2A;
    //ESP_LOGE("ZAPIS","Zapisuję do CFG_CH0: 0x%08lX\n", test_pattern);
    //max22200_write_32bit(MAX22200_ADDR_CH0, test_pattern);

    /*uint32_t read_back = max22200_read_32bit(MAX22200_ADDR_CH0);
    ESP_LOGE("ODCZYT","Odczytano z CFG_CH0: 0x%08lX\n", read_back);

    if (read_back == test_pattern) {
        ESP_LOGE("OK","Fizyczna warstwa SPI + CMD + zasilanie DZIALAJĄ!\n");
    } else {
        ESP_LOGE("BLAD","Dane w petli zostały znieksztalcone!\n");
    }*/
}