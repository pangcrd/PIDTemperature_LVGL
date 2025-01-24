/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Example PID temperture control. For TFT LCD use Rotary Encoder + ESP32  // 
// Design UI on Squareline Studio. LVGL V9.1                               //
// Youtube:https://www.youtube.com/@pangcrd                                //
// Github: https://github.com/pangcrd                                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include <TFT_eSPI.h>
#include "EncoderRead.h"
#include "component.h"

#define BUZZER_PIN 25

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/
EncoderRead encoder(33, 32, 34); //PinA, PinB,buttons (PinA and PinB must be connected to interrupt-supported pins).

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

enum { SCREENBUFFER_SIZE_PIXELS = screenWidth * screenHeight / 10 };
static lv_color_t buf [SCREENBUFFER_SIZE_PIXELS];

TFT_eSPI tft = TFT_eSPI( screenWidth, screenHeight ); /* TFT instance */

//declare the function to create a group and input device for the rotary encoder
lv_indev_t *encoder_indev = NULL;

lv_group_t *groups[2]; 
lv_obj_t *screens[2];  

int current_screen = 0;// Default is screen 1

float Step[] = {1.0f, 0.1f, 0.01f,10.0f,100.0f}; //Step for setting temperture

int decimal = 0;
int current_index = 0;

bool setBuzzerFlag = false;


/* Display flushing */
void my_disp_flush (lv_display_t *disp, const lv_area_t *area, uint8_t *pixelmap)
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    if (LV_COLOR_16_SWAP) {
        size_t len = lv_area_get_size( area );
        lv_draw_sw_rgb565_swap( pixelmap, len );
    }

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( (uint16_t*) pixelmap, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

/*========== Read the encoder rotary ==========*/
void my_encoder_read (lv_indev_t *indev_drv, lv_indev_data_t * data)
{
    
    static int32_t last_counter = 0;

    int32_t counter = encoder.getCounter();
    bool btn_state = encoder.encBtn();

    data->enc_diff = counter - last_counter;
    data->state = btn_state ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

    last_counter = counter;
}

/*Set tick routine needed for LVGL internal timings*/
static uint32_t my_tick_get_cb (void) { return millis(); }

/*========== Group and config input device ==========*/

 void group_init(){

    //Initialize the Rotary Encoder input device. For LVGL version 9+ only
    lv_indev_t *encoder_indev = lv_indev_create();
    lv_indev_set_type(encoder_indev, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(encoder_indev, my_encoder_read);

    /*Initialize the Rotary Encoder input device. For LVGL version 8.3 only*/
    // static lv_indev_drv_t indev_drv;
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_ENCODER;
    // indev_drv.read_cb = encoder_read;
    // encoder_indev = lv_indev_drv_register(&indev_drv);
    /*DO NOT USE IN THIS CODE. CAUSE NOT WORK WITH LVGL V9+*/

    // Number of screen and group. I use 2 group for 2 screen.
    screens[0] = ui_Screen1;
    screens[1] = ui_Screen2;

    for (int i = 0; i < 2; i++) {
        groups[i] = lv_group_create(); 
    }

    // Add obj into group
    lv_group_add_obj(groups[0], ui_Button1);
    lv_group_add_obj(groups[0], ui_Button2);
    lv_group_add_obj(groups[0], ui_Button3);
    lv_group_add_obj(groups[0], ui_Button7);
    lv_group_add_obj(groups[0], ui_Button6);
    lv_group_add_obj(groups[0], ui_Button4);
    lv_group_add_obj(groups[1], ui_Button5);
    
    // Set group for encoder.
    lv_indev_set_group(encoder_indev, groups[0]);
    lv_scr_load(screens[0]);
 }

/**========== Buzzer beeper START ==========*/
void buzzer(){
    static uint32_t buzzerTimePrev = 0L;
    static boolean buzzerIsBusy = false;

        if (setBuzzerFlag && !buzzerIsBusy) {
                buzzerTimePrev = millis();
                setBuzzerFlag = false; // Reset flag
                buzzerIsBusy = true;
                digitalWrite(BUZZER_PIN, HIGH);
            }

            if (buzzerIsBusy && millis() - buzzerTimePrev > 75) {
                buzzerIsBusy = false;
                digitalWrite(BUZZER_PIN, LOW);
            }
}
 /*========== Buzzer beeper END   ==========*/

/*========== for show graph ==========*/

void graph(lv_timer_t * timer){

        float NTCtemp = readNTCTemp(); 
        //Serial.println(NTCtemp);
        
        /*Chart display*/
        lv_chart_set_next_value(ui_Chart1, ui_Chart1_series_1, NTCtemp);
        lv_chart_refresh(ui_Chart1); /*Refesh chart*/
        

        /*Temperture show*/
        String tempString = String(NTCtemp, 1)+ "\u00B0C";  //Symbol degree
        const char* tempValue = tempString.c_str();   // Convert string to const char*
        lv_label_set_text(ui_Label4, tempValue);
        lv_label_set_text(ui_Label8, tempValue);

        

}
/*========== for show graph END==========*/
void GUI_action(){
     lv_obj_t *focused_obj = lv_group_get_focused(groups[current_screen]);
    // update group when switch screen
    lv_indev_set_group(encoder_indev, groups[current_screen]);

    if (encoder.encBtn()) {
        setBuzzerFlag = true;
        if (current_screen == 0) { // Screen 1
            if (focused_obj == ui_Button4) {
                lv_scr_load(screens[1]); // Switch to screen 2
                current_screen = 1; // Current screen is screen 2

            }  else if (focused_obj == ui_Button1) { //Button click then run code
   
                     setpoint -= Step[current_index];
                    _ui_label_set_property(ui_Label2, _UI_LABEL_PROPERTY_TEXT, (String(setpoint, decimal) + " \u00B0C").c_str());

                } else if (focused_obj == ui_Button2) {

                    setpoint += Step[current_index];
                    _ui_label_set_property(ui_Label2, _UI_LABEL_PROPERTY_TEXT, (String(setpoint,decimal) + " \u00B0C").c_str());

                } else if (focused_obj == ui_Button3) {
                     //control function Here
                        PIDcontrol();
                        lv_label_set_text(ui_Label6, "RUN");
                        Serial.println("Set Temp: " + String(setpoint));

                } else if (focused_obj == ui_Button7 ) {
                    current_index = (current_index - 1 + 5) % 5;
                    decimal = current_index; 
                    _ui_label_set_property(ui_Label3, _UI_LABEL_PROPERTY_TEXT, (String(Step[current_index])).c_str());
                } else if (focused_obj == ui_Button6 ) {
                    current_index = (current_index + 1) % 5;
                    decimal = current_index; 
                    _ui_label_set_property(ui_Label3, _UI_LABEL_PROPERTY_TEXT, (String(Step[current_index])).c_str());
                }

        } else if (current_screen == 1) { // Screen 2
            if (focused_obj == ui_Button5) {
                lv_scr_load(screens[0]); 
                current_screen = 0; 
            }
        }
    }
}

void setup ()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    setBuzzerFlag = true;
    ledcSetup(0, 5000, 8);
    ledcAttachPin(23,0);// xuất pwm ra chân 23 kênh 0

    // String LVGL_Arduino = "Hello Arduino! ";
    // LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    // Serial.println( LVGL_Arduino );
    // Serial.println( "I am LVGL_Arduino" );
    encoder.begin();

    pinMode(25, OUTPUT);

    pinMode(BUTTON_PIN, INPUT_PULLDOWN); 
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonPress, FALLING); 

    lv_init();

// #if LV_USE_LOG != 0
//     lv_log_register_print_cb( my_print ); /* register print function for debugging */
// #endif

    tft.begin();          /* TFT init */
    tft.setRotation( 1 ); /* Landscape orientation, flipped */

    static lv_disp_t* disp;
    disp = lv_display_create( screenWidth, screenHeight );
    lv_display_set_buffers( disp, buf, NULL, SCREENBUFFER_SIZE_PIXELS * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL );
    lv_display_set_flush_cb( disp, my_disp_flush );


    lv_tick_set_cb( my_tick_get_cb );

    ui_init();
    group_init();

    lv_timer_t* timer = lv_timer_create(graph,1000,NULL); //Update sensor value after 2 second

    Serial.println( "Setup done" );
}



void loop ()
{   
    GUI_action();
    buzzer();
    StopHeat();
    //Serial.println("PWM: " + String(Output));
    lv_timer_handler(); /* let the GUI do its work */
    delay(5);
    
    
}
