#ifndef STATION_WIFI_HEADER_H_
#define STATION_WIFI_HEADER_H_

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#include "driver/gpio.h"

void udp_client_task(void *);
void event_handler(void*, esp_event_base_t, int32_t, void*);
void wifi_init_sta(void);

#endif // STATION_HEADER
