#include <Stepper.h>

const int DISPENSOR_ACTUATOR_pin1 = 22;
const int DISPENSOR_ACTUATOR_pin2 = 24;
const int DISPENSOR_ACTUATOR_pin3 = 26;
const int DISPENSOR_ACTUATOR_pin4 = 28;
const unsigned long STEPS = 32;
const int SPEED = 60;
const unsigned long PLACE_DELAY = 1000;

/* INTIALIZATION */
Stepper myStepper(STEPS, DISPENSOR_ACTUATOR_pin1, DISPENSOR_ACTUATOR_pin2, DISPENSOR_ACTUATOR_pin3, DISPENSOR_ACTUATOR_pin4);

void setup() {
  // put your setup code here, to run once:
  myStepper.setSpeed(SPEED);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool PLCE = HIGH;
  if (PLCE){
    myStepper.step(STEPS);
    delay(PLACE_DELAY);
    PLCE = LOW;
  }
}
