/* CODE FOR INTERIM FSM PRESENTATION */

/* INPUT TERNIMALS */
const int START_BUTTON_pin = 1;
const int DESIRED_COLOR_pin = 2;
const int DETECTED_COLOR_pin = 3;
const int LOAD_DISTANCE_pin = A4;

/* OUTPUT TERMINALS */
const int LEFT_MOTOR_pin = 10;
const int RIGHT_MOTOR_pin = 11;
const int DISPENSOR_ACTUATOR_pin = 12;
const int LED_DISPLAY_DI_pin = 9;
const int LED_DISPLAY_CI_pin = 13;

void setup() {
  pinMode(LEFT_MOTOR_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_pin, OUTPUT);
  pinMode(DISPENSOR_ACTUATOR_pin, OUTPUT);
  pinMode(LED_DISPLAY_DI_pin, OUTPUT);
  pinMode(LED_DISPLAY_CI_pin, OUTPUT);
}

void loop() {
}
