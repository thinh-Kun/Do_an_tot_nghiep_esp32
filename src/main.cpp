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

// const char* my_mqtt_server = "513booyoungct4.ddns.net";
const char* my_mqtt_server = "iotbro.hopto.org";
const int port_id = 1883;

// 1#-500?500/10|2$0!
// 2#100?1000/100|1$0!
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

int send_one_time = 0;
void loop() {
  // put your main code here, to run repeatedly:
//   if (g_state == READ_CV){
//   Serial.println("Send Data!");
//   static uint32_t index = 0;
//   /* Print data*/
//   DynamicJsonDocument data_send_temp(1024);
//   data_send_temp["cmd"] = "data";
//   data_send_temp["object"] = "cv";
//   JsonObject attribute = data_send_temp.createNestedObject("attribute");
//   for(int i = 0; i < 20; i++)
//   {
//     attribute["index"] = i;
//     if (attribute["index"] == 0){
//       attribute["data"] = -44.173531;
//     }
//         if (attribute["index"] == 1){
//       attribute["data"] = -0.244535;
//     }
//         if (attribute["index"] == 2){
//       attribute["data"] = 7.03912;
//     }
//         if (attribute["index"] == 3){
//       attribute["data"] = 15.091314;
//     }
//         if (attribute["index"] == 4){
//       attribute["data"] = 7.126454;
//     }
//         if (attribute["index"] == 5){
//       attribute["data"] = -0.995607;
//     }
//         if (attribute["index"] == 6){
//       attribute["data"] = -3.353625;
//     }
//         if (attribute["index"] == 7){
//       attribute["data"] = -10.366544;
//     }
//             if (attribute["index"] == 8){
//       attribute["data"] = -17.903467;
//     }
//             if (attribute["index"] == 9){
//       attribute["data"] = -4.244432;
//     }
//             if (attribute["index"] == 10){
//       attribute["data"] = -0.183401;
//     }
//             if (attribute["index"] == 11){
//       attribute["data"] = -0.165935;
//     }
//         if (attribute["index"] == 12){
//       attribute["data"] = 7.17012;
//     }
//         if (attribute["index"] == 13){
//       attribute["data"] = 15.641518;
//     }
//         if (attribute["index"] == 14){
//       attribute["data"] = 7.720325;
//     }
//         if (attribute["index"] == 15){
//       attribute["data"] = -0.602605;
//     }
//         if (attribute["index"] == 16){
//       attribute["data"] = -3.283758;
//     }
//         if (attribute["index"] == 17){
//       attribute["data"] = -10.384012;
//     }
//             if (attribute["index"] == 18){
//       attribute["data"] = -18.017004;
//     }
//             if (attribute["index"] == 19){
//       attribute["data"] = -12.523694;
//     }




//     //     attribute["index"] = i;
//     // if (attribute["index"] == 0){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 1){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 2){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 3){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 4){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 5){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 6){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 7){
//     //   attribute["data"] = i+10;
//     // }
//     //         if (attribute["index"] == 8){
//     //   attribute["data"] = i+10;
//     // }
//     //         if (attribute["index"] == 9){
//     //   attribute["data"] = i+10;
//     // }
//     //         if (attribute["index"] == 10){
//     //   attribute["data"] = i+10;
//     // }
//     //         if (attribute["index"] == 11){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 12){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 13){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 14){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 15){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 16){
//     //   attribute["data"] = i+10;
//     // }
//     //     if (attribute["index"] == 17){
//     //   attribute["data"] = i+10;
//     // }
//     //         if (attribute["index"] == 18){
//     //   attribute["data"] = i+10;
//     // }
//     //         if (attribute["index"] == 19){
//     //   attribute["data"] = i+10;
//     // }
//     String Jdata;
//     serializeJson(data_send_temp, Jdata);
//     client.publish("mems_server", Jdata.c_str());
//     delay(10);
    
//     // printf("%d;%.6f\n",index++, pData[i]);
//   }
//   g_state = IDLE;
//   }

//   if (g_state == READ_EIS){
//   Serial.println("Send Data!");
//   static uint32_t index = 0;
//   /* Print data*/
//   DynamicJsonDocument data_send_temp(1024);
//   data_send_temp["cmd"] = "data";
//   data_send_temp["object"] = "eis";
//   JsonObject attribute = data_send_temp.createNestedObject("attribute");
//   for(int i = 100; i < 130; i = i+1)
//   {
//     attribute["freq"] = i;

//     if (attribute["freq"] == 100){
//       attribute["magnitude"] = 6393241.000000;
//       attribute["phase"] = -106.356323;
//     }
//         if (attribute["freq"] == 101){
//       attribute["magnitude"] = 5713914.000000;
//       attribute["phase"] = -87.773361;
//     }
        
// if (attribute["freq"] == 102){
//   attribute["magnitude"] = 3539905.000000;
//   attribute["phase"] = -88.629570;
// }
// if (attribute["freq"] == 103){
//   attribute["magnitude"] = 2549115.250000;
//   attribute["phase"] = -93.431824;
// }
// if (attribute["freq"] == 104){
//   attribute["magnitude"] = 2249437.750000;
//   attribute["phase"] = -93.281212;
// }
// if (attribute["freq"] == 105){
//   attribute["magnitude"] = 1761123.875000;
//   attribute["phase"] = -89.951698;
// }
// if (attribute["freq"] == 106){
//   attribute["magnitude"] = 1663818.500000;
//   attribute["phase"] = -87.440414;
// }
// if (attribute["freq"] == 107){
//   attribute["magnitude"] = 1479253.500000;
//   attribute["phase"] = -92.777969;
// }
// if (attribute["freq"] == 108){
//   attribute["magnitude"] = 1391798.750000;
//   attribute["phase"] = -86.541107;
// }
// if (attribute["freq"] == 109){
//   attribute["magnitude"] = 1256313.750000;
//   attribute["phase"] = -88.180016;
// }
// if (attribute["freq"] == 110){
//   attribute["magnitude"] = 1133563.125000;
//   attribute["phase"] = -90.962219;
// }
// if (attribute["freq"] == 111){
//   attribute["magnitude"] = 1008440.625000;
//   attribute["phase"] = -90.096039;
// }
// if (attribute["freq"] == 112){
//   attribute["magnitude"] = 955661.812500;
//   attribute["phase"] = -89.487808;
// }
// if (attribute["freq"] == 113){
//   attribute["magnitude"] = 890405.187500;
//   attribute["phase"] = -89.827301;
// }
// if (attribute["freq"] == 114){
//   attribute["magnitude"] = 866320.687500;
//   attribute["phase"] = -88.769234;
// }
// if (attribute["freq"] == 115){
//   attribute["magnitude"] = 787600.125000;
//   attribute["phase"] = -91.030434;
// }
// if (attribute["freq"] == 116){
//   attribute["magnitude"] = 770854.937500;
//   attribute["phase"] = -88.695709;
// }
// if (attribute["freq"] == 117){
//   attribute["magnitude"] = 713007.125000;
//   attribute["phase"] = -90.043159;
// }
// if (attribute["freq"] == 118){
//   attribute["magnitude"] = 668429.375000;
//   attribute["phase"] = -89.379509;
// }
// if (attribute["freq"] == 119){
//   attribute["magnitude"] = 649530.625000;
//   attribute["phase"] = -90.436394;
// }
// if (attribute["freq"] == 120){
//   attribute["magnitude"] = 594253.375000;
//   attribute["phase"] = -88.731575;
// }
// if (attribute["freq"] == 121){
//   attribute["magnitude"] = 588069.750000;
//   attribute["phase"] = -88.123611;
// }
// if (attribute["freq"] == 122){
//   attribute["magnitude"] = 575459.250000;
//   attribute["phase"] = -90.487122;
// }
// if (attribute["freq"] == 123){
//   attribute["magnitude"] = 549858.375000;
//   attribute["phase"] = -90.467445;
// }
// if (attribute["freq"] == 124){
//   attribute["magnitude"] = 538516.875000;
//   attribute["phase"] = -89.957626;
// }
// if (attribute["freq"] == 125){
//   attribute["magnitude"] = 516630.031250;
//   attribute["phase"] = -89.838806;
// }
// if (attribute["freq"] == 126){
//   attribute["magnitude"] = 487321.250000;
//   attribute["phase"] = -90.368080;
// }
// if (attribute["freq"] == 127){
//   attribute["magnitude"] = 465958.406250;
//   attribute["phase"] = -89.875504;
// }





//     String Jdata;
//     serializeJson(data_send_temp, Jdata);
//     client.publish("mems_server", Jdata.c_str());
//     delay(10);
    
//     // printf("%d;%.6f\n",time++, pData[i]);
//   }
//   g_state = IDLE;
//   }

  // // Gửi tin nhắn
  // if (/* điều kiện để gửi tin nhắn */) {
  //   client.publish("yourTopic", "Hello World");
  // }

  // 1#-500?500/10|2$0!
      if (Serial.available()) {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, Serial);
        
        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }
        
        const char* value = doc["key"]; // "value"
        Serial.print("Send Oke!");
        // Xử lý giá trị tại đây
    }

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
  vTaskDelay(5);
  }
  
}

