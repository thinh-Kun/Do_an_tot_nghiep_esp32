#ifndef ML_APP_MOTOR_
#define ML_APP_MOTOR_
#include "Adafruit_ADS1X15.h"

void dc_motor_init(void);
void VoltageCtrl_Main(void);
extern Adafruit_ADS1115 ads;
extern float targetVoltage1;
extern float targetVoltage2;
#endif  /*ML_APP_MOTOR_*/