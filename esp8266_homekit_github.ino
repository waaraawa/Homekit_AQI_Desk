#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <arduino_homekit_server.h>
#include <ArduinoOTA.h>
#include <TaskScheduler.h>
#include "Adafruit_BME280.h"
#include "PMS.h"
#include <IRsend.h>
#include "esp8266_config.h"
#include "homekit_config.h"
#include "wifi_config.h"

Scheduler ts;
PMS pms(Serial);
PMS::DATA pms_data;
Adafruit_BME280 bme;
IRsend irsend(PIN_IR_LED);

Task tOta(OTA_INTERVAL, TASK_FOREVER, &ota_handle, &ts, true);
Task tHomekit(HOMEKIT_INTERVAL, TASK_FOREVER, &homekit_handle, &ts, true);

Task tPMS_init(1000L, TASK_ONCE, &pms_init_handle, &ts, false);
Task tPMS_read(MEASURE_INTERVAL_PMS, TASK_ONCE, &pms_read_handle, &ts, false);

Task tBME280_init(1L, TASK_ONCE, &bme280_init_handle, &ts, false);
Task tBME280_wakeup(MEASURE_INTERVAL_BME280, TASK_ONCE, &bme280_wakeup_handle, &ts, false);
Task tBME280_sleep(1L, TASK_ONCE, &bme280_sleep_handle, &ts, false);
Task tBME280_read(10L, TASK_ONCE, &bme280_read_handle, &ts, false);

void ota_handle()
{
//    Serial.println("ota_handle++");
    ArduinoOTA.handle();
}

void homekit_handle()
{
    arduino_homekit_loop();
//	INFO("heap: %d, sockets: %d", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
}

void pms_read_asap()
{
    Serial.print(__FUNCTION__);
    Serial.print(", ");
    Serial.print(millis());
    Serial.println("++");

    // check which task will be excute nexttime.
    tPMS_read.forceNextIteration();
}

void pms_init_handle()
{
    Serial.print(__FUNCTION__);
    Serial.print(", ");
    Serial.print(millis());
    Serial.println("++");

    // pms.passiveMode();
    // tPMS_wakeup.setIterations(TASK_FOREVER);
    // tPMS_wakeup.restart();
    // tPMS_wakeup.forceNextIteration();

    tPMS_read.setIterations(TASK_FOREVER);
    tPMS_read.restart();
    tPMS_read.restartDelayed();
}

uint8_t pms_get_index(uint16_t pm25, uint16_t pm10)
{
    //              PM10    PM2.5
    // 0: unknown
    // 1: excellent 0-15    0-8
    // 2: good      16-40   9-20
    // 3: fair      41-50   21-25
    // 4: inferior  51-75   26-37
    // 5: poor      76-     38-
    uint8_t pm25_idx = 0;
    uint8_t pm10_idx = 0;
    uint8_t pm_idx = 0;

    // get PM2.5 index
    if (38 <= pm25)
        pm25_idx = 5;
    else if (26 <= pm25)
        pm25_idx = 4;
    else if (21 <= pm25)
        pm25_idx = 3;
    else if (9 <= pm25)
        pm25_idx = 2;
    else if (0 <= pm25)
        pm25_idx = 1;

    // get PM10 index
    if (76 <= pm10)
        pm10_idx = 5;
    else if (51 <= pm10)
        pm10_idx = 4;
    else if (41 <= pm10)
        pm10_idx = 3;
    else if (16 <= pm10)
        pm10_idx = 2;
    else if (0 <= pm10)
        pm10_idx = 1;

    // get Air Quality index
    if (pm25_idx < pm10_idx)
        pm_idx = pm10_idx;
    else
        pm_idx = pm25_idx;

    return pm_idx;
}

void pms_read_handle()
{
    // Serial.print(__FUNCTION__);
    // Serial.print(", ");
    // Serial.print(millis());
    // Serial.println("++");

#if 1
    if (pms.readUntil(pms_data, 500))
    {
#if 1
        Serial.print("PM 1.0 (ug/m3): ");
        Serial.println(pms_data.PM_AE_UG_1_0);

        Serial.print("PM 2.5 (ug/m3): ");
        Serial.println(pms_data.PM_AE_UG_2_5);

        Serial.print("PM 10.0 (ug/m3): ");
        Serial.println(pms_data.PM_AE_UG_10_0);
#endif
        h_aq_i.value.uint8_value = pms_get_index(pms_data.PM_AE_UG_2_5, pms_data.PM_AE_UG_10_0);
        homekit_characteristic_notify(&h_aq_i, h_aq_i.value);
        h_aq_pm25.value.float_value = (float)pms_data.PM_AE_UG_2_5;
        homekit_characteristic_notify(&h_aq_pm25, h_aq_pm25.value);
        h_aq_pm10.value.float_value = (float)pms_data.PM_AE_UG_10_0;
        homekit_characteristic_notify(&h_aq_pm10, h_aq_pm10.value);
    }
    else
    {
        tPMS_read.restartDelayed();
    }
#endif

//   pms.passiveMode();
}

void bme280_read_asap()
{
    Serial.print(__FUNCTION__);
    Serial.print(", ");
    Serial.print(millis());
    Serial.println("++");

    // check which task will be excute nexttime.
    tBME280_wakeup.forceNextIteration();
}

void bme280_init_handle()
{
    Serial.print(__FUNCTION__);
    Serial.print(", ");
    Serial.print(millis());
    Serial.println("++");

    do
    {
        if (!bme.begin(BME280_I2C_ADDRESS))
        {
            Serial.println("Could not find a valid BME280 sensor, check wiring!");
           break;
        }

        tBME280_wakeup.setIterations(TASK_FOREVER);
        tBME280_wakeup.restart();
        tBME280_wakeup.forceNextIteration();
    } while (false);
}

void bme280_wakeup_handle()
{
    // Serial.print(__FUNCTION__);
    // Serial.print(", ");
    // Serial.print(millis());
    // Serial.println("++");

    // Set normal mode
    // bme.setSampling(Adafruit_BME280::MODE_NORMAL, Adafruit_BME280::SAMPLING_X16,
    //     Adafruit_BME280::SAMPLING_NONE, Adafruit_BME280::SAMPLING_X16,
    //     Adafruit_BME280::FILTER_X16, Adafruit_BME280::STANDBY_MS_0_5);
    bme.setSampling(Adafruit_BME280::MODE_FORCED, Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::SAMPLING_NONE, Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::FILTER_OFF, Adafruit_BME280::STANDBY_MS_0_5);

    tBME280_read.restartDelayed();
}

void bme280_sleep_handle()
{
    // Serial.print(__FUNCTION__);
    // Serial.print(", ");
    // Serial.print(millis());
    // Serial.println("++");

    // Set sleep mode
    bme.setSampling(Adafruit_BME280::MODE_SLEEP, Adafruit_BME280::SAMPLING_NONE,
        Adafruit_BME280::SAMPLING_NONE, Adafruit_BME280::SAMPLING_NONE,
        Adafruit_BME280::FILTER_X16, Adafruit_BME280::STANDBY_MS_1000);
}

void bme280_read_handle()
{
    // Serial.print(__FUNCTION__);
    // Serial.print(", ");
    // Serial.print(millis());
    // Serial.println("++");

    do
    {
        bme.takeForcedMeasurement();
        float temp = bme.readTemperature();

        if (isnan(temp))
        {
            Serial.println("Temperature NAN\n");
            break;
        }

        // check BME280 temperature validation
        if ((-40 > temp ) || (85 < temp))
        {
            Serial.print("Temperature out of range: ");
            Serial.println(temp);
            break;
        }

        float humi = bme.readHumidity();

        if (isnan(humi))
        {
            Serial.println("Humidity NAN\n");
            break;
        }

        // check BME280 humidity validation
        if ((0 > humi) && (100 <= humi))
        {
            Serial.print("Humidity out of range: ");
            Serial.println(humi);
            break;
        }
#if 1
        Serial.print("Humidity: ");
        Serial.print(humi);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.println(" *C ");
#endif
#if 1
        // Temperature Sensor Service
        h_temp_s_c.value.float_value = temp;
        homekit_characteristic_notify(&h_temp_s_c, h_temp_s_c.value);

        // Humidity Sensor Service
        h_humi_s_c.value.float_value = humi;
        homekit_characteristic_notify(&h_humi_s_c, h_humi_s_c.value);

        // Thermostat Service
        h_thermo_c_temp.value.float_value = temp;
        homekit_characteristic_notify(&h_thermo_c_temp, h_thermo_c_temp.value);
        h_thermo_c_humi.value.float_value = humi;
        homekit_characteristic_notify(&h_thermo_c_humi, h_thermo_c_humi.value);
#endif
    } while (false);

    tBME280_sleep.restartDelayed();
}

void setup_homekit()
{
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.macAddress(mac);
    int name_len = snprintf(NULL, 0, "%s_%02X%02X%02X", h_name.value.string_value, mac[3], mac[4], mac[5]);
    char *name_value = (char*)malloc(name_len + 1);
    snprintf(name_value, name_len + 1, "%s_%02X%02X%02X", h_name.value.string_value, mac[3], mac[4], mac[5]);
    h_name.value = HOMEKIT_STRING_CPP(name_value);

    current_heating_cooling_state = 0;
    target_heating_cooling_state = 0;
    target_temperature = 10;
    temperature_display_unit = 0;

    arduino_homekit_setup(&h_config);
}

void setup_pms()
{
    tPMS_init.restartDelayed();
}

void setup_ir()
{
    irsend.begin();
}

void setup_bme280()
{
    tBME280_init.restartDelayed();
}

 void setup_ota()
{
    ArduinoOTA.setHostname(OTA_NAME);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
        {
            type = "sketch";
        }
        else
        { // U_SPIFFS
            type = "filesystem";
        }

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
        {
            Serial.println("Auth Failed");
        }
        else if (error == OTA_BEGIN_ERROR)
        {
            Serial.println("Begin Failed");
        }
        else if (error == OTA_CONNECT_ERROR)
        {
            Serial.println("Connect Failed");
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
            Serial.println("Receive Failed");
        }
        else if (error == OTA_END_ERROR)
        {
            Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();
}

void setup() {
    Serial.begin(9600);
    Serial.setRxBufferSize(32);
    Serial.setDebugOutput(false);

    WiFi.mode(WIFI_STA);
    WiFi.persistent(false);
    WiFi.disconnect(false);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);

    printf("\n");
    printf("SketchSize: %d B\n", ESP.getSketchSize());
    printf("FreeSketchSpace: %d B\n", ESP.getFreeSketchSpace());
    printf("FlashChipSize: %d B\n", ESP.getFlashChipSize());
    printf("FlashChipRealSize: %d B\n", ESP.getFlashChipRealSize());
    printf("FlashChipSpeed: %d\n", ESP.getFlashChipSpeed());
    printf("SdkVersion: %s\n", ESP.getSdkVersion());
    printf("FullVersion: %s\n", ESP.getFullVersion().c_str());
    printf("CpuFreq: %dMHz\n", ESP.getCpuFreqMHz());
    printf("FreeHeap: %d B\n", ESP.getFreeHeap());
    printf("ResetInfo: %s\n", ESP.getResetInfo().c_str());
    printf("ResetReason: %s\n", ESP.getResetReason().c_str());

    setup_ota();
    setup_bme280();
    setup_pms();
    setup_ir();

    DEBUG_HEAP();
    setup_homekit();
    DEBUG_HEAP();

    pinMode(LED_BUILTIN , OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    ts.execute();
}
