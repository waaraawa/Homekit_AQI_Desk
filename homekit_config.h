#ifndef HOMEKIT_CONFIG__H__99D20943_B47B_45CD_AF23_FF497B9150D8
#define HOMEKIT_CONFIG__H__99D20943_B47B_45CD_AF23_FF497B9150D8

#include <Arduino.h>
#include <homekit/types.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <stdio.h>
#include <port.h>

#define ACCESSORY_NAME  ("ESP8266_TEMPERATURE")
#define ACCESSORY_SN  ("SN_0123456")  //SERIAL_NUMBER
#define ACCESSORY_MANUFACTURER ("Arduino Homekit")
#define ACCESSORY_MODEL  ("ESP8266")

//==============================
// Homekit setup and loop
//==============================
extern homekit_server_config_t h_config;

extern homekit_characteristic_t h_name;

// Air Quality Service, HOMEKIT_SERVICE_AIR_QUALITY_SENSOR
extern homekit_characteristic_t h_aq_i;
extern homekit_characteristic_t h_aq_pm25;
extern homekit_characteristic_t h_aq_pm10;

// Temperature Sensor Service, HOMEKIT_SERVICE_TEMPERATURE_SENSOR
extern homekit_characteristic_t h_temp_s_c;

// Humidity Sensor Service, HOMEKIT_SERVICE_HUMIDITY_SENSOR
extern homekit_characteristic_t h_humi_s_c;

// Thermostat Service, HOMEKIT_SERVICE_THERMOSTAT
// TODO, change to HOMEKIT_SERVICE_HEATER_COOLER
extern homekit_characteristic_t h_thermo_c_st;
extern homekit_characteristic_t h_thermo_t_st;
extern homekit_characteristic_t h_thermo_c_temp;
extern homekit_characteristic_t h_thermo_t_temp;
extern homekit_characteristic_t h_thermo_temp_unit;
extern homekit_characteristic_t h_thermo_c_humi;


uint8_t current_heating_cooling_state;
uint8_t target_heating_cooling_state;
float target_temperature;
uint8_t temperature_display_unit;

#endif // HOMEKIT_CONFIG__H__99D20943_B47B_45CD_AF23_FF497B9150D8
