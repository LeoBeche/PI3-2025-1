/* ADC2 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "esp_system.h"
#if CONFIG_IDF_TARGET_ESP32
#include "esp_adc_cal.h"
#endif

// resistores de 1184 e 4.97k homs

#define DEFAULT_VREF                3300        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES               100         //Multisampling

#define TRANS_CONDUT                99          // 99 mV/A , medido em bancada e feito a média -- visitar medição 
#define AJUST_DIVISOR               
#define OFF_SET_MEDIDOR_CORRENTE    487         // 493 mV lidos sem carga 
#define OFF_SET_MEDIDOR_CORRENTE_V  1           // 1 milivolts entre o medido no multímetro e o medido pelo esp
#define OFF_SET_MEDIDOR_RAW         2173        // offset em relação a zero corrente   
  
#define ADC2_CHANNEL                ADC2_CHANNEL_7
#define TESAO_MEDIDA_ADC2           483         // 527 mV no divisor de tensao 
#define TENSAO_LIDA_ESP             635
#define TENSAO_FONTE                24
#define OFF_SET_ADC2                10

// possui um valor intrinsceco de offset por causa da precisão do canal de 0db (150 mV - 2.450 V) mas lê bem valores entre 22 e 27 volts

#define PERIODO_MS 1000

#if CONFIG_IDF_TARGET_ESP32
static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
#elif CONFIG_IDF_TARGET_ESP32S2BETA
static const adc_channel_t channel = ADC_CHANNEL_6;     // GPIO7 if ADC1, GPIO17 if ADC2
#endif 
static const adc_atten_t atten = ADC_ATTEN_0db;
static const adc_unit_t unit = ADC_UNIT_1;

#if CONFIG_IDF_TARGET_ESP32
static void check_efuse(void)
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}
#endif


void app_main(void)
{
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();


    //Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channel, atten);

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, 1100, adc_chars);
    print_char_val_type(val_type);

    int32_t valor_corrigido = 0;
    uint32_t voltage = 0;
    uint32_t adc_reading = 0;

    //ADC 2 init ------------------------------
    int             read_raw = 0;
    uint32_t        read_raw_2 = 0;
    esp_err_t r;
    uint32_t tensao_saida = 0;
    uint32_t   conversao_tensao_tensao_mv = (1000000*TENSAO_FONTE)/TENSAO_LIDA_ESP;
    uint32_t corrente_saida = 0;

    // potência instantânea:
    uint64_t    potencia = 0;

    // energia:
    uint32_t    energia = 0;
    uint32_t    ticks = 0;
    uint32_t    ticks_antes = 0;

    gpio_num_t adc_gpio_num;

    r = adc2_pad_get_io_num( ADC2_CHANNEL, &adc_gpio_num);
    assert( r == ESP_OK );

    printf("ADC2 channel %d @ GPIO %d\n", ADC2_CHANNEL, adc_gpio_num);

    //be sure to do the init before using adc2. 
    printf("adc2_init...\n");
    adc2_config_channel_atten( ADC2_CHANNEL, ADC_ATTEN_11db );

    vTaskDelay(2 * portTICK_PERIOD_MS);
    // -----------------------------
   
    //Continuously samples 
    while (1) {
        adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            adc_reading += adc1_get_raw((adc1_channel_t)channel);
        }
        adc_reading /= NO_OF_SAMPLES;
#if CONFIG_IDF_TARGET_ESP32
        //Convert adc_reading to voltage in mV
        voltage = esp_adc_cal_raw_to_voltage(adc_reading , adc_chars);
        //printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage - (uint32_t)OFF_SET_MEDIDOR_CORRENTE_V);
        //printf("Raw: %d\tVoltage: %dmV\n", adc_reading , voltage);
        valor_corrigido = ((voltage - (uint32_t)OFF_SET_MEDIDOR_CORRENTE_V) - (uint32_t)OFF_SET_MEDIDOR_CORRENTE);
        if (valor_corrigido < 0 )
        {
            valor_corrigido = 0;
        }
        corrente_saida = 51457 * valor_corrigido;
        printf("Corrente: %u uA\n", corrente_saida);
#endif

        read_raw_2 = 0;
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            r = adc2_get_raw( ADC2_CHANNEL, ADC_WIDTH_12Bit, &read_raw);
            read_raw_2 += (uint32_t)read_raw;
        }
        read_raw_2 /= NO_OF_SAMPLES;
        
        voltage = esp_adc_cal_raw_to_voltage(read_raw_2, adc_chars);
        //printf("Raw: %d\n", read_raw_2);
        if ( r == ESP_OK ) {
            //printf("Tensão ADC_2: %u mV\n", voltage );
            tensao_saida = (voltage)*conversao_tensao_tensao_mv;
            printf("tensao de entrada: %u uV\n", tensao_saida );
        } 
        else {
            printf("%s\n", esp_err_to_name(r));
        }
        
        potencia = (uint64_t)tensao_saida * (uint64_t)corrente_saida;
        potencia = potencia/1000000000;
        printf("Pontência instantânea: %d mW\n", (uint32_t) potencia );

        energia += (uint32_t)potencia * PERIODO_MS/1000;
        printf("Energia: %d J\n", energia);

        printf("\n");

        vTaskDelay(pdMS_TO_TICKS(PERIODO_MS));
    }
}


