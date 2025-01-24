#include  "component.h"

float kp = 1.3;
float ki = 1.1;
float kd = 0.02;

float currentTemp = 0.0;
float previousError = 0.0;
float integral = 0.0;
float output = 0.0;

volatile bool PIDrun = true;

float setpoint = 50.0;

float elapsedTime, Time, timePrev;




/*========== Read NTC thermistor ==========*/
 float readNTCTemp() {

    int adcValue = analogRead(NTC_PIN);  //Using GPIO support ADC feature
    float voltageNTC = 3.3 / 4095.0 * adcValue; //ADC value ESP32 is 3.3v ADCs (resolution: 212=4096 2 12 = 4096 )
    float currentNTC = (3.3 - voltageNTC) / 100000.0;  //NTC 100k => 100000.0
    float Res_NTC = voltageNTC / currentNTC;
    float temperature = 1 / (log(Res_NTC / 100000.0) / 3950.0 + 1 / (25.0 + 273.15)) - 273.15;// Measure NTC thermistor. GOOGLE IT!

    return temperature;
    //Serial.println("Current Temp: " + String(temperature));
}
/*========== Read NTC thermistor END ==========*/

void PIDcontrol(){ 
    // First we read the real value of temperature
    currentTemp = readNTCTemp();
    //Next we calculate the error between the setpoint and the real value
    float error = setpoint - (currentTemp + 5);// "number" is setpoint value
    //Calculate the P value
    float proportional = 0.01*kp *error;
    //Calculate the I value in a range on +-3
    integral = 0.01*integral +(ki*error);
    //For derivative we need real time to calculate speed change rate
    timePrev = Time;
    Time = millis();  
    elapsedTime = (Time - timePrev) / 1000;   
    float derivative = 0.01*kd*((error - previousError)/elapsedTime);
    // calcula PWM from PID (Ut)
    output = proportional + integral + derivative;
    //Limit the PWM value from 0 to 255
    if (output >= 255) output = 255; //PWM: 0-255
    if (output <= 0) output = 0;

    ledcWrite(0, output);
    previousError = error;

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