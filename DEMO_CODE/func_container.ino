/* THIS IS ACTUALLY THE FUNCTION CONTAINER */
#include <Wire.h>
#include "Adafruit_TCS34725.h"


#define THRESHHOLD 0
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X);

/* PREPROCESSING FUCNTIONS | DETERMINES FSM INPUTS */
int game_mode_select() {
  srt_reading = digitalRead(GAME_MODE_BUTTON_PIN);
  if(!t_started && srt_reading==1 && last_srt_reading==0){
    Serial.println("Game Mode Started");
    SRT = 1;
    t_started = true;
    LEDState=1;
    tstart = millis();
  }
  else if (t_started && srt_reading==1 && last_srt_reading==0){
    Serial.println("Game Mode Ended");
    t_started = false;
    SRT = 0;
    LEDState=0;
  }
  if (SRT and t_started and (millis()-tstart >= max_time)){
    Serial.print("Amount of time that has passed: ");
    Serial.println(millis()-tstart);
    Serial.println("Game Mode Ended");
    SRT = 0;
    t_started = false;
    LEDState=0;
  }
  last_srt_reading=srt_reading;
  // digitalWrite(GAME_MODE_LED_PIN, LEDState);
  // delay(20);
  return SRT;
}

int checkForMole(){
  ir_reading = digitalRead(MOLE_LOADED_PIN);
  return !ir_reading;
}

void count1(){
  pulses1++;
}
void count2(){
  pulses2++;
}

unsigned long motorEncoder(){
  detachInterrupt(digitalPinToInterrupt(LEFT_MTR_ENC_PIN));
  detachInterrupt(digitalPinToInterrupt(RIGHT_MTR_ENC_PIN));
//  Serial.print("Left Motor: ");
//  Serial.print(pulses1);
//  Serial.print("\tRight Motor: ");
//  Serial.println(pulses2);
  attachInterrupt(digitalPinToInterrupt(LEFT_MTR_ENC_PIN), count1, FALLING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_MTR_ENC_PIN), count2, FALLING);
}

void readColor() {
  lux = tcs.getRGB(&red_var, &green_var, &blue_var);
//  Serial.print("\tR:\t"); Serial.print(red_var);
//  Serial.print("\tG:\t"); Serial.print(green_var);
//  Serial.print("\tB:\t"); Serial.println(blue_var); 
  // Red Square
  if (red_var >= 110 && green_var <= 5 && blue_var <= 5) {
    // Serial.println("Detected Red Square");
    // 119, 3, 1
    // 119, 3, 1
    RED = 1;
    BLU = 0;
    YLW = 0;
  }
  else if (red_var <= 25 && green_var <= 20 && blue_var <= 20) {
    // Serial.println("Detected Blue Square");
    // 20, 14, 15
    // 19, 14, 16
    RED = 0;
    BLU = 1;
    YLW = 0;
  }
  else if (red_var <= 45 && green_var <= 20 && blue_var <= 5) {
    // Serial.println("Detected Yellow Square");
    // 42, 18, 1
    // 42, 18, 1
    RED = 0;
    BLU = 0;
    YLW = 1;
  }
}

void colorSelection() {
  if (!SRT) {
    red_reading = digitalRead(RED_SELEC_BUTTON_PIN);
    blue_reading = digitalRead(BLUE_SELEC_BUTTON_PIN);
    yellow_reading = digitalRead(YELLOW_SELEC_BUTTON_PIN);

    if (red_reading == 1 and last_red_reading == 0) {
      DRED = 1;
      DBLU = 0;
      DYLW = 0;
    }
    if (blue_reading == 1 and last_blue_reading == 0) {
      DRED = 0;
      DBLU = 1;
      DYLW = 0;
    }
    if (yellow_reading == 1 and last_yellow_reading == 0) {
      DRED = 0;
      DBLU = 0;
      DYLW = 1;
    }

    last_red_reading = red_reading;
    last_blue_reading = blue_reading;
    last_yellow_reading = yellow_reading;
  }
}

void getBoardPos() {
  // Set the Previous Position
  prevPos[0] = currPos[0];
  prevPos[1] = currPos[1];
  // Change the map if the robot has gone forward
  if (LMTR and RMTR){
    if (compass == 0){
      currPos[1]++;
    }
    else if (compass == 1) {
      currPos[0]++;
    }
    else if (compass == 2) {
      currPos[1]--;
    }
    else if (compass == 3) {
      currPos[0]--;
    }
  }
  Serial.print("Current Board Position: ");
  Serial.print(currPos[0]);
  Serial.print("\t");
  Serial.println(currPos[1]);
  Serial.print("Previous Board Position: ");
  Serial.print(prevPos[0]);
  Serial.print("\t");
  Serial.println(prevPos[1]);
  if (LEFT) {  // not LMTR and RMTR
    if (compass == 0) {
      compass = 3;
    }
    else {
      compass--;
    }
  }
  if (RGHT) {  // LMTR and not RMTR
    if (compass == 3) {
      compass = 0;
    }
    else {
      compass++;
    }
  }
  Serial.print("Current Board Heading: ");
  Serial.println(compass);
}

void checkPos() {
  motorEncoder();
  // We are still in movement
  if (SRT && !ECDR && !LEFT && !RGHT && !TURN) {
    // We are still actively going forward
    // motorEncoder();
    if ((pulses1>=fwd_dpulses1) && (pulses2>=fwd_dpulses2)){
      ECDR = 1;
    }
    else {
      ECDR = 0;
    }
  }
//  else if (SRT && !ECDR && LEFT && !RGHT) {
//    // We are still actively turning left
//    // motorEncoder();
//    if (pulses1>=left_dpulses1 && pulses2>=left_dpulses2){
//      TURN = 1;
//    }
//    else {
//      TURN = 0;
//    }
//  }
//  else if (SRT && !ECDR && !LEFT && RGHT) {
//    // We are still actively turning right
//    // motorEncoder;
//    if (pulses1>=right_dpulses1 && pulses2>=right_dpulses2){
//      TURN = 1;
//    }
//    else {
//      TURN = 0;
//    }
//  }
}

void checkMap() {
  // If we have completed the previous movement, determine the next movement
  if (SRT && TURN && (LEFT ^ RGHT)) {
    // We just finished turning, we will always go forward
    Serial.println("Just finished turning!");
    getBoardPos();
    LEFT = 0;
    RGHT = 0;
    next_turn++;
  }
  else if (SRT && ECDR && !LEFT && !RGHT) {
    // We just finished going forward, check to see if we are on a turn square
    getBoardPos();
    for (int i=0; i<num_turns; i++){
//      Serial.print("Turn Array: ");
//      Serial.print(turnArr[i][0]);
//      Serial.print(" ");
//      Serial.println(turnArr[i][1]);
      if (turnArr[i][0]==currPos[0] && turnArr[i][1]==currPos[1]) {
        // We need to turn
        if (turnArr[i][2]==0) {
          // Turn left
          Serial.println("TURNING LEFT!");
          LEFT = 1;
          RGHT = 0;
          break;   
        }
        else {
          // Turn right
          Serial.println("TURNING RIGHT!");
          LEFT = 0;
          RGHT = 1;
          break;
        }
      }
    }
  }
}


/* POSTPROCESSING FUNCTIONS | USES FSM OUTPUTS */
void forward() {
  
//  long initial_left_pulses = pulses1;  // left_pulses;
//  long initial_right_pulses = pulses2;  // right_pulses;

  // while (((pulses1 - initial_left_pulses) < desired_pulses) && ((pulses2 - initial_right_pulses) < desired_pulses)) {
    // testing ccw
    // motorEncoder();
    
    digitalWrite(LEFT_MTR_IN1_PIN, HIGH);
    digitalWrite(LEFT_MTR_IN2_PIN, LOW);
    
    digitalWrite(RIGHT_MTR_IN1_PIN, HIGH);
    digitalWrite(RIGHT_MTR_IN2_PIN, LOW);
    
    analogWrite(LEFT_MTR_ENA_PIN, left_fwd_ena);
    analogWrite(RIGHT_MTR_ENA_PIN, right_fwd_ena);
    
  // }
}
 
void turn_right() {
  
//  long initial_left_pulses = pulses1;  // left_pulses;
//  long initial_right_pulses = pulses2;  // right_pulses;

  // while ((pulses2 - initial_right_pulses) < desired_pulses) {
    motorEncoder();

    // testing ccw
    digitalWrite(LEFT_MTR_IN1_PIN, LOW);
    digitalWrite(LEFT_MTR_IN2_PIN, HIGH);
   
    digitalWrite(RIGHT_MTR_IN1_PIN, HIGH);
    digitalWrite(RIGHT_MTR_IN2_PIN, LOW);

    analogWrite(LEFT_MTR_ENA_PIN, left_right_ena);//speed fr 
    analogWrite(RIGHT_MTR_ENA_PIN, right_right_ena);//speed fr 

  // }
}

void turn_left() {
  
//  long initial_left_pulses = pulses1;  // left_pulses;
//  long initial_right_pulses = pulses2;  // right_pulses;

  // while ((pulses1 - initial_left_pulses) < desired_pulses) {

    // testing ccw
    digitalWrite(LEFT_MTR_IN1_PIN, HIGH);
    digitalWrite(LEFT_MTR_IN2_PIN, LOW);
   
    digitalWrite(RIGHT_MTR_IN1_PIN, LOW);
    digitalWrite(RIGHT_MTR_IN2_PIN, HIGH);

    analogWrite(LEFT_MTR_ENA_PIN, left_left_ena);
    analogWrite(RIGHT_MTR_ENA_PIN, right_left_ena);

  // }
}

void stop_motors() {
  digitalWrite(LEFT_MTR_IN1_PIN, LOW);
  digitalWrite(LEFT_MTR_IN2_PIN, LOW);
  
  digitalWrite(RIGHT_MTR_IN1_PIN, LOW);
  digitalWrite(RIGHT_MTR_IN2_PIN, LOW);

  // analogWrite(LEFT_MTR_ENA_PIN, 0);
  // analogWrite(RIGHT_MTR_ENA_PIN, 0);
}

void dispense(double nbStep) {
  if (nbStep >= 0) {
    DISP_DIR = 1;
  }
  else {
    DISP_DIR = 0;
    nbStep=-nbStep;
  }

  for (int i=0; i < nbStep * 8; i++) {
    switch(DISP_STEPS) {
      case 0:
      digitalWrite(DISPENSOR_ACTUATOR_pin1, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin2, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin3, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin4, HIGH);
      break;
      
      case 1:
      digitalWrite(DISPENSOR_ACTUATOR_pin1, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin2, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin3, HIGH);
      digitalWrite(DISPENSOR_ACTUATOR_pin4, HIGH);
      break;
      
      case 2:
      digitalWrite(DISPENSOR_ACTUATOR_pin1, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin2, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin3, HIGH);
      digitalWrite(DISPENSOR_ACTUATOR_pin4, LOW);
      break;

      case 3:
      digitalWrite(DISPENSOR_ACTUATOR_pin1, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin2, HIGH);
      digitalWrite(DISPENSOR_ACTUATOR_pin3, HIGH);
      digitalWrite(DISPENSOR_ACTUATOR_pin4, LOW);
      break;

      case 4:
      digitalWrite(DISPENSOR_ACTUATOR_pin1, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin2, HIGH);
      digitalWrite(DISPENSOR_ACTUATOR_pin3, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin4, LOW);
      break;

      case 5:
      digitalWrite(DISPENSOR_ACTUATOR_pin1, HIGH);
      digitalWrite(DISPENSOR_ACTUATOR_pin2, HIGH);
      digitalWrite(DISPENSOR_ACTUATOR_pin3, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin4, LOW);
      break;

      case 6:
      digitalWrite(DISPENSOR_ACTUATOR_pin1, HIGH);
      digitalWrite(DISPENSOR_ACTUATOR_pin2, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin3, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin4, LOW);
      break;

      case 7:
      digitalWrite(DISPENSOR_ACTUATOR_pin1, HIGH);
      digitalWrite(DISPENSOR_ACTUATOR_pin2, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin3, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin4, HIGH);
      break;

      default:
      digitalWrite(DISPENSOR_ACTUATOR_pin1, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin2, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin3, LOW);
      digitalWrite(DISPENSOR_ACTUATOR_pin4, LOW);
      break;
    }

    delayMicroseconds(1000);
    if (DISP_DIR == 1) {
      DISP_STEPS++;
    }

    if (DISP_DIR == 0) {
      DISP_STEPS--;
    }

    if (DISP_STEPS > 7) {
      DISP_STEPS = 0;
    }

    if (DISP_STEPS < 0) {
      DISP_STEPS = 7;
    }
    
  }
}

void ledStates() {
  digitalWrite(R_LED_PIN, DRED);
  digitalWrite(B_LED_PIN, DBLU);
  digitalWrite(Y_LED_PIN, DYLW);
  digitalWrite(MOLE_LOADED_LED_PIN, MOLE);
  digitalWrite(GAME_MODE_LED_PIN, SRT);
}
