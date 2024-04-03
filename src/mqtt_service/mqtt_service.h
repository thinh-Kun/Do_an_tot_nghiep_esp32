#ifndef ML_SERVICE_MQTT_
#define ML_SERVICE_MQTT_

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "app/ad5941_app/ad5941_app.h"

typedef enum STATE{
    IDLE,
    CONTROL_VOL,
    READ_CV,
    READ_EIS,
    STATE_NUMBER
}STATE;
extern WiFiClient espClient;
extern PubSubClient client;
extern STATE g_state;

void setup_wifi(void);
void reconnect(void);
void callback(char* topic, byte * message, unsigned int length);

#endif /*ML_SERVICE_MQTT_*/