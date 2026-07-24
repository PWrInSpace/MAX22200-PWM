#include "max22200.h"
#include "max22200_regs.h"
#include "spi_init.h"
#include "esp_log.h"

void app_main(void) {
    //inicjalizacja
    max22200_init_hardware();
    vTaskDelay(pdMS_TO_TICKS(10));
    //test status
    uint32_t status_val = max22200_read(MAX22200_ADDR_STATUS);
    ESP_LOGE("ODCZYT","Odczytany STATUS: 0x%08lX\n", status_val);

    if (status_val == 0x00000000 || status_val == 0xFFFFFFFF) {
        ESP_LOGE("BLAD","STATUS niepoprawny. Sprawdź kable i zasilanie!\n");
        return;
    }
    ESP_LOGE("OK","Rejestr STATUS odpowiedzial.\n");
    //write->read-back test
    uint32_t test_pattern = 0x2A;
    ESP_LOGE("ZAPIS","Zapisuję do CFG_CH0: 0x%08lX\n", test_pattern);
    max22200_write(MAX22200_ADDR_CH0, test_pattern);

    uint32_t read_back = max22200_read(MAX22200_ADDR_CH0);
    ESP_LOGE("ODCZYT","Odczytano z CFG_CH0: 0x%08lX\n", read_back);

    if (read_back == test_pattern) {
        ESP_LOGE("OK","Fizyczna warstwa SPI + CMD + zasilanie DZIALAJĄ!\n");
    } else {
        ESP_LOGE("BLAD","Dane w petli zostały znieksztalcone!\n");
    }
}