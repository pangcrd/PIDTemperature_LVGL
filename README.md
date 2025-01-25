# 📚 DIY PID Temperature Controller Use LVGL + ESP32

## 🗯️ About this project
- 🌟I'm using ESP32 38 PIN board.Change your board in [platformio.ini](https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/platformio.ini) file.
- 🌟Design UI on [Squareline Studio](https://squareline.io/)V1.5 
- 🌟Change your SPI PIN in [User_Setup.h](https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/lib/TFT_eSPI/User_Setup.h) file.
- 🌟Using rotary encoder for control UI, How to change [Input device](https://docs.lvgl.io/9.1/overview/indev.html#indev-keys) <<Click
- 🌟Use can create Setpoint upto 1000 °C, Base on your hardware : Temperature sensor, driver control, cooling system.
- 🌟In this test i use Hotend + NTC thermistor
- 🌟You can integrate IOT into this small project so you can control it via the Internet. Take full advantage of the power of the ESP32.

## 🛠 How to use
- ⚡PID value you can config in [PIDNTC.cpp](https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/src/PIDNTC.cpp) at line 3.
- ⚡Prevent over-shoot temperature with colling fan.
- ⚡Set your setpoint value with UP and DOWN icon, click right side button to RUN.  
![UPDOWN](https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/img/hdsd1.PNG)
- ⚡Click RED icon below setpoint tab to set step. Default 1.0, can set 0.01/0.1/1.0/10.0/100.0.Add or remove step goto [main.cpp](https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/src/main.cpp) line 36.  
![STEP](https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/img/hdsd2.PNG)  
- ⚡Status current temperature and state of controller.  
![STATE](https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/img/hdsd3.PNG)
- ⚡Use graphs to visually display temperature values.  
![GRAPH](https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/img/hdsd4.PNG)    

## 🔎 Schematic
![schematic](https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/img/pic3.png)

## 📷 Some picture
<table>
  <tr>
    <td><img src="https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/img/pic1.PNG" alt="Image 1" width="400"/></td>
    <td><img src="https://github.com/pangcrd/PIDTemperature_LVGL/blob/main/img/pic2.PNG" alt="Image 2" width="400"/></td>  
  </tr>
</table>  
## 🎬 Demo video test 👇  

📺 [Youtube Channel](https://www.youtube.com/shorts/YZ69Bs-Csz8).

