#include "homekit_config.h"

homekit_value_t get_current_heating_cooling_state()
{
    printf("%s++\n", __FUNCTION__);
    return HOMEKIT_UINT8(current_heating_cooling_state);
}

homekit_value_t get_target_heating_cooling_state()
{
    printf("%s++\n", __FUNCTION__);
    return HOMEKIT_UINT8(target_heating_cooling_state);
}

void set_target_heating_cooling_state(homekit_value_t value)
{
    printf("%s++\n", __FUNCTION__);
    if (homekit_format_uint8 != value.format)
    {
        printf("Invalid on-value format: %d\n", value.format);
        return;
    }

    target_heating_cooling_state = value.uint8_value;
    // TODO, send cmd
}

homekit_value_t get_target_temperature()
{
    printf("%s++\n", __FUNCTION__);
    return HOMEKIT_FLOAT(target_temperature);
}

void set_target_temperature(homekit_value_t value)
{
    printf("%s++\n", __FUNCTION__);
    if (homekit_format_float != value.format)
    {
        printf("Invalid on-value format: %d\n", value.format);
        return;
    }

    target_temperature = value.float_value;
    // TODO, send cmd
}

homekit_value_t get_temperature_display_unit()
{
    printf("%s++\n", __FUNCTION__);
    return HOMEKIT_UINT8(temperature_display_unit);
}

void set_temperature_display_unit(homekit_value_t value)
{
    printf("%s++\n", __FUNCTION__);
    if (homekit_format_float != value.format)
    {
        printf("Invalid on-value format: %d\n", value.format);
        return;
    }

    temperature_display_unit = value.uint8_value;
    // TODO, send cmd
}

//
homekit_characteristic_t h_name = HOMEKIT_CHARACTERISTIC_(NAME, ACCESSORY_NAME);

// Air Quality Service, HOMEKIT_SERVICE_AIR_QUALITY_SENSOR
homekit_characteristic_t h_aq_i = HOMEKIT_CHARACTERISTIC_(AIR_QUALITY, 0);
homekit_characteristic_t h_aq_pm25 = HOMEKIT_CHARACTERISTIC_(PM25_DENSITY, 0);
homekit_characteristic_t h_aq_pm10 = HOMEKIT_CHARACTERISTIC_(PM10_DENSITY, 0);

// Temperature Sensor Service, HOMEKIT_SERVICE_TEMPERATURE_SENSOR
homekit_characteristic_t h_temp_s_c = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);

// Humidity Sensor Service, HOMEKIT_SERVICE_HUMIDITY_SENSOR
homekit_characteristic_t h_humi_s_c = HOMEKIT_CHARACTERISTIC_(CURRENT_RELATIVE_HUMIDITY, 0);

// Thermostat Service, HOMEKIT_SERVICE_THERMOSTAT
homekit_characteristic_t h_thermo_c_st = HOMEKIT_CHARACTERISTIC_(CURRENT_HEATING_COOLING_STATE, 0);
homekit_characteristic_t h_thermo_t_st = HOMEKIT_CHARACTERISTIC_(TARGET_HEATING_COOLING_STATE, 0, .getter = get_target_heating_cooling_state, .setter = set_target_heating_cooling_state);
homekit_characteristic_t h_thermo_c_temp = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);
homekit_characteristic_t h_thermo_t_temp = HOMEKIT_CHARACTERISTIC_(TARGET_TEMPERATURE, 0, .getter = get_target_temperature, .setter = set_target_temperature);
homekit_characteristic_t h_thermo_temp_unit = HOMEKIT_CHARACTERISTIC_(TEMPERATURE_DISPLAY_UNITS, 0, .getter = get_temperature_display_unit, .setter = set_temperature_display_unit);
homekit_characteristic_t h_thermo_c_humi = HOMEKIT_CHARACTERISTIC_(CURRENT_RELATIVE_HUMIDITY, 0);

void accessory_identify(homekit_value_t _value)
{
    printf("%s++\n", __FUNCTION__);
}

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(
        .id = 1,
        .category = homekit_accessory_category_air_conditioner,
        .services = (homekit_service_t*[]) {
            HOMEKIT_SERVICE(
                ACCESSORY_INFORMATION,
                .characteristics = (homekit_characteristic_t*[]) {
                    HOMEKIT_CHARACTERISTIC(NAME, "Sensor Hub"),
                    HOMEKIT_CHARACTERISTIC(MANUFACTURER, ACCESSORY_MANUFACTURER),
                    HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, ACCESSORY_SN),
                    HOMEKIT_CHARACTERISTIC(MODEL, ACCESSORY_MODEL),
                    HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.0.1"),
                    HOMEKIT_CHARACTERISTIC(IDENTIFY, accessory_identify),
                    NULL
                }
            ),
            HOMEKIT_SERVICE(
                AIR_QUALITY_SENSOR,
                .characteristics = (homekit_characteristic_t*[]) {
                    HOMEKIT_CHARACTERISTIC(NAME, "Air Quality Sensor"),
                    &h_aq_i,
                    &h_aq_pm25,
                    &h_aq_pm10,
                    NULL
                }
            ),
            HOMEKIT_SERVICE(
                TEMPERATURE_SENSOR,
                .characteristics = (homekit_characteristic_t*[]) {
                    HOMEKIT_CHARACTERISTIC(NAME, "Temperature Sensor"),
                    &h_temp_s_c,
                    NULL
                }
            ),
            HOMEKIT_SERVICE(
                HUMIDITY_SENSOR,
                .characteristics = (homekit_characteristic_t*[]) {
                    HOMEKIT_CHARACTERISTIC(NAME, "Humidity Sensor"),
                    &h_humi_s_c,
                    NULL
                }
            ),
            HOMEKIT_SERVICE(
                THERMOSTAT,
                .primary = true,
                .characteristics = (homekit_characteristic_t*[]) {
                    HOMEKIT_CHARACTERISTIC(NAME, "Air Conditioner"),
                    &h_thermo_c_st,
                    &h_thermo_t_st,
                    &h_thermo_c_temp,
                    &h_thermo_t_temp,
                    &h_thermo_temp_unit,
                    &h_thermo_c_humi,
                    NULL
                }
            ),
            NULL
        }
    ),
    NULL
};

homekit_server_config_t h_config = {
    .accessories = accessories,
    .password = "111-11-111",
    //.on_event = on_homekit_event,
    //.setupId = "ABCD"
};
