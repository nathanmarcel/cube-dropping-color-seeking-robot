/* CODE FOR INTERIM FSM PRESENTATION */

#include <Stepper.h>
#include <SPI.h>
#include "Adafruit_TCS34725.h"
#include "definitions.ino"

/* INTIALIZATION */
Stepper myStepper(STEPS, DISPENSOR_ACTUATOR_pin1, DISPENSOR_ACTUATOR_pin2, DISPENSOR_ACTUATOR_pin3, DISPENSOR_ACTUATOR_pin4);

void setup() {
  /* PINMODE FOR ALL OUTPUT PINS */
  for (int i = 0; i < OUTPUT_PIN_LS.size(); i++) {
    pinMode(OUTPUT_PIN_LS[i], OUTPUT);
  }
  
  /* START SERIAL MONITOR */
  Serial.begin(9600);
  
  /* Wait for serial monitor to open */
  while(!Serial) {
    delay(10);
  }

  /* SET UP OUTPUT PINS */
  pinMode(ena_pin, OUTPUT);
  pinMode(LEFT_MOTOR_pin, OUTPUT);
  pinMode(RIGHT_MOTOR_pin, OUTPUT);
  // pinMode(DISPENSOR_ACTUATOR_pin, OUTPUT);  // Don't think we need this - test
  pinMode(LED_DISPLAY_DI_pin, OUTPUT);
  pinMode(LED_DISPLAY_CI_pin, OUTPUT);

  /* SET UP INPUT PINS */
  pinMode(START_BUTTON_pin, INPUT);
  pinMode(DESIRED_COLOR_pin, INPUT);
  pinMode(DETECTED_COLOR_pin, INPUT);
  pinMode(MOLE_LOADED_pin, INPUT);

  /* SET UP SENSORS */
  myStepper.setSpeed(SPEED);
  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X);
  tcs.begin();
}

void loop() {
  /* TEST FSM */
  // Change starting inputs
//  DRED = 1;
//  MOLE = 1;
//  if (inputs == 1){
//    SRT = 1;
//  }
//  if (inputs == 5){
//    LEFT = 1;
//  }
//  else{
//    LEFT = 0;
//    RGHT = 0;
//  }
//
//  Serial.print("Input detected color (1=red, 2=blue, 3=yellow):");
//  while (Serial.available() <= inputs){
//  if (Serial.available() > 0) {
//    // read the incoming byte:
//    CLR = Serial.read();
//    // 1 = RED, 2 = BLUE, 3 = YELLOW
//  }
//  if (CLR == 1){
//    RED = 1;
//  }
//  else if (CLR == 2){
//    BLU = 1;
//  }
//  else if (CLR == 3){
//    YLW = 1;
//  }
//  }
//
//  inputs = inputs + 1;

  /* UPDATE INPUTS */
  // Start button
  int srt_reading = digitalRead(START_BUTTON_pin);
  if (srt_reading != last_srt_reading){
    delay(debounceDelay);
  }
  if (srt_reading != SRT){
    SRT = srt_reading;
  }
  last_srt_reading = srt_reading;

  // Analog IR sensor
  ir_reading = analogRead(MOLE_LOADED_pin);

  // Color sensor
  lux = tcs.getRGB(&red_var, &green_var, &blue_var);

  /* PROCESS INPUTS */
  // Determine game mode
  if (SRT and not t_started) {
    start = millis();
    t_started = true;
    Serial.println(start);
  }
  if (SRT and t_started and (millis()-start >= 120000)){
    Serial.print("Amount of time that has passed: ");
    Serial.println(millis()-start);
    SRT = 0;
    t_started = false;
  }

  // Determine if the reading indicates that moles are loaded
  if (ir_reading < MOLE_DIST){
    MOLE = 1;
  }
  else{
    MOLE = 0;
  }

  /* FINITE STATE MACHINE */

  switch(state) {

    /* ROBOT IN OFF STATE*/
    case S0:
      Serial.println("State 0");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      LED = 0; 

      // Decide next state
      if (not SRT and DRED and not DBLU and not DYLW){
        state = S1;
      }
      else if (not SRT and not DRED and DBLU and not DYLW){
        state = S2;
      }
      else if (not SRT and not DRED and not DBLU and DYLW){
        state = S3;
      }
      else{
        state = S0;
      }
    
    break;

    case S1:
      Serial.println("State 1");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      LED = 1; 

      // Decide next state
      if (SRT and DRED and not DBLU and not DYLW and MOLE){
        state = S4;
      }
      else{
        state = S1;
      }
    break;

    case S2:
      Serial.println("State 2");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      LED = 1; 

      // Decide next state
      if (SRT and not DRED and DBLU and not DYLW and MOLE){
        state = S4;
      }
      else{
        state = S2;
      }

    break;

    case S3:
      Serial.println("State 3");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      LED = 1; 

      // Decide next state
      if (SRT and not DRED and not DBLU and DYLW and MOLE){
        state = S4;
      }
      else{
        state = S3;
      }

    break;

    case S4:
    Serial.println("State 4");
    // Set outputs
    LMTR = 1;
    RMTR = 1;
    PLCE = 0;
    LED = 0; 

    // Decide next state
    if (SRT and ECDR and LEFT and not RGHT){
      state = S5;
    }
    else if (SRT and ECDR and not LEFT and RGHT){
      state = S6;
    }
    else if ((SRT and DRED and not DBLU and not DYLW and RED and not BLU and not YLW and MOLE) or (SRT and not DRED and DBLU and not DYLW and not RED and BLU and not YLW and MOLE) or (SRT and not DRED and not DBLU and DYLW and not RED and not BLU and YLW and MOLE)){
      state = S7;
    }
    else if (SRT and not LEFT and not RGHT and not ECDR){
      state = S4;
    }
    else{
      state = S0;
    }
    
    break;

    case S5:
    Serial.println("State 5");
    // Set outputs
    LMTR = 0;
    RMTR = 1;
    PLCE = 0;
    LED = 0; 

    // Decide next state
    if (SRT and LEFT and not RGHT and not ECDR){
      state = S5;
    }
    else{
      state = S4;
    }
    
    break;

    case S6:
    Serial.println("State 6");
    // Set outputs
    LMTR = 1;
    RMTR = 0;
    PLCE = 0;
    LED = 0; 

    // Decide next state
    if (SRT and not LEFT and RGHT and not ECDR){
      state = S6;
    }
    else{
      state = S4;
    }
    
    break;

    case S7:
    Serial.println("State 7");
    // Set outputs
    LMTR = 0;
    RMTR = 0;
    PLCE = 1;
    LED = 0; 

    // Decide next state
    if (SRT and not MOLE){
      state = S7;
    }
    else if (SRT and not LEFT and not RGHT and not ECDR){
      state = S4;
    }
    else if (SRT and LEFT and not RGHT and not ECDR){
      state = S5;
    }
    else if (SRT and not LEFT and RGHT and not ECDR){
      state = S6;
    }
    else{
      state = S0;
    }

    break;
    
  }
  
  /* IMPLEMENT OUPUTS */
  // Drive Motors
  digitalWrite(LEFT_MOTOR_pin, LEFT);
  digitalWrite(RIGHT_MOTOR_pin, RGHT);
  analogWrite(ena_pin, 255);  // Always drive motor full | TODO: Implement PID controller and/or speed control
  
  // Dispense Mole
  if (PLCE){
    myStepper.step(STEPS);
    delay(PLACE_DELAY);
  }

  // Change LED Colors
  
}

int game_mode_select() {
  int srt_reading = digitalRead(IN_BUTTON_GAME_MODE);
  if( srt_reading==1 && last_srt_reading==0){
    if(LEDState==0){
      digitalWrite(OUT_GAME_MODE,HIGH);
      LEDState=1;
    }
    else{
      digitalWrite(OUT_GAME_MODE,LOW);
      LEDState=0;
    }
  }
  last_srt_reading=srt_reading;
  delay(100);
  
}
