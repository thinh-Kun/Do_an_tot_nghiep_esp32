#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
#include <Arduino.h>
#include <app/ad5941_app/ad5941_app.h>
#include <app/dc_app/dc_motor_app.h>
#include <WiFi.h>
#include "mqtt_service/mqtt_service.h"

const char* my_mqtt_server = "513booyoungct4.ddns.net";
const int port_id = 1883;

// 1#-500?500/10|2$0!
// 2#100?10000/100|2$0!
// 3#30?40/!
DynamicJsonDocument data_send(1024);
void mqttLoopTask(void *parameter);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Begin App!");
  if (!ads.begin(0x48))
  {
    Serial.println("!ADS_Failed");    
    while (1);
  }
  dc_motor_init();
  AD5940_MCUResourceInit(0);
  setup_wifi();
  client.setServer(my_mqtt_server, port_id);
  client.setCallback(callback);
  xTaskCreatePinnedToCore(mqttLoopTask, "mqttLoopTask", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // put your main code here, to run repeatedly:


  // // Gửi tin nhắn
  // if (/* điều kiện để gửi tin nhắn */) {
  //   client.publish("yourTopic", "Hello World");
  // }

  // 1#-500?500/10|2$0!
  if (g_state == CONTROL_VOL){

    VoltageCtrl_Main();
    g_state = IDLE;
    Serial.printf("Sended VOL!\n");
  }
  else if (g_state == READ_CV){
    AD5940_CV_Main(); 
    g_state = IDLE;
    Serial.printf("Sended CV!\n");
  }
  else if (g_state == READ_EIS){
    AD5940_EIS_Main(); 
    g_state = IDLE;
    Serial.printf("Sended EIS!\n");
  }
}

void mqttLoopTask(void *parameter) {
  for (;;){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  vTaskDelay(10);
  }
  
}

