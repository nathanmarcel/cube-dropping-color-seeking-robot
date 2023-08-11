//#include <vector.h>
// FUNCTION DEFINITIONS
union myunion {
  int i;
  long i_long;
  float f;
  unsigned int i_u;
};

/* FUNCTION THAT CONVERTS DISTANCE IN CM TO ENCODER COUNT */
unsigned long dis2enc(unsigned long distance) {
  float D_inches = 2.54;
  float D = D_inches * 2.54; //Diameter of wheel in centimetres
  return((distance / (3.141 * D)) * 85);
}

// DECLARING ALL PINS USED
/* DISPENSING MOTOR PINS */
const int DISPENSOR_ACTUATOR_pin1 = 9; // IN1, IN2, IN3, IN4
const int DISPENSOR_ACTUATOR_pin2 = 8;
const int DISPENSOR_ACTUATOR_pin3 = 7;
const int DISPENSOR_ACTUATOR_pin4 = 6;

/* LED DISPLAY PINS */
const int RED_SELEC_BUTTON_PIN = 46;
const int BLUE_SELEC_BUTTON_PIN = 48;
const int YELLOW_SELEC_BUTTON_PIN = 50;
const int R_LED_PIN = 44;
const int B_LED_PIN = 40;
const int Y_LED_PIN = 36;

/* GAME MODE PINS */
const int GAME_MODE_BUTTON_PIN = 52;
const int GAME_MODE_LED_PIN = 32;

/* COLOR SENSOR PIN */
// These should be handled in the Wire library
const int SDA_PIN = 20;
const int SCL_PIN = 21;

/* MOLE LOADED PIN */
const int MOLE_LOADED_PIN = 24;
const int MOLE_LOADED_LED_PIN =  28;

/* MOTOR PINS */
const int LEFT_MTR_ENA_PIN = 4;
const int LEFT_MTR_IN1_PIN = 10;
const int LEFT_MTR_IN2_PIN = 11;
const int LEFT_MTR_ENC_PIN = 19;

const int RIGHT_MTR_ENA_PIN = 5;
const int RIGHT_MTR_IN1_PIN = 12;
const int RIGHT_MTR_IN2_PIN = 13;
const int RIGHT_MTR_ENC_PIN = 18;

const int OUTPUT_PIN_LS[15] = {DISPENSOR_ACTUATOR_pin1, DISPENSOR_ACTUATOR_pin2, DISPENSOR_ACTUATOR_pin3, DISPENSOR_ACTUATOR_pin4, R_LED_PIN, Y_LED_PIN, B_LED_PIN, MOLE_LOADED_LED_PIN, GAME_MODE_LED_PIN, LEFT_MTR_ENA_PIN, LEFT_MTR_IN1_PIN, LEFT_MTR_IN2_PIN, RIGHT_MTR_ENA_PIN, RIGHT_MTR_IN1_PIN, RIGHT_MTR_IN2_PIN}; 

/* GLOBAL CONSTANTS */
const signed long STEPS = -64;
int DISP_STEPS = 0;
int DISP_DIR = 0;
const unsigned long ENCODER_READ_TIME = 10;

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
int WHCK = 0;
int TURN = 0;

// FSM Outputs
int LMTR = 0;
int RMTR = 0;
int PLCE = 0;
int STBY = 0;

int state = 100; // State variable

/* Sensor variables */
byte red_var;
byte green_var;
byte blue_var;
int ir_reading;
int lux;

/* Color selection variables */
int red_reading = 0;
int blue_reading = 0;
int yellow_reading = 0;
int last_red_reading = 0;
int last_blue_reading = 0;
int last_yellow_reading = 0;

/* Timing variables */
bool t_started = false;
unsigned long tstart;
unsigned long max_time = 120000;
unsigned long time_old;

/* Start variables */
unsigned long debounceDelay = 50; // ms
int srt_reading;
int last_srt_reading = LOW;
int LEDState = 0;

/* Driving Variables */
unsigned long pulses1;
unsigned long pulses2;
const int num_turns = 7;
unsigned long turnArr[num_turns][4] = {{3, 3, 1, 0}, {0, 3, 0, 0}, {0, 2, 0, 0}, {2, 2, 1, 0}, {2, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}};  // Left is 0, Right is 1
unsigned long currPos[2] = {3, 2};
unsigned long prevPos[2];
int compass = 0;  // 0 - N, 1 - E, 2 - S, 3 - W
// Test these!!
int next_turn = 0;
unsigned long lastSquare[2] = {2, 3};
int start_checker_sai = 0;
unsigned long fwd_dpulses1 = dis2enc(30.48);
unsigned long fwd_dpulses2 = dis2enc(30.48);
const unsigned long left_dpulses1 = 43;  // dis2enc(30.48);
const unsigned long left_dpulses2 = 43;  // dis2enc(30.48);
const unsigned long right_dpulses1 = 43;  // dis2enc(30.48);
const unsigned long right_dpulses2 = 43;  // dis2enc(30.48);
signed long pulses_err;
unsigned long left_fwd_ena = 230;
unsigned long right_fwd_ena = 230;

/* STATE NAMES */
const int S0 = 100;
const int S1 = 101;
const int S2 = 102;
const int S3 = 103;
const int S4 = 104;
const int S5 = 105;
const int S6 = 106;
const int S7 = 107;
