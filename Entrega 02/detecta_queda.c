#include <stdio.h>
#include "esp_system.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define INPUT_0 34

void app_main(void)
{
    //Configurando pino
    gpio_config_t config_int = {};
    config_int.mode = GPIO_MODE_INPUT;               //Como entrada
    config_int.pin_bit_mask = ((1ULL << INPUT_0));   //GPIO34
    config_int.pull_down_en = 0;                    //Pull down desabilitado
    config_int.pull_up_en = 0;                      //Pull up desabilitado
    config_int.intr_type = GPIO_INTR_DISABLE;      //Interrupção desabilitada

    if (gpio_config(&config_int) == ESP_OK)
    {
        printf("Configurado com sucesso.\n");
    }
    else
    {
        printf("ERRO GPIO config.");
        exit(EXIT_FAILURE);
    }

    char level = 0;

    while (1)
    {
        level = gpio_get_level(GPIO_NUM_34);        // Lê pino
        
        if (level)
        {
            printf("Queda de tensão detectada!\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
