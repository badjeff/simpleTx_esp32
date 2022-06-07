#pragma once

#include <Arduino.h>

#if defined(TARGET_ESP32)

  //xTaskDelay main loop, temporary release thread to another task
  #define ELRS_TASK_LOOP_DELAY    10 // ticks
  #define OUTPUT_TASK_LOOP_DELAY  250 // ticks

  #define USE_GPIO_INPUT_CH
  
  #define ANALOG_IN_PIN_ELEVATOR 32
  #define ANALOG_IN_PIN_AILERON 33
  #define ANALOG_IN_PIN_THROTTLE 34
  #define ANALOG_IN_PIN_RUDDER 35
  #define ANALOG_IN_PIN_AUX1 25  // Arm switch
  #define ANALOG_IN_PIN_AUX2 26  // Mode switch
  #define ANALOG_IN_PIN_AUX3 27  // gpio27
  #define ANALOG_IN_PIN_AUX4 12  // gpio12
  #define VOLTAGE_READ_PIN 36

#elif defined(TARGET_ESP32_S)

  #if defined(TARGET_ESP32_S2)
    //xTaskDelay main loop, temporary release thread to another task
    #define ELRS_TASK_LOOP_DELAY    15 // ticks
    #define OUTPUT_TASK_LOOP_DELAY  500 // ticks
  #elif defined(TARGET_ESP32_S3)
    //xTaskDelay main loop, temporary release thread to another task
    #define ELRS_TASK_LOOP_DELAY    10 // ticks
    #define OUTPUT_TASK_LOOP_DELAY  250 // ticks
  #endif

  // #define USE_GPIO_INPUT_CH
  #define USE_USB_HID_INPUT_CH

  #if defined(USE_GPIO_INPUT_CH)

    #define ANALOG_IN_PIN_ELEVATOR A2 // gpio3
    #define ANALOG_IN_PIN_AILERON A3 // gpio4
    #define ANALOG_IN_PIN_THROTTLE A4 // gpio5
    #define ANALOG_IN_PIN_RUDDER A5 // gpio6
    #define ANALOG_IN_PIN_AUX1 A10  // gpio11 - Arm switch
    #define ANALOG_IN_PIN_AUX2 A11 // gpio12 - Mode switch
    #define ANALOG_IN_PIN_AUX3 A12  // gpio13
    #define ANALOG_IN_PIN_AUX4 A13  // gpio14
    #define VOLTAGE_READ_PIN A0 // gpio0

  #elif defined(USE_USB_HID_INPUT_CH)

    #if defined(TARGET_ESP32_S2)
      #define DAEMON_TASK_LOOP_DELAY  100 // ticks
      #define CLASS_TASK_LOOP_DELAY   15 // ticks
      #define DAEMON_TASK_COREID      0
      #define CLASS_TASK_COREID       0
    #elif defined(TARGET_ESP32_S3)
      #define DAEMON_TASK_LOOP_DELAY  50 // ticks
      #define CLASS_TASK_LOOP_DELAY   6 // ticks
      #define DAEMON_TASK_COREID      1
      #define CLASS_TASK_COREID       1
    #endif

  #endif

#endif

extern bool powerChangeHasRun;

extern uint32_t tickTime;
extern uint16_t rates[];
// click deboucer
static uint32_t clickCurrentMicros = 0;

#define TEMPSTRINGLENGTH 400 // This is the max dialog size (80 characters * 5 lines)
                             // We could reduce this to ~240 on the 128x64 screens
                             // But only after all sprintf are replaced with snprintf
                             // Maybe move this to target_defs.h
extern char tempstring[TEMPSTRINGLENGTH];

void check_link_state(uint32_t currentMicros);
const char *hdr_str_cb(const void *data);
void crsfdevice_init();
void bt_handle(uint8_t value);