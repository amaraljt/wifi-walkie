#include "station_wifi.h"

#define LED    GPIO_NUM_2
#define BUTTON GPIO_NUM_15

static const char *TAG = "Wifi Station";

void app_main(void)
{
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

  bool task_running = false;

  while(1){
    if(gpio_get_level(BUTTON) == 1){
      gpio_set_level(LED, 1);
      ESP_LOGI(TAG, "Button Pressed");
      
      if(!task_running){
        task_running = true;
        xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
      }
    }else{
      gpio_set_level(LED, 0);
      task_running = false;
    }
    vTaskDelay(1);
  }
}

