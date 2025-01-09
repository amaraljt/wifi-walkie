#include "i2c_lcd.h"
#include "unistd.h"

#define SLAVE_ADDRESS_LCD 0x27
#define I2C_NUM I2C_NUM_0

esp_err_t err;
i2c_master_dev_handle_t dev_handle = NULL;
static const char *TAG = "LCD";

/* i2c master configuration */
void i2c_master_init(void){

  i2c_master_bus_config_t bus_config = {
    .i2c_port = I2C_NUM_0,
    .sda_io_num = GPIO_NUM_21,
    .scl_io_num = GPIO_NUM_22,
    .flags.enable_internal_pullup = true,
    .clk_source = I2C_CLK_SRC_DEFAULT,
  };

  i2c_master_bus_handle_t bus_handle;
  ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

  i2c_device_config_t dev_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = 0x27,
    .scl_speed_hz = 100000,
  };

  ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
}


void lcd_send_cmd(char cmd){
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (cmd & 0xf0);
  data_l = ((cmd << 4) & 0xf0);
  data_t[0] = data_u | 0x0C; // en = 1, rs = 0
  data_t[1] = data_u | 0x08; // en = 0, rs = 0
  data_t[2] = data_l | 0x0C; // en = 1, rs = 0
  data_t[3] = data_l | 0x08; // en = 0, rs = 0
  
  err = i2c_master_transmit(dev_handle, data_t, 4, 1000);
  vTaskDelay(pdMS_TO_TICKS(20));
  if(err != 0) ESP_LOGI(TAG, "Error sending command");
}

void lcd_send_data(char data){
  char data_u, data_l;
  uint8_t data_t[4];

  data_u = (data & 0xf0);
  data_l = ((data << 4) & 0xf0);
  data_t[0] = data_u | 0x0D; // en = 1, rs = 0
  data_t[1] = data_u | 0x09; // en = 0, rs = 0
  data_t[2] = data_l | 0x0D; // en = 1, rs = 0
  data_t[3] = data_l | 0x09; // en = 0, rs = 0
  
  err = i2c_master_transmit(dev_handle, data_t, 4, 1000);
  vTaskDelay(pdMS_TO_TICKS(20));
  if(err != 0) ESP_LOGI(TAG, "Error in sending data");
}

void lcd_clear(void){
  lcd_send_cmd(0x01);  // Use the clear display command
  usleep(1000);  // Need to wait after clear
}

void lcd_put_cur(int row, int col){
  switch(row){
    case 0:
      col |= 0x80;
      break;
    case 1:
      col |= 0xC0;
      break;
  }
  lcd_send_cmd(col);
}

void lcd_init(void){
  // 4 bit initialization
  vTaskDelay(pdMS_TO_TICKS(50));

  // First send 0x30 three times
  lcd_send_cmd(0x30);
  vTaskDelay(pdMS_TO_TICKS(5));
  lcd_send_cmd(0x30);
  vTaskDelay(pdMS_TO_TICKS(5));
  lcd_send_cmd(0x30);
  vTaskDelay(pdMS_TO_TICKS(1));
  
  // Set to 4-bit mode
  lcd_send_cmd(0x20);  // 4-bit mode
  vTaskDelay(pdMS_TO_TICKS(1));
  
  // Function set: 4-bit mode, 2 lines, 5x8 font
  lcd_send_cmd(0x28);
  vTaskDelay(pdMS_TO_TICKS(1));
  
  // Display control: Display off
  lcd_send_cmd(0x08);
  vTaskDelay(pdMS_TO_TICKS(1));
  
  // Clear display
  lcd_send_cmd(0x01);
  vTaskDelay(pdMS_TO_TICKS(2));
  
  // Entry mode set
  lcd_send_cmd(0x06);
  vTaskDelay(pdMS_TO_TICKS(1));
  
  // Display ON with cursor
  lcd_send_cmd(0x0C);
  vTaskDelay(pdMS_TO_TICKS(2));

  ESP_LOGI(TAG, "Initialized.");
}

void lcd_send_string(char *str){
  while (*str) lcd_send_data(*str++);
}

