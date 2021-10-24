#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include "functional"
typedef std::function<void(int newMode)> ModeSelectEvent;
typedef std::function<void(int percentDone)> UpdateEvent;


// BWF Code for inside and outside
#define INSIDE_BWF          86
#define OUTSIDE_BWF         5

#define LED_PIN 17
#define SWITCH_3_PIN 13
#define SWITCH_BOOT_PIN 0

#ifndef BOARD_VERSION
#define BOARD_VERSION 11
#endif

#if BOARD_VERSION == 10
    #define LEFT_MOTOR_FORWARD_PWM_PIN 27
    #define LEFT_MOTOR_BACKWARDS_PWM_PIN 14
#elif BOARD_VERSION == 11
    #define LEFT_MOTOR_FORWARD_PWM_PIN 14
    #define LEFT_MOTOR_BACKWARDS_PWM_PIN 5
#endif

#define LEFT_MOTOR_PWM_CHANNEL_FORWARD 8
#define LEFT_MOTOR_PWM_CHANNEL_BACKWARDS 7
#define LEFT_MOTOR_SENSE_PIN 34

#define RIGHT_MOTOR_FORWARD_PWM_PIN 12
#define RIGHT_MOTOR_BACKWARDS_PWM_PIN 16
#define RIGHT_MOTOR_PWM_CHANNEL_FORWARD 2
#define RIGHT_MOTOR_PWM_CHANNEL_BACKWARDS 1
#define RIGHT_MOTOR_SENSE_PIN 35

#define CUTTER_MOTOR_FORWARD_PWM_PIN 18

#if BOARD_VERSION == 10
    #define CUTTER_MOTOR_BACKWARDS_PWM_PIN 5
#elif BOARD_VERSION == 11
    #define CUTTER_MOTOR_BACKWARDS_PWM_PIN 27
#endif

#define CUTTER_MOTOR_PWM_CHANNEL_FORWARD 4
#define CUTTER_MOTOR_PWM_CHANNEL_BACKWARDS 3
#define CUTTER_MOTOR_SENSE_PIN 32

#define AUX_MOTOR_FORWARD_PWM_PIN 23
#define AUX_MOTOR_BACKWARDS_PWM_PIN 4
#define AUX_MOTOR_PWM_CHANNEL_FORWARD 5
#define AUX_MOTOR_PWM_CHANNEL_BACKWARDS 6
#define AUX_MOTOR_SENSE_PIN 39

#define LOAD_START_IGNORE_TIME 300
#define LOAD_LIMIT_WHEEL 110
#define LOAD_LIMIT_CUTTER 110
#define LOAD_FILTER 0.01
#define LOAD_LIMIT_AUX 200

#define LEFT_SENSOR_PIN 26
#define RIGHT_SENSOR_PIN 25

#define BATTERY_SENSOR_PIN 33
#define BATTERY_CHARGE_PIN 36

#define BUMPER_PIN 19

#define FULL_SPEED 255
#define NORMAL_ACCELERATION_TIME 400
#define SHORT_ACCELERATION_TIME 100

#define CUTTER_SPEED 255

#define TILT_ANGLE 40;
#define FLIP_ANGLE 80;

#define OPTION_STEP_TIME 750

#define ANALOG_RESOLUTION 11
#define ANALOG_RESOLUTION_MAX_VALUE (pow(2, ANALOG_RESOLUTION) - 1)

#define ERROR_NOERROR 0
#define ERROR_FLIPPED 1
#define ERROR_OUT 2
#define ERROR_STUCK 3
#define ERROR_BUMPER_STUCK 4
#define ERROR_INVALID_OP_MODE 5
#define ERROR_INVALID_BAHAVIOR 6

#endif

