
#include  "component.h"

#define NTC_PIN 26
#define BUTTON_PIN 35 
#define pwmPin 23
//#define FAN 16

// PID value tune by yourself.
double Kp = 1.0, Ki= 0.8, Kd= 0.01;

PID myPID(&Input, &Output, &setpoint, Kp, Ki, Kd, DIRECT);

double setpoint = 50, Input, Output;

volatile bool PIDrun = true;


/*========== Read NTC thermistor ==========*/
 float readNTCTemp() {

    int adcValue = analogRead(NTC_PIN);  //Using GPIO support ADC feature
    float voltageNTC = 3.3 / 4095.0 * adcValue; //ADC value ESP32 is 3.3v ADCs (resolution: 212=4096 2 12 = 4096 )
    float currentNTC = (3.3 - voltageNTC) / 100000.0;  //NTC 100k => 100000.0
    float Res_NTC = voltageNTC / currentNTC;
    float temperature = 1 / (log(Res_NTC / 100000.0) / 3950.0 + 1 / (25.0 + 273.15)) - 273.15;// Measure NTC thermistor. GOOGLE IT!

    return temperature;
   
}
/*========== Read NTC thermistor END ==========*/


void PIDcontrol(){ 

  Input = readNTCTemp();
  myPID.SetMode(AUTOMATIC);
  myPID.Compute();
  myPID.SetOutputLimits(0, 255);
  ledcWrite(0,Output);
  
  
}
void IRAM_ATTR handleButtonPress() {
    PIDrun = !PIDrun;  // Chuyển đổi trạng thái PIDrun
}

void StopHeat(){
    if (!PIDrun) {
        ledcWrite(0,0);
        lv_label_set_text(ui_Label6, "STOP");
        return;
    }
}
// void Cooling (){
//     float currentTemp = readNTCTemp();
//         if (currentTemp > setpoint + 2) {
//             analogWrite(FAN, 255);
//     } else if (currentTemp < setpoint + 1) {
//             analogWrite(FAN, 128); 
//     } else {
//             analogWrite(FAN, 0);   
//     }
// }

