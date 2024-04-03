#ifndef ML_APP_AD5941_
#define ML_APP_AD5941_

#include "library/ad5940.h"
#include "library/Impedance.h"
#include "library/RampTest.h"

extern float S_Vol;
extern float E_Vol;
extern int StepNumber;
extern int RepeatTimes;
extern BoolFlag logEn;

void AD5940_CV_Main(void);
void AD5940_EIS_Main(void);

#endif  /*ML_APP_AD5941_*/