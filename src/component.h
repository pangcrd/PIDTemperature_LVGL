#ifndef COMPONENT_H
#define COMPONENT_H

#include "Arduino.h"
#include <lvgl.h>
#include <ui.h>
#include "PID_v1.h"

#define NTC_PIN 26
#define BUTTON_PIN 35 
#define pwmPin 23


extern  double dt, last_time;
extern  double integral, previous, output;
extern  double kp, ki, kd;
extern  double setpoint, Input, Output;
extern  PID myPID;

extern volatile bool PIDrun;

float readNTCTemp();
void PIDcontrol();
void IRAM_ATTR handleButtonPress();
void Cooling ();
void StopHeat();



#endif