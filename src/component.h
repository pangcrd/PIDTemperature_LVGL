#ifndef COMPONENT_H
#define COMPONENT_H

#include "Arduino.h"
#include <lvgl.h>
#include <ui.h>

#define NTC_PIN 26
#define BUTTON_PIN 35 
#define pwmPin 23

// PID value tune by yourself. ĐM mất 2 ngày mới chỉnh được đúng chỉ số [Tiêu chuẩn +-2]
extern float kp;
extern float ki;
extern float kd;

extern float currentTemp;
extern float previousError;
extern float integral;
extern float pwmValue;

extern float elapsedTime, Time, timePrev;


extern volatile bool PIDrun;

extern float setpoint;

float readNTCTemp();
void PIDcontrol();
void IRAM_ATTR handleButtonPress();
void StopHeat();



#endif