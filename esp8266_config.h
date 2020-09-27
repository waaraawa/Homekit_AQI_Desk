#ifndef ESP8266_CONFIG__H__6114ffce_fceb_429a_9880_1c0b0e19e12b
#define ESP8266_CONFIG__H__6114ffce_fceb_429a_9880_1c0b0e19e12b

// PIN definitions
#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

//#define PIN_LED 13
#define PIN_IR_LED D6

#define OTA_NAME "homekit_test"

#define OTA_INTERVAL        100L // 100 ms
#define HOMEKIT_INTERVAL    500L // 500 ms

#define MEASURE_INTERVAL_PMS    10 * 1000L // 10 sec
#define MEASURE_INTERVAL_BME280 10 * 1000L // 10 sec

#define BME280_I2C_ADDRESS  0x76

void setup_wifi();
void setup_homekit();

// Callback methods prototypes
void ota_handle();
void homekit_handle();

void bme280_read_asap();
void bme280_init_handle();
void bme280_wakeup_handle();
void bme280_sleep_handle();
void bme280_read_handle();

void pms_init_handle();
void pms_sleep_handle();
void pms_read_handle();

#endif // ESP8266_CONFIG__H__6114ffce_fceb_429a_9880_1c0b0e19e12b
