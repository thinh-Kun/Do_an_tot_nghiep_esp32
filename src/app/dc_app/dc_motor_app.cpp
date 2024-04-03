#include "dc_motor_app.h"
#include <AccelStepper.h>
#include "mqtt_service/mqtt_service.h"

#define VOLTAGE_NONE    0
#define VOLTAGE_1       1
#define VOLTAGE_2       2

#define CMD_EXTRA_NONE  0
#define CMD_EXTRA_SUB   1
#define CMD_SUB         2
#define CMD_ADD         3
#define CMD_EXTRA_ADD   4

#define VALID_VOLTAGE_ERROR 1 /*V*/
#define MAX_ATTEMP 10

#define WAIT_TIME 6000 /*ms*/
#define HALFSTEP 8
float targetVoltage1; //gia tri Voltage tra ve cho stepper1
float curentVoltage1 = 0; //gia tri Voltage nhap vao cho stepper1
float targetVoltage2; //gia tri Voltage tra ve cho stepper2
float curentVoltage2 = 0;; //gia tri Voltage nhap vao cho stepper2
// ULN2003 Motor Driver Pins
#define motorPin1  13     // IN1 on the ULN2003 driver 1
#define motorPin2  12     // IN2 on the ULN2003 driver 1
#define motorPin3  14     // IN3 on the ULN2003 driver 1
#define motorPin4  27     // IN4 on the ULN2003 driver 1

#define motorPin5  26    // IN1 on the ULN2003 driver 2
#define motorPin6  25    // IN2 on the ULN2003 driver 2
#define motorPin7  33   // IN3 on the ULN2003 driver 2
#define motorPin8  32   // IN4 on the ULN2003 driver 2

float StepsPerVol = 37.4064  ; // so buoc tren 1 Vol
int stepperSpeed = 400; //speed of the stepper (steps per second)

// initialize the ads library
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
// initialize the stepper library
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
AccelStepper stepper2(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; 
#define HAVE_VOLTAGE_CONTROL 1
#ifdef HAVE_VOLTAGE_CONTROL 
void readVoltage(void){

  int16_t adc0, adc1;
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  curentVoltage1 = ads.computeVolts(adc0) * 18;  // Nhân với 1 hàm tuyến tính (chưa có công thúc), Tại đây không dùng map vì map trả về kiểu Int (Tự làm tròn)
  curentVoltage2 = ads.computeVolts(adc1) * 18;

#ifdef DEBUG
  Serial.print("AIN0: "); 
  Serial.print(adc0);
  Serial.print("\tVoltage1: ");
  Serial.println(curentVoltage1, 7); 
  Serial.println();

  Serial.print("AIN1: "); 
  Serial.print(adc1);
  Serial.print("\tVoltage2: ");
  Serial.println(curentVoltage2, 7); 
  Serial.println();
#endif
}

long volToSteps(float vol1,float vol2){
  long a;
  a = (vol2 - vol1)*StepsPerVol;
  return a;
}

void VoltageCtrl_Main(void) {
  int attempCount = 0;
  long currPos = 0;
  float voltage1Error;
  float voltage2Error;
  int isSuccess = 0;

  const float targetVoltage1_l = targetVoltage1;
  const float targetVoltage2_l = targetVoltage2;

  readVoltage();
  voltage1Error = fabs(targetVoltage1_l - curentVoltage1);
  voltage2Error = fabs(targetVoltage2_l - curentVoltage2);

#ifdef DEBUG
  Serial.println("Voltage: " + String(curentVoltage1) + " " + String(curentVoltage2));
  Serial.println("Error: " + String(voltage1Error) + " " + String(voltage2Error));
#endif
  do {
    if (voltage1Error > VALID_VOLTAGE_ERROR) {
     long relative_step = volToSteps(curentVoltage1, targetVoltage1_l);
      stepper1.move(relative_step);
#ifdef DEBUG
      currPos = stepper1.currentPosition();
      Serial.print("Stepper1:\trelative step: ");
      Serial.print(relative_step);
      Serial.print("\tpos before: ");
      Serial.print(currPos);
#endif
      stepper1.runToPosition();
#ifdef DEBUG
      currPos = stepper1.currentPosition();
      Serial.print("\tpos after: ");
      Serial.println(currPos);
#endif
    }

    if (voltage2Error > VALID_VOLTAGE_ERROR) {
      long relative_step = volToSteps(curentVoltage2, targetVoltage2_l);
      stepper2.move(relative_step);
#ifdef DEBUG
      currPos = stepper2.currentPosition();
      Serial.print("Stepper2:\trelative step: ");
      Serial.print(relative_step);
      Serial.print("\tpos before: ");
      Serial.print(currPos);
#endif
      stepper2.runToPosition();
#ifdef DEBUG
      currPos = stepper2.currentPosition();
      Serial.print("\tpos after: ");
      Serial.println(currPos);
#endif
    }
    attempCount++;
    Serial.println("Wait for 6s");
    delay(WAIT_TIME);

    /*read voltage values after rotate steppers */
    readVoltage();
    voltage1Error = fabs(targetVoltage1_l - curentVoltage1);
    voltage2Error = fabs(targetVoltage2_l - curentVoltage2);

    if (voltage1Error < VALID_VOLTAGE_ERROR && voltage2Error < VALID_VOLTAGE_ERROR) 
      isSuccess = 1;
    /*send result to UI*/
    DynamicJsonDocument data_send(256);
    data_send["cmd"] = "data";
    data_send["object"] = "vol";
    JsonObject attribute = data_send.createNestedObject("attribute");
    attribute["vol1"] = curentVoltage1;
    attribute["vol2"] = curentVoltage2;
    String Jdata;
    serializeJson(data_send, Jdata);
    client.publish("Thinh", Jdata.c_str());
    delay(10);
    // Serial.println("0;" + String(attempCount) + ";" + String(MAX_ATTEMP) + ";"  + String(isSuccess) + ";" + String(curentVoltage1) + ";" + String(curentVoltage2));
  }
  while((voltage1Error > VALID_VOLTAGE_ERROR || voltage2Error > VALID_VOLTAGE_ERROR) && attempCount < MAX_ATTEMP);
}
#endif

// hàm xử lý ngắt
void IRAM_ATTR onTimer() {   
  portENTER_CRITICAL_ISR(&timerMux); //vào chế độ tránh xung đột
  //sendVoltage();
  portEXIT_CRITICAL_ISR(&timerMux); // thoát 
}

void dc_motor_init(){
    stepper1.setMaxSpeed(1000.0); // toc do max cua dong co
    stepper1.setAcceleration(250);// gia toc
    stepper1.setSpeed(stepperSpeed);// toc do hien tai

  
    stepper2.setMaxSpeed(1000.0); // toc do max cua dong co
    stepper2.setAcceleration(250);// gia toc
    stepper2.setSpeed(stepperSpeed);// toc do hien tai
}