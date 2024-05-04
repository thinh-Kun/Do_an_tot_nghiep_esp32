
#include <mqtt_service/mqtt_service.h>
#include <app/dc_app/dc_motor_app.h>

const char* ssid_wifi = "thinh";
const char* pass_wifi = "12345678";
const char* ssid_server = "admin"; //
const char* pass_server = "admin13589"; //
WiFiClient espClient;
PubSubClient client(espClient);
STATE g_state = IDLE;

void setup_wifi(void){
    delay(10);
    WiFi.begin(ssid_wifi, pass_wifi);
    while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect(void){
    while (!client.connected()){
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        // if (client.connect(clientId.c_str(), ssid_server, pass_server)){
                  if (client.connect(clientId.c_str())){
            Serial.println("connected");
            client.subscribe("mems_esp");
            client.publish("esp32/status", "Connect Success");
            delay(1000);
        }
        else {
            Serial.print("failed, err_code:");
            Serial.println(client.state());
            delay(5000);
        }
    }
}
void callback(char* topic, byte * message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
    for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  StaticJsonDocument<1024> doc; // Kích thước tùy chọn
  DeserializationError error = deserializeJson(doc, messageTemp);
  if (!error) {
  String cmd = doc["cmd"];
  String object = doc["object"];
  if (cmd == "control")
  {
    if (object == "vol"){
      targetVoltage1 = doc["attribute"]["vol1"];
      targetVoltage2 = doc["attribute"]["vol2"];
      g_state = CONTROL_VOL;
    }
    else if (object == "cv"){
      S_Vol = doc["attribute"]["start_voltage"];
      E_Vol = doc["attribute"]["end_voltage"];
      StepNumber = doc["attribute"]["step"];
      RepeatTimes = doc["attribute"]["repeat_times"];
      logEn = doc["attribute"]["log_en"];
      g_state = READ_CV;
    }
    else if (object == "eis"){
      S_Vol = doc["attribute"]["start_freq"];
      E_Vol = doc["attribute"]["stop_freq"];
      StepNumber = doc["attribute"]["sweep_point"];
      RepeatTimes = doc["attribute"]["repeat_times"];
      logEn = doc["attribute"]["log_en"];
      g_state = READ_EIS;
    }
  }

    } else {
        Serial.println("Error parsing JSON");
    }

  if (messageTemp == "check")  //COMMAND OF SERVER
  {
    client.publish("mems_server", "OK");
    delay(100);
  }

}