/********************************************************************
 * Instituto Federal de Santa Catarina - Campus Florianópolis
 * Curso: Engenharia Eletrônica
 * Unidade Curricular: Projeto Integrador III
 * Alunos: André, Greicili e Leonardo
 * Data: 24/07/2025
 ********************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/dac.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_netif.h"

#include "esp_http_client.h"

#if CONFIG_IDF_TARGET_ESP32
#include "esp_adc_cal.h"
#endif

#define SPP_SERVER_NAME "SPP_SERVER"
#define DEVICE_NAME "POWER_BANK_SYSTEM"

// resistores de 1184 e 4.97k homs
#define DEFAULT_VREF 1100 // Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 64  // Multisampling

#define TRANS_CONDUT 99 // 99 mV/A, medido em bancada e feito a média -- visitar medição
#define AJUST_DIVISOR
#define OFF_SET_MEDIDOR_CORRENTE 604 // 604 mV lidos sem carga
#define OFF_SET_MEDIDOR_RAW 2173     // offset em relação a zero corrente
#define CONVERSAO_DIVISOR 42013

#define ADC2_CHANNEL ADC2_CHANNEL_7
#define TESAO_MEDIDA_ADC2 483 // 527 mV no divisor de tensao
#define TENSAO_LIDA_ESP 635
#define TENSAO_FONTE 24
#define OFF_SET_ADC2 10

#define OFF_SET_TENSAO 33
#define PERIODO_MS 1000

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_4; // GPIO32 ADC1
static const adc_atten_t atten = ADC_ATTEN_DB_0;    // Definição da atenuação de entrada do ADC
static const adc_unit_t unit = ADC_UNIT_1;

#if CONFIG_IDF_TARGET_ESP32
static void check_efuse(void)
{
    // Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
    {
        printf("eFuse Two Point: Supported\n");
    }
    else
    {
        printf("eFuse Two Point: NOT supported\n");
    }

    // Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
    {
        printf("eFuse Vref: Supported\n");
    }
    else
    {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        printf("Characterized using Two Point Value\n");
    }
    else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        printf("Characterized using eFuse Vref\n");
    }
    else
    {
        printf("Characterized using Default Vref\n");
    }
}
#endif

/**************** Definições para a API callmebot ****************/
extern const char letsencrypt_root_pem_start[] asm("_binary_letsencrypt_root_pem_start"); // Arquivo PEM para certificado TLS do callme bot
extern const char letsencrypt_root_pem_end[] asm("_binary_letsencrypt_root_pem_end");
bool callmebot_enable = false;
/**************** Definições para o WIFI ****************/
/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define CONFIG_ESP_MAXIMUM_RETRY 5
static esp_event_handler_instance_t ip_event_handler;
static esp_event_handler_instance_t wifi_event_handler;
bool wifi_iniciated = false; // Variável para indicar que está conectado no Wifi
static int s_retry_num = 0;  // Grava o número de tentativas de reconexão

/**************** Definições para o NVS ****************/
char id_credencial[] = "nvs_cred";     // Namespace das credenciais do Wifi no NVS
char id_callmebot[] = "nvs_callmebot"; // Namespace das credenciais do callmebot no NVS

typedef struct
{
    char key[16];
    char phone[16];
} Callmebot_t;
Callmebot_t callmebot = {{0}, {0}}; // Estrutura para armazenar o telefone e key para usar a API callmebot

typedef struct
{
    char ssid[16];
    char pass[16];
} Credencial_t; // Estrutura para armazenar o SSID e Password da rede Wifi
Credencial_t credencial = {{0}, {0}};

/**************** Definições para o Bluetooth ****************/
static uint32_t client_handle = 0;
// Máquina de estados para o controle das mensagens enviadas por Bluetooth
static enum {
    STATE_WAIT_KEY,
    STATE_WAIT_PHONE,
    STATE_WAIT_SSID,
    STATE_WAIT_PASS,
    STATE_WAIT_DELETE,
} input_state = STATE_WAIT_KEY;

/**************** Protótipos das funções ****************/
/* NVS */
bool load_credentials(char *id, void *param); // Obter as credenciais armazenadas no NVS
void save_credentials(char *id, void *param); // Salvar as credenciais no NVS
void reset_credentials(char *id);             // Apagar as credenciais no NVS

/* WIFI */
void prepare_wifi(void);                                                                               // Função para inicializar algumas funções que só podem ser chamadas uma vez
esp_err_t connect_to_wifi(Credencial_t *param);                                                        // Função para se conectar ao Wifi
void reset_wifi(void);                                                                                 // Função para reiniciar o wifi
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data); // Tratador de eventos do Wifi

/* Bluetooth */
void bluetooth_init(void);                                                   // Iniciar Bluetooth
void bluetooth_shutdown_task(void *param);                                   // Desligar Bluetooth
void spp_send_text(const char *text);                                        // Enviar mensagem para o cliente conectado
static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param); // Tratador de eventos do Bluetooth

/* Tasks */
void send_message_task(void *param); // Enviar mensagem pelo whatsapp

/**************** Implementação das funções para o NVS ****************/
bool load_credentials(char *id, void *param)
{
    char TAG[] = "LOAD NVS";
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(id, NVS_READONLY, &nvs_handle);
    if (err == ESP_OK)
    {
        esp_err_t err1 = ESP_FAIL;
        esp_err_t err2 = ESP_FAIL;

        if (!strcmp(id, "nvs_cred"))
        {
            Credencial_t *cred = (Credencial_t *)param;

            size_t len = sizeof(cred->ssid);
            err1 = nvs_get_str(nvs_handle, "SSID", cred->ssid, &len);

            len = sizeof(cred->pass);
            err2 = nvs_get_str(nvs_handle, "PASS", cred->pass, &len);
        }
        else if (!strcmp(id, "nvs_callmebot"))
        {
            Callmebot_t *cred = (Callmebot_t *)param;

            size_t len = sizeof(cred->key);
            err1 = nvs_get_str(nvs_handle, "KEY", cred->key, &len);

            len = sizeof(cred->phone);
            err2 = nvs_get_str(nvs_handle, "PHONE", cred->phone, &len);
        }
        nvs_close(nvs_handle);
        if (err1 == ESP_OK && err2 == ESP_OK)
        {
            ESP_LOGI(TAG, "Credenciais obtidas com sucesso na NVS!");
            return true;
        }
        else
        {
            ESP_LOGE(TAG, "Erro (%s) e (%s) em obter as credencias!", esp_err_to_name(err1), esp_err_to_name(err2));
            return false;
        }
    }

    ESP_LOGE(TAG, "Error (%s) em obter as credenciais!", esp_err_to_name(err));
    return false;
}

void save_credentials(char *id, void *param)
{
    char TAG[] = "SAVE NVS";

    nvs_handle_t nvs_handle;
    if (nvs_open(id, NVS_READWRITE, &nvs_handle) == ESP_OK)
    {
        if (!strcmp(id, "nvs_cred"))
        {
            Credencial_t *cred = (Credencial_t *)param;
            nvs_set_str(nvs_handle, "SSID", cred->ssid);
            nvs_set_str(nvs_handle, "PASS", cred->pass);
        }
        else if (!strcmp(id, "nvs_callmebot"))
        {
            Callmebot_t *cred = (Callmebot_t *)param;
            nvs_set_str(nvs_handle, "KEY", cred->key);
            nvs_set_str(nvs_handle, "PHONE", cred->phone);
        }

        nvs_commit(nvs_handle);
        nvs_close(nvs_handle);
        ESP_LOGI(TAG, "Credenciais salvas na NVS");
    }
    else
    {
        ESP_LOGE(TAG, "Erro ao abrir NVS para salvar");
    }
}

void reset_credentials(char *id)
{
    char TAG[] = "RESET NVS";
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(id, NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK)
    {
        if (!strcmp(id, "nvs_cred"))
        {
            nvs_erase_key(nvs_handle, "SSID");
            nvs_erase_key(nvs_handle, "PASS");
        }
        else if (!strcmp(id, "nvs_callmebot"))
        {
            nvs_erase_key(nvs_handle, "KEY");
            nvs_erase_key(nvs_handle, "PHONE");
        }
        nvs_commit(nvs_handle);
        nvs_close(nvs_handle);
        ESP_LOGE(TAG, "Credenciais apagadas!");
    }
    else
    {
        ESP_LOGE(TAG, "Erro (%s) ao apagar credenciais!", esp_err_to_name(err));
    }
}

/**************** Implementação das funções para o Wifi ****************/
void prepare_wifi(void)
{
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &wifi_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &ip_event_handler));
}

esp_err_t connect_to_wifi(Credencial_t *param)
{
    char TAG[] = "Wifi Conect";

    ESP_LOGI(TAG, "Conectando ao WiFi SSID:%s\nPASS:%s\n", param->ssid, param->pass);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    wifi_iniciated = true;

    wifi_config_t wifi_config = {
        .sta = {
            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
             * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
             */
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    strncpy((char *)wifi_config.sta.ssid, param->ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, param->pass, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "Connected to ap");
        return ESP_OK;
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "Failed to connect");
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
        return ESP_FAIL;
    }

    return ESP_FAIL;
}

void reset_wifi(void)
{
    // Reset Wi-Fi
    esp_wifi_disconnect();
    esp_wifi_stop();
    esp_wifi_deinit();
    vTaskDelay(pdMS_TO_TICKS(100)); // Pequeno delay para garantir desligamento
}

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    char TAG[] = "Evento Wifi";
    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            if (s_retry_num < CONFIG_ESP_MAXIMUM_RETRY)
            {
                esp_wifi_connect();
                s_retry_num++;
                ESP_LOGI(TAG, "retry to connect to the AP");
            }
            else
            {
                xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
                reset_wifi();
                reset_credentials(id_credencial);
                if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED)
                    bluetooth_init();
                input_state = STATE_WAIT_SSID;
                spp_send_text("Falha na autenticação!\nInforme novamente o SSID:\n");
            }
            break;
        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "Connected to the AP");
            save_credentials(id_credencial, &credencial);
            spp_send_text("Conectado com sucesso!\n");
            break;
        default:
            break;
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        wifi_iniciated = true;
        char *msg = "%2APOWER_BANK_SYSTEM%20informa%3A%2A%0AEquipamento%20configurado%20com%20sucesso%21"; // mensagem codificado para url (www.urlencoder.io)
        xTaskCreate(send_message_task, "send_msg_task", 4096, msg, 5, NULL);
    }
}

/**************** Implementação das funções para o Bluetooth ****************/
void bluetooth_init(void)
{
    char TAG[] = "Init Bluetooth";

    ESP_LOGI(TAG, "Ligando Bluetooth...");
    esp_err_t ret;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK)
    {
        ESP_LOGE(TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK)
    {
        ESP_LOGE(TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK)
    {
        ESP_LOGE(TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK)
    {
        ESP_LOGE(TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK)
    {
        ESP_LOGE(TAG, "%s spp register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    esp_spp_init(ESP_SPP_MODE_CB);
}

void bluetooth_shutdown_task(void *param)
{
    char TAG[] = "Shutdown Bluetooth";

    ESP_LOGI(TAG, "Desligando Bluetooth...");

    esp_err_t ret = esp_spp_deinit();
    ESP_LOGI(TAG, "esp_spp_deinit: %s", esp_err_to_name(ret));

    if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_ENABLED)
    {
        ret = esp_bluedroid_disable();
        ESP_LOGI(TAG, "esp_bluedroid_disable: %s", esp_err_to_name(ret));
    }

    if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_INITIALIZED)
    {
        ret = esp_bluedroid_deinit();
        ESP_LOGI(TAG, "esp_bluedroid_deinit: %s", esp_err_to_name(ret));
    }

    if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED)
    {
        ret = esp_bt_controller_disable();
        ESP_LOGI(TAG, "esp_bt_controller_disable: %s", esp_err_to_name(ret));
    }

    ret = esp_bt_controller_deinit();
    ESP_LOGI(TAG, "esp_bt_controller_deinit: %s", esp_err_to_name(ret));
    ESP_LOGI(TAG, "Bluetooth desligado!");
    vTaskDelete(NULL);
}

void spp_send_text(const char *text)
{
    if (client_handle != 0)
    {
        esp_spp_write(client_handle, strlen(text), (uint8_t *)text);
    }
}

static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    char TAG[] = "Evento Bluetooth";

    switch (event)
    {
    case ESP_SPP_INIT_EVT:
        if (param->init.status == ESP_SPP_SUCCESS)
        {
            ESP_LOGI(TAG, "ESP_SPP_INIT_EVT");
            esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, SPP_SERVER_NAME); // ESP_SPP_SEC_AUTHENTICATE
        }
        else
        {
            ESP_LOGE(TAG, "ESP_SPP_INIT_EVT status:%d", param->init.status);
        }
        break;
    case ESP_SPP_CLOSE_EVT:
        ESP_LOGI(TAG, "ESP_SPP_CLOSE_EVT status:%d handle:%" PRIu32 " close_by_remote:%d", param->close.status,
                 param->close.handle, param->close.async);
        break;
    case ESP_SPP_START_EVT:
        if (param->start.status == ESP_SPP_SUCCESS)
        {
            ESP_LOGI(TAG, "ESP_SPP_START_EVT handle:%" PRIu32 " sec_id:%d scn:%d", param->start.handle, param->start.sec_id,
                     param->start.scn);
            esp_bt_dev_set_device_name(DEVICE_NAME);
            esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
        }
        else
        {
            ESP_LOGE(TAG, "ESP_SPP_START_EVT status:%d", param->start.status);
        }
        break;
    case ESP_SPP_DATA_IND_EVT:
        char data[128] = {0};
        int len = param->data_ind.len > sizeof(data) - 1 ? sizeof(data) - 1 : param->data_ind.len;
        memcpy(data, param->data_ind.data, len);
        ESP_LOGI(TAG, "Recebido via BT: %s", data);

        switch (input_state)
        {
        case STATE_WAIT_KEY:
            strncpy(callmebot.key, data, sizeof(callmebot.key) - 1);
            callmebot.key[len - 2] = '\0';
            input_state = STATE_WAIT_PHONE;
            spp_send_text("KEY recebida.\nInforme seu telefone: 55DDXXXXXXXX\n");
            break;
        case STATE_WAIT_PHONE:
            strncpy(callmebot.phone, data, sizeof(callmebot.phone) - 1);
            callmebot.phone[len - 2] = '\0';
            spp_send_text("Telefone recebido.\n");
            save_credentials(id_callmebot, &callmebot);
            if (wifi_iniciated)
            {
                input_state = STATE_WAIT_DELETE;
                spp_send_text("Mensagem de teste enviada para o call me bot.\n");
                char *msg = "%2APOWER_BANK_SYSTEM%20informa%3A%2A%0AEquipamento%20configurado%20com%20sucesso%21"; // mensagem codificado para url (www.urlencoder.io)
                xTaskCreate(send_message_task, "send_msg_task", 4096, msg, 4, NULL);
            }
            else
            {
                input_state = STATE_WAIT_SSID;
                spp_send_text("Digite o SSID da rede Wi-Fi:\n");
            }
            break;
        case STATE_WAIT_SSID:
            strncpy(credencial.ssid, data, sizeof(credencial.ssid) - 1);
            credencial.ssid[len - 2] = '\0';
            input_state = STATE_WAIT_PASS;
            spp_send_text("SSID recebido.\nDigite a senha da rede Wi-Fi:\n");
            break;

        case STATE_WAIT_PASS:
            strncpy(credencial.pass, data, sizeof(credencial.pass) - 1);
            credencial.pass[len - 2] = '\0';
            spp_send_text("Senha recebida. Conectando ao Wi-Fi...\n");
            input_state = STATE_WAIT_DELETE;
            connect_to_wifi(&credencial);
            break;
        default:
            break;
        }
        break;

    case ESP_SPP_WRITE_EVT:
        ESP_LOGI(TAG, "ESP_SPP_WRITE_EVT");
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        ESP_LOGI(TAG, "Cliente conectado via Bluetooth");
        client_handle = param->srv_open.handle;
        input_state = STATE_WAIT_KEY;
        spp_send_text("Informe a KEY obtido do call me bot:\n");
        break;
    default:
        break;
    }
}

void send_message_task(void *param)
{
    if (load_credentials(id_callmebot, &callmebot))
    {
        char TAG[] = "Mensagem pelo Whatsapp";
        char url[512];
        snprintf(url, sizeof(url), "https://api.callmebot.com/whatsapp.php?phone=%s&text=%s&apikey=%s",
                 callmebot.phone, (const char *)param, callmebot.key);

        printf("URL = %s", url);

        esp_http_client_config_t config = {
            .url = url,
            .cert_pem = letsencrypt_root_pem_start,
            .method = HTTP_METHOD_POST,
        };

        esp_http_client_handle_t client = esp_http_client_init(&config);
        esp_err_t err = esp_http_client_perform(client);
        uint16_t cod_http = esp_http_client_get_status_code(client);

        if (err == ESP_OK && cod_http == 200)
        {
            ESP_LOGI(TAG, "Mensagem enviada com sucesso para o call me bot");
            callmebot_enable = true;
            xTaskCreate(bluetooth_shutdown_task, "bt_shutdown", 4096, NULL, 5, NULL);
        }
        else
        {
            ESP_LOGE(TAG, "Erro ao enviar mensagem: %s, código http: %d", esp_err_to_name(err), cod_http);
            callmebot_enable = false;
            reset_credentials(id_callmebot);
            if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED)
                bluetooth_init();
            input_state = STATE_WAIT_KEY;
            spp_send_text("Não foi possível se conectar ao call me bot.\nInforme novamente a KEY:\n");
        }

        esp_http_client_cleanup(client);
    }
    else
    {
        if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED)
            bluetooth_init();
        input_state = STATE_WAIT_KEY;
        spp_send_text("Não foi possível se conectar ao call me bot.\nInforme novamente a KEY:\n");
    }

    vTaskDelete(NULL);
}

void app_main(void)
{
    // Check if Two Point or Vref are burned into eFuse
    check_efuse();

    // Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channel, atten);

    // Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, 1100, adc_chars);
    print_char_val_type(val_type);

    int32_t valor_corrigido = 0;
    uint32_t voltage = 0;
    uint32_t adc_reading = 0;

    // ADC 2 init ------------------------------
    int read_raw = 0;
    uint32_t read_raw_2 = 0;
    esp_err_t r;
    uint32_t tensao_saida = 0;
    uint32_t conversao_tensao_tensao_mv = 28409; // nova (1000000*TENSAO_FONTE)/TENSAO_LIDA_ESP;
    uint32_t corrente_saida = 0;

    // potência instantânea:
    uint64_t potencia = 0;

    // energia:
    uint64_t energia = 0;

    gpio_num_t adc_gpio_num;

    r = adc2_pad_get_io_num(ADC2_CHANNEL, &adc_gpio_num);
    assert(r == ESP_OK);

    printf("ADC2 channel %d @ GPIO %d\n", ADC2_CHANNEL, adc_gpio_num);

    // be sure to do the init before using adc2.
    printf("adc2_init...\n");
    adc2_config_channel_atten(ADC2_CHANNEL, ADC_ATTEN_DB_0); // mudado

    vTaskDelay(2 * portTICK_PERIOD_MS);

    // Configurando Pino GPIO34 como entrada : Detectar Queda de tensão
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
    bool level = 0;                                                                                        // Variável para verificar estado do GPIO34
    bool enviado = 0;                                                                                      // Variável para verificar se a mensagem já foi enviada
    bool queda = 0;                                                                                        // Variável para verificar se já teve queda
    char *msg_retorno = "%2APOWER_BANK_SYSTEM%20informa%3A%2A%0AEnergia%20retornou%21";                    // mensagem codificado para url (www.urlencoder.io)
    char *msg_queda = "%2APOWER_BANK_SYSTEM%20informa%3A%2A%20%0AQueda%20de%20tens%C3%A3o%20detectada%21"; // mensagem codificado para url (www.urlencoder.io)
    bool enviado_pot = false;
    char *consumo = "%2APOWER_BANK_SYSTEM%20informa%3A%2A%0AProcesso%20finalizado."; 

    // Inicialização NVS (armazenar dados na memória não volátil)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    prepare_wifi();
    reset_credentials(id_callmebot);
    reset_credentials(id_credencial);

    if (load_credentials(id_credencial, &credencial))
    {
        ESP_LOGI("Load Credencial", "Credenciais encontradas. Conectando automaticamente...");
        if (connect_to_wifi(&credencial) == ESP_OK)
        {
            ESP_LOGI("MAIN", "Conectado no WIFI.\n");
        }
        else
        {
            ESP_LOGI("MAIN", "Não conectado. Iniciando modo de configuração via Bluetooth.");
        }
    }
    else
    {
        ESP_LOGI("MAIN", "Nenhuma credencial encontrada. Iniciando modo de configuração via Bluetooth.");
        bluetooth_init();
    }

    // Continuously samples
    while (1)
    {

        level = gpio_get_level(GPIO_NUM_34);
        if (level)
        {
            if (!enviado && callmebot_enable)
            {
                xTaskCreate(send_message_task, "send_msg_task", 4096, msg_queda, 4, NULL);
                enviado = true;
                queda = true;
            }
        }
        else
        {
            if (!enviado && queda && callmebot_enable)
            {
                xTaskCreate(send_message_task, "send_msg_task", 4096, msg_retorno, 4, NULL);
                enviado = true;
                queda = false;
            }
        }

        adc_reading = 0;
        // Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++)
        {
            adc_reading += adc1_get_raw((adc1_channel_t)channel);
        }
        adc_reading /= NO_OF_SAMPLES;
#if CONFIG_IDF_TARGET_ESP32
        // Convert adc_reading to voltage in mV
        voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
        valor_corrigido = (voltage - (uint32_t)OFF_SET_MEDIDOR_CORRENTE);
        if (valor_corrigido < 0)
        {
            valor_corrigido = 0;
        }
        corrente_saida = (uint32_t)CONVERSAO_DIVISOR * valor_corrigido;
        printf("Corrente: %lf A\n", (double)((double)corrente_saida / 1000000));
        printf("Corrente: %ld A\n", voltage);
#endif

        read_raw_2 = 0;
        for (int i = 0; i < NO_OF_SAMPLES; i++)
        {
            r = adc2_get_raw(ADC2_CHANNEL, ADC_WIDTH_BIT_12, &read_raw);
            read_raw_2 += (uint32_t)read_raw;
        }
        read_raw_2 /= NO_OF_SAMPLES;

        voltage = esp_adc_cal_raw_to_voltage(read_raw_2, adc_chars);
        // printf("Raw: %d\n", read_raw_2);
        if (r == ESP_OK)
        {
            // printf("Tensão ADC_2: %u mV\n", voltage-(uint32_t)OFF_SET_TENSAO );
            tensao_saida = (voltage - (uint32_t)OFF_SET_TENSAO) * conversao_tensao_tensao_mv;
            printf("tensao de entrada: %lf V\n", (double)tensao_saida / 1000000); //<-aqui
        }
        else
        {
            printf("%s\n", esp_err_to_name(r));
        }

        potencia = (uint64_t)tensao_saida * (uint64_t)corrente_saida;
        potencia = potencia / 1000000000;
        printf("Pontência instantânea: %lf W\n", (double)((double)potencia / (1000)));

        energia += (uint32_t)potencia * PERIODO_MS / 1000;
        printf("Energia: %lf Wh\n", (double)((double)energia / (3600000)));

        if ((double)((double)potencia / (1000)) < 5)
        {
            if (!enviado_pot)
            {
                xTaskCreate(send_message_task, "send_msg_task", 4096, consumo, 4, NULL);
                energia = 0;
                enviado_pot = true;
            }
        }
        else
        {
            enviado_pot = false;
        }

        printf("\n");

        vTaskDelay(pdMS_TO_TICKS(PERIODO_MS));
    }
}
