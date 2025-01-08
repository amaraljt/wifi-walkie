#include "station_wifi.h"
#include "iot_button.h"

#define LED    GPIO_NUM_2
#define BUTTON GPIO_NUM_15

static const char *TAG = "Wifi Station";
TaskHandle_t send_data_task_handle = NULL;

static void button_single_click_cb(void *arg,void *usr_data)
{
  ESP_LOGI(TAG, "BUTTON_SINGLE_CLICK");

  // Check if the task is already running
  if (send_data_task_handle == NULL) {
      // Create the task
      xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
  } else {
      ESP_LOGW(TAG, "Task already running, ignoring button press.");
  }
    
}

void app_main(void)
{
  // create gpio button
  button_config_t gpio_btn_cfg = {
      .type = BUTTON_TYPE_GPIO,
      .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
      .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
      .gpio_button_config = {
          .gpio_num = 15,
          .active_level = 0,
      },
  };
  button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
  if(NULL == gpio_btn) {
      ESP_LOGE(TAG, "Button create failed");
  }

  iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_single_click_cb,NULL);

  //Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
  wifi_init_sta();

  ESP_ERROR_CHECK(esp_netif_init());

  /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
   * Read "Establishing Wi-Fi or Ethernet Connection" section in
   * examples/protocols/README.md for more information about this function.
   */
  //ESP_ERROR_CHECK(example_connect());

  gpio_set_direction(LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

  while(1){
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

