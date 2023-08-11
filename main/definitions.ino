#include <vector.h>

// DECLARING ALL PINS USED
/* DISPENSING MOTOR PINS */
const int DISPENSOR_ACTUATOR_pin1 = 22;
const int DISPENSOR_ACTUATOR_pin2 = 24;
const int DISPENSOR_ACTUATOR_pin3 = 26;
const int DISPENSOR_ACTUATOR_pin4 = 28;

/* LED DISPLAY PINS */
const int COLOR_SELECTION_BUTTON_PIN = 11;
const int R_LED_PIN = 11;
const int Y_LED_PIN = 11;
const int B_LED_PIN = 11;
const int MOLE_LOADED_LED_PIN =  11;

/* GAME MODE PINS */
const int GAME_MODE_BUTTON_PIN = 4;
const int GAME_MODE_LED_PIN = 9;

/* COLOR SENSOR PIN */
const int DETECTED_COLOR_PIN = 11;

/* MOLE LOADED PIN */
const int MOLE_LOADED_PIN = A4;

/* MOTOR PINS */
const int LEFT_MTR_ENABLE_PIN = 11;
const int LEFT_MTR_IN1_PIN = 11;
const int LEFT_MTR_IN2_PIN = 11;
const int LEFT_MTR_ENCODER_PIN = 11;

const int RIGHT_MTR_ENABLE_PIN = 11;
const int RIGHT_MTR_IN1_PIN = 11;
const int RIGHT_MTR_IN2_PIN = 11;
const int RIGHT_MTR_ENCODER_PIN = 11;

std::vector<const int> OUTPUT_PIN_LS = {DISPENSOR_ACTUATOR_pin1, DISPENSOR_ACTUATOR_pin2, DISPENSOR_ACTUATOR_pin3, DISPENSOR_ACTUATOR_pin4, R_LED_PIN, Y_LED_PIN, B_LED_PIN, MOLE_LOADED_LED_PIN, GAME_MODE_LED_PIN, LEFT_EN_MTR_PIN, LEFT_IN1_MTR_PIN, LEFT_IN2_MTR_PIN, RIGHT_EN_MTR_PIN, RIGHT_IN1_MTR_PIN, RIGHT_IN2_MTR_PIN};

/* GLOBAL CONSTANTS */
const unsigned long STEPS = 32;
const int SPEED = 60;
const unsigned long PLACE_DELAY = 1000;
const byte setter = B11100000;
const byte brightness = B00001111;
const int MOLE_DIST = 50;

/* GLOBAL VARIABLES */
// FSM Inputs
int SRT = 0;
int DRED = 0;
int DBLU = 0;
int DYLW = 0;
int RED = 0;
int BLU = 0;
int YLW = 0;
int ECDR = 0;
int LEFT = 0;
int RGHT = 0;
int MOLE = 0;

// FSM Outputs
int LMTR = 0;
int RMTR = 0;
int PLCE = 0;
int LED = 0;

int state = 100; // State variable

/* Sensor variables */
byte red_var;
byte green_var;
byte blue_var;
int ir_reading;
int lux;

/* Timing variables */
bool t_started = false;
unsigned long start;

/* Start variables */
unsigned long debounceDelay = 50; // ms
int last_srt_reading = LOW;

/* Temporary variables for testing */
int DCLR;
int CLR;
int inputs = 0;

/* STATE NAMES */
const int S0 = 100;
const int S1 = 101;
const int S2 = 102;
const int S3 = 103;
const int S4 = 104;
const int S5 = 105;
const int S6 = 106;
const int S7 = 107;

// FUNCTION DEFINITIONS
union myunion {
  int i;
  long i_long;
  float f;
  unsigned int i_u;
};

/* FUNCTION THAT CONVERTS DISTANCE IN CM TO ENCODER COUNT */
unsigned long dis2enc(myunion distance) {
  float D = 10.5; //Diameter of wheel in centimetres
  return((unsigned long) 3.141 * D / 32);
}
