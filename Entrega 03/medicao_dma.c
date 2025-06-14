/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_adc/adc_continuous.h"
#include "driver/gpio.h"

#define FRAME_CONV_SIZE 20
#define ADC_GET_CHANNEL(p_data) ((p_data)->type1.channel)
#define ADC_GET_DATA(p_data) ((p_data)->type1.data)
int ADC_DATA[2];
int SUM_DATA[2];
int voltage;
int current;
int finished = 0;

static adc_channel_t channel[2] = {ADC_CHANNEL_3, ADC_CHANNEL_7}; //CH3: Leitura Tensão; CH7: Leitura Corrente

adc_continuous_handle_t adc_handle = NULL;
TaskHandle_t cb_task;

static bool IRAM_ATTR callback(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    vTaskNotifyGiveFromISR(cb_task, &mustYield);
    return (mustYield == pdTRUE);
}

static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num)
{
    adc_continuous_handle_cfg_t handle_config = {
        .max_store_buf_size = 40,           // ADC faça a amostragem de 20 conversões (40/SOC_ADC_DIGI_RESULT_BYTES)
        .conv_frame_size = FRAME_CONV_SIZE, // Pode armazenar 10 conversões (20/SOC_ADC_DIGI_RESULT_BYTES)
    };

    ESP_ERROR_CHECK(adc_continuous_new_handle(&handle_config, &adc_handle));

    adc_continuous_config_t adc_config = {
        .sample_freq_hz = 20 * 1000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
        .pattern_num = channel_num};

    adc_digi_pattern_config_t adc_channel_config[SOC_ADC_PATT_LEN_MAX] = {0};
    for (int i = 0; i < channel_num; i++)
    {
        adc_channel_config[i].atten = ADC_ATTEN_DB_11;
        adc_channel_config[i].channel = channel[i];
        adc_channel_config[i].unit = ADC_UNIT_1;
        adc_channel_config[i].bit_width = ADC_BITWIDTH_12;

        ESP_LOGI("ADC INIT:", "adc_pattern[%d].atten is :%" PRIx8, i, adc_channel_config[i].atten);
        ESP_LOGI("ADC INIT:", "adc_pattern[%d].channel is :%" PRIx8, i, adc_channel_config[i].channel);
        ESP_LOGI("ADC INIT:", "adc_pattern[%d].unit is :%" PRIx8, i, adc_channel_config[i].unit);
    }

    adc_config.adc_pattern = adc_channel_config;
    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &adc_config));

    adc_continuous_evt_cbs_t cb_config = {
        .on_conv_done = callback,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(adc_handle, &cb_config, NULL));
}

//Task para a leitura do ADC
void cbTask(void *parameters)
{
    uint8_t buf[30];
    uint32_t rxLen = 0;
    esp_err_t ret;
    int count = 0;

    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        ret = adc_continuous_read(adc_handle, buf, FRAME_CONV_SIZE, &rxLen, 0);
        if (ret == ESP_OK)
        {
            SUM_DATA[0] = 0;
            SUM_DATA[1] = 0;
            count = 0;

            for (int i = 0; i < rxLen; i += SOC_ADC_DIGI_RESULT_BYTES)
            {
                adc_digi_output_data_t *p = (adc_digi_output_data_t *)&buf[i];
                uint16_t channel = ADC_GET_CHANNEL(p);
                uint16_t data = ADC_GET_DATA(p);
                if (channel == ADC_CHANNEL_3)
                {
                    SUM_DATA[0] += data;
                    count += 1;
                }
                if (channel == ADC_CHANNEL_7)
                    SUM_DATA[1] += data;
            }
            finished = 1;
            ADC_DATA[0] = SUM_DATA[0] / count;
            ADC_DATA[1] = SUM_DATA[1] / count;
            finished = 0;
        }
    }
}

void app_main(void)
{
    //Componentes do divisor de tensão para medição de tensão
    int R1 = 1000000;
    int R2 =  100000;
    float R1R2 = (R1+R2)/(R2*1.0);

    // Configurando Pino GPIO34 como entrada
    gpio_config_t config_int = {};
    config_int.mode = GPIO_MODE_INPUT;
    config_int.pin_bit_mask = ((1ULL << 34));
    config_int.pull_down_en = 0;
    config_int.pull_up_en = 0;
    config_int.intr_type = GPIO_INTR_DISABLE;

    if (gpio_config(&config_int) == ESP_OK)
    {
        printf("Configurado com sucesso.\n");
    }
    else
    {
        printf("ERRO GPIO config.");
        exit(EXIT_FAILURE);
    }
    
    char level = 0; // Variável para verificar estado do GPIO34

    xTaskCreate(cbTask, "Callback Task", 4096, NULL, 0, &cb_task);
    continuous_adc_init(channel, 2);
    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));

    while (1)
    {
        level = gpio_get_level(GPIO_NUM_34);
        if (level)
        {
            printf("Queda de tensão detectada!\n");
        }

        if (finished)
        {
            voltage = ADC_DATA[0] * 3300 / 4095;
            //current = ADC_DATA[1] * 3300 / 4095;
            printf("CH3:%f V\n", R1R2 * (voltage + 188)/1000.0);
            //printf("CH7:%f A\n", );
            vTaskDelay(10/portTICK_PERIOD_MS);
        }
    }

    ESP_ERROR_CHECK(adc_continuous_stop(adc_handle));
    ESP_ERROR_CHECK(adc_continuous_deinit(adc_handle));
}
