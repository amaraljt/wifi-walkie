#ifndef ACCESS_POINT_SERVER_H_
#define ACCESS_POINT_SERVER_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "lwip/sockets.h"
#include <lwip/netdb.h>

#define CONFIG_EXAMPLE_IPV4 192.168.4.1
#define CONFIG_EXAMPLE_IPV6 /* insert IPv6 */

void udp_server_task(void *);
void wifi_event_handler(void *, esp_event_base_t, int32_t, void *);
void wifi_init_softap(void);

#endif // ACCESS_POINT_SERVER_H_
