/* 
   Esse código é baseado nos códigos ADC1 e ADC2 da biblioteca de exemplos do ESP32-IDF.
   
   Nos testes foi usado o ADC1 para ler a corrente que passa através do sensor de corrente por efeito hall.
   O ADC2 foi usado para a mediçao de tensão pois ele será usado menos para essa função, já que, a tensão não mudará muito
   enquanto o ADC pode ser usado para outros objetivos como a comunicação via WI-FI.
  
   Utilizaram-se para as medições: Um divisor de tensão para medir a tensão de entrada, e no sensor de efeito hall,
   também foi usado um divisor para que a saída dele sem corrente (2.48V) não sature o ADC.
   
   Dificuldades:
   A maior dificuldade de fazer medições através dos ADCs é utilizar a atenuação e relação de tensão mais adequada
   para garantir a melhor resolução possível para a medida. Tendo em vista a tabela localizado no repositório da espressif
   em: esp-idf/components/driver/deprecated/driver/adc.h, leitura de tensão tem faixas que começam em 100 a 150 mV então
   essa faixa inicial foi evitada.   
   
   A configuração do ADC1 foi pensada para ler a menor variação de tensão possível de corrente, os testes com o sensor de efeito Hall
   mostraram que o aumento de 1 A gera um aumento de 99 mV e a tensão do sensor sem corrente é de 2.48 V. Foi ignorado o valor máximo
   de tensão de saída no sensor pois a corrente não passará de 15 Ampères. Com isso é possível correlacionar os valores de atenuação que 
   contenha a maior quantidade de valores por milivolt de incremento de tensão.

   Para calcular esse parâmetro foi feito as seguintes correlações para os ADC1 usando 12 bits:

   V_offset = 2.48   

   1000*V_max_adc/(2^12-1) = V_passo (miliVolts/sample)
   
   resolucao = 0 A - 15 A 

   escursao = 0.099 * 15 (V/A * A) -> 1.485 V 

   tensao_total_maxima = V_offset  + escursao  -> 3,965 V

   Para 0 db -> 100 mV - 950 mv
	
	Ou seja 950 mV -> 3965 mV

	relação de 0.2396
	
	relação_tensão_corrente = com essa relação, cada ampère de corente se traduz em: 23.72 mV/A (99 mV/A * relação)

	degrau: V_passo/relação_tensão_corrente -> 0.01132461 A/sample -> 11.32461 mA/sample 


   Para 2.5 db ->
	
	Ou seja 1250 mV -> 3965 mV
	
	relação de 0.61791
	
	relação_tensão_corrente = com essa relação, cada ampère de corente se traduz em: 61.173 mV/A (99 mV/A * relação)
	
	degrau: V_passo/relação_tensão_corrente -> 0.0131735 A/sample -> 13.1735 mA/sample 

   
   Para 11 db ->
	
	Ou seja 1750 mV -> 3965 mV
	
	relação de 0.61791
	
	relação_tensão_corrente = com essa relação, cada ampère de corente se traduz em: 61.173 mV/A (99 mV/A * relação)
	
	degrau: V_passo/relação_tensão_corrente -> 0.0131735 A/sample -> 13.1735 mA/sample 


   Para 11 db ->
	
	Ou seja 2450 mV -> 3965 mV
	
	relação de 0.61791
	
	relação_tensão_corrente = com essa relação, cada ampère de corente se traduz em: 61.173 mV/A (99 mV/A * relação)
	
	degrau: V_passo/relação_tensão_corrente -> 0.0131735 A/sample -> 13.1735 mA/sample 
          
	
   Com esses valores usamos a atenução de 0db e relação de 0.2396
   

   A configuração do ADC2 tem em vista o divisor de tensão, como medição mínima de (100 - 150 mV), então a minima tensão
   a ser medida será proporcional a tensão máxima medida e a atenuação escolhida. Ter um offset não é um problema já que 
   o intuito é ler valores de 15 a 27 volts.  

   1000*V_max_adc/(2^12-1) = V_passo (miliVolts/sample)
   
   resolucao = 15 V - 27 V 

   escursao = 12 V 

   tensao_total_maxima = 27 V

   Para 0 db -> 100 mV - 950 mv
	
	Ou seja 950 mV -> 27000 mV

	relação de 0.03518 V/V ou 35.18 mV/V
	
	degrau: V_passo/relação -> 7.6345 mV/sample

        com offset de leitura de 2.84 Volts (100 mV)


   Para 2.5 db ->
	
	Ou seja 1250 mV -> 27000 mV
	
	relação de 0.046296
	
	degrau: V_passo/relação -> 7.912 mV/sample
	
	com offset de leitura de 2.16 V (100 mV)  

   
   Para 11 db ->
	
	Ou seja 1750 mV -> 27000 mV
	
	relação de 0.064815
		
	degrau: V_passo/relação -> 8.29 mV/sample 

	com offset de leitura de 2.31 V (150 mV)  	


   Para 11 db ->
	
	Ou seja 2450 mV -> 27000 mV
	
	relação de 0.090741
	
	degrau: V_passo/relação_tensão_corrente -> 8.88 mV/sample 
         
   	com offset de leitura de 1.65 V (150 mV) 

   Com esses valores usamos a atenução de 0db e relação de 0.03518
   
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
#include "esp_int_wdt.h"


#endif

// resistores de 1184 e 4.97k homs

#define DEFAULT_VREF                1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES               16          //Multisampling

#define TRANS_CONDUT                99          // 99 mV/A , medido em bancada e feito a média -- visitar medição 
#define AJUST_DIVISOR               
#define OFF_SET_MEDIDOR_CORRENTE    604         // 604 mV lidos sem carga 
#define OFF_SET_MEDIDOR_RAW         2173        // offset em relação a zero corrente   
#define CONVERSAO_DIVISOR           42013  

#define ADC2_CHANNEL                ADC2_CHANNEL_7
#define TESAO_MEDIDA_ADC2           483         // 527 mV no divisor de tensao 
#define TENSAO_LIDA_ESP             635
#define TENSAO_FONTE                24
#define OFF_SET_ADC2                10

//novo
#define OFF_SET_TENSAO 33
// possui um valor intrinsceco de offset por causa da precisão do canal de 0db (150 mV - 2.450 V) mas lê bem valores entre 22 e 27 volts

#define PERIODO_MS 1000
#define PERIODO_MS_READ_ADC 1

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

uint32_t tensao_saida = 0;
uint32_t corrente_saida = 0;
// potência instantânea:
uint64_t    potencia = 0;
// energia:
uint64_t    energia = 0;

    int32_t valor_corrigido = 0;
    uint32_t voltage = 0;
    uint32_t adc_reading = 0;

    //ADC 2 init ------------------------------
    int             read_raw = 0;
    uint32_t        read_raw_2 = 0;
    esp_err_t r;
    
    
    uint32_t   conversao_tensao_tensao_mv = 28409;// nova (1000000*TENSAO_FONTE)/TENSAO_LIDA_ESP;

    gpio_num_t adc_gpio_num;
    uint32_t start;
    uint32_t stop;

void task_adc_read(void *parametros)
{
    
    while (1) {
        start = xTaskGetTickCount();  
        adc_reading = 0;
        
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            adc_reading += adc1_get_raw((adc1_channel_t)channel);
            vTaskDelay(pdMS_TO_TICKS(PERIODO_MS_READ_ADC));
        }
        adc_reading /= NO_OF_SAMPLES;
#if CONFIG_IDF_TARGET_ESP32
        
        //Convert adc_reading to voltage in mV
        voltage = esp_adc_cal_raw_to_voltage(adc_reading , adc_chars);
        valor_corrigido = (voltage  - (uint32_t)OFF_SET_MEDIDOR_CORRENTE);
        if (valor_corrigido < 0 )
        {
            valor_corrigido = 0;
        }
        corrente_saida = (uint32_t)CONVERSAO_DIVISOR * valor_corrigido;
#endif

        read_raw_2 = 0;
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            r = adc2_get_raw( ADC2_CHANNEL, ADC_WIDTH_12Bit, &read_raw);
            read_raw_2 += (uint32_t)read_raw;
            vTaskDelay(pdMS_TO_TICKS(PERIODO_MS_READ_ADC));
        }
        read_raw_2 /= NO_OF_SAMPLES;
        
        voltage = esp_adc_cal_raw_to_voltage(read_raw_2, adc_chars);
        
        if ( r == ESP_OK ) {
           
            tensao_saida = (voltage-(uint32_t)OFF_SET_TENSAO)*conversao_tensao_tensao_mv;
           
        } 
        else {
            printf("%s\n", esp_err_to_name(r));
        }
        
        potencia = (uint64_t)tensao_saida * (uint64_t)corrente_saida;
        potencia = potencia/1000000000;

        energia += (uint32_t)potencia * PERIODO_MS/1000;
                
        stop = xTaskGetTickCount() - start;
    }
}

void task_print_medida(void *parametros)
{
    while(1)
    {
    printf("Corrente: %lf A\n", (double)((double)corrente_saida/1000000));
    printf("Tensao de entrada: %lf V\n", (double)tensao_saida/1000000 ); //<-aqui
    printf("Pontência instantânea: %lf W\n", (double)((double)potencia/(1000)) );
    printf("Energia: %lf Wh\n", (double)((double)energia/(3600000)));
    //printf("Tempo de task adc: %u \n",stop);
    printf("\n");
    vTaskDelay(pdMS_TO_TICKS(PERIODO_MS));
    }  
}


void app_main(void)
{
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();
    TaskHandle_t pxCreatedTask;
    esp_err_t esp_task_wdt_deinit();
    //Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channel, atten);

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, 1100, adc_chars);
    print_char_val_type(val_type);

    r = adc2_pad_get_io_num( ADC2_CHANNEL, &adc_gpio_num);
    assert( r == ESP_OK );

    printf("ADC2 channel %d @ GPIO %d\n", ADC2_CHANNEL, adc_gpio_num);

    //be sure to do the init before using adc2. 
    printf("adc2_init...\n");
    adc2_config_channel_atten( ADC2_CHANNEL, ADC_ATTEN_0db ); //mudado

    vTaskDelay(2 * portTICK_PERIOD_MS);
    // -----------------------------

    xTaskCreate(&task_adc_read, "Leitura ADCs", 2048, NULL, 1, &pxCreatedTask);
    xTaskCreate(&task_print_medida, "Print ", 2048, NULL, 1, NULL);
    //esp_task_wdt_delete(pxCreatedTask);
    vTaskSuspend(NULL);

    while(1)
    {
    
    };
}


