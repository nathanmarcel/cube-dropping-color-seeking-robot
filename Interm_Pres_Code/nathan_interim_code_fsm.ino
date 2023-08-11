/* CODE FOR INTERIM FSM PRESENTATION */

/* INPUT TERNIMALS */
const int START_BUTTON_pin = 1;
const int DESIRED_COLOR_pin = 2;
const int DETECTED_COLOR_pin = 3;
const int ROBOT_DISTANCE_pin = A4;
const int MOLES_LOADED_pin = 5;

/* OUTPUT TERMINALS */
const int LEFT_MOTOR_pin = 10;
const int RIGHT_MOTOR_pin = 11;
const int DISPENSOR_ACTUATOR_pin = 12;
const int LED_DISPLAY_DI_pin = 9;
const int LED_DISPLAY_CI_pin = 13;

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
int RIGHT = 0;
int MOLE = 0;
int DISP = 0;
// FSM Outputs
int LMTR = 0;
int RMTR = 0;
int PLCE = 0;
int LED = 0;
// State variable
int state = 100;
// Sensor variables
byte red_var;
byte green_var;
byte blue_var;

// Temporary variables
int DCLR;
int CLR;
int inputs = 0;
int choose_input_color = 0;
int right_turn_finished = 0;
int left_turn_finished = 0;
int action_state_4 = 0;

/* STATE NAMES */
const int S0 = 100;
const int S1 = 101;
const int S2 = 102;
const int S3 = 103;
const int S4 = 104;
const int S5 = 105;
const int S6 = 106;
const int S7 = 107;

void setup() {
  Serial.begin(9600);
  //Wait for serial monitor to open
  while(!Serial)
  {
    delay(10);
  }
}

void loop() {
  /* UPDATE INPUTS */
  // Change starting inputs

  if (not choose_input_color) {
    state = S0;
    Serial.println("State 0");
    Serial.print("Left Motor = ");
    Serial.print(LMTR);
    Serial.print(", Right Motor = ");
    Serial.print(RMTR);
    Serial.print(", PLCE = ");
    Serial.print(PLCE);
    Serial.print(", LED = ");
    Serial.println(LED);
  
  
    Serial.println("Input color to detect(1=red, 2=blue, 3=yellow):");
    while(Serial.available() == 0) {
    }
    CLR = Serial.parseInt();
    // 1 = RED, 2 = BLUE, 3 = YELLOW
    if (CLR == 1){
      RED = 1;
      state = S1;
    }
    else if (CLR == 2){
      BLU = 1;
      state = S2;
    }
    else if (CLR == 3){
      YLW = 1;
      state = S3;
    }
    
    SRT = 1;
  }
  choose_input_color = choose_input_color + 1;
  MOLE = 1;
  
  /* FINITE STATE MACHINE */

  switch(state) {

    /* ROBOT IN OFF STATE*/
    case S0:
      Serial.println("State 0 here");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      LED = 0; 

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
      Serial.println("\nState 1");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      LED = 1; 
      Serial.print("Left Motor = ");
      Serial.print(LMTR);
      Serial.print(", Right Motor = ");
      Serial.print(RMTR);
      Serial.print(", PLCE = ");
      Serial.print(PLCE);
      Serial.print(", LED = ");
      Serial.println(LED);

      Serial.println("Designate Red? (1=Y, 0=N)");
      while (Serial.available() == 0) {
      }
      DRED = Serial.parseInt();

      // Decide next state
      if (SRT && DRED && !DBLU && !DYLW && MOLE){
        state = S4;
        
      }
      else{
        state = S1;
      }
    break;

    case S2:
      Serial.println("\nState 2");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      LED = 1;
      Serial.print("Left Motor = ");
      Serial.print(LMTR);
      Serial.print(", Right Motor = ");
      Serial.print(RMTR);
      Serial.print(", PLCE = ");
      Serial.print(PLCE);
      Serial.print(", LED = ");
      Serial.println(LED);

      Serial.println("Designate Blue? (1=Y, 0=N)");
      while (Serial.available() == 0) {
      }
      DBLU = Serial.parseInt();
  

      // Decide next state
      if (SRT && !DRED && DBLU && !DYLW && MOLE){
        state = S4;
      }
      else{
        state = S2;
      }

    break;

    case S3:
      Serial.println("\nState 3");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      LED = 1; 
      Serial.print("Left Motor = ");
      Serial.print(LMTR);
      Serial.print(", Right Motor = ");
      Serial.print(RMTR);
      Serial.print(", PLCE = ");
      Serial.print(PLCE);
      Serial.print(", LED = ");
      Serial.println(LED);
  
      Serial.println("Designate Yellow? (1=Y, 0=N)");
      while (Serial.available() == 0) {
      }
      DYLW = Serial.parseInt();
      
      // Decide next state
      if (SRT && !DRED && !DBLU && DYLW && MOLE){
        state = S4;
      }
      else{
        state = S3;
      }

    break;

    case S4:
      Serial.println("\nState 4");
      // Set outputs
      LMTR = 1;
      RMTR = 1;
      PLCE = 0;
      LED = 0; 
      Serial.print("Left Motor = ");
      Serial.print(LMTR);
      Serial.print(", Right Motor = ");
      Serial.print(RMTR);
      Serial.print(", PLCE = ");
      Serial.print(PLCE);
      Serial.print(", LED = ");
      Serial.println(LED);



      Serial.println("Turn or Dispense? (0=Left, 1=Right, 2 = Dispense)");
      while (Serial.available() == 0) {
      }
      action_state_4 = Serial.parseInt();
  
      if(action_state_4 == 0) {
        LEFT = 1;
      }
      else if(action_state_4 == 1) {
        RIGHT = 1;
      }
      else if (action_state_4 == 2) {
        DISP = 1;
      }
  
      if(LEFT) {
        state = S5;
      }
      else if (RIGHT) {
        state = S6;
      }
      else if (DISP) {
        state = S7;
      }
      else {
        state = S4;
      }
    break;

    case S5:
    Serial.println("\nState 5");
    // Set outputs
    LMTR = 0;
    RMTR = 1;
    PLCE = 0;
    LED = 0; 
    Serial.print("Left Motor = ");
    Serial.print(LMTR);
    Serial.print(", Right Motor = ");
    Serial.print(RMTR);
    Serial.print(", PLCE = ");
    Serial.print(PLCE);
    Serial.print(", LED = ");
    Serial.println(LED);

    Serial.println("Left Turn Complete? (1=Y)");
      while (Serial.available() == 0) {
      }
      left_turn_finished = Serial.parseInt();
      
    if (left_turn_finished){
      state = S4;
      left_turn_finished = 0;
    }
    else{
      state = S5;
    }
    
    break;

    case S6:
    Serial.println("\nState 6");
    // Set outputs
    LMTR = 1;
    RMTR = 0;
    PLCE = 0;
    LED = 0; 
    Serial.print("Left Motor = ");
    Serial.print(LMTR);
    Serial.print(", Right Motor = ");
    Serial.print(RMTR);
    Serial.print(", PLCE = ");
    Serial.print(PLCE);
    Serial.print(", LED = ");
    Serial.println(LED);

    Serial.println("Right Turn Complete? (1=Y)");
    while (Serial.available() == 0) {
      }
    right_turn_finished = Serial.parseInt();
      
    if (right_turn_finished){
      state = S4;
      right_turn_finished = 0;
    }
    else{
      state = S6;
    }
    
    break;

    case S7:
    Serial.println("\nState 7");
    // Set outputs
    LMTR = 0;
    RMTR = 0;
    PLCE = 1;
    LED = 0; 
    Serial.print("Left Motor = ");
    Serial.print(LMTR);
    Serial.print(", Right Motor = ");
    Serial.print(RMTR);
    Serial.print(", PLCE = ");
    Serial.print(PLCE);
    Serial.print(", LED = ");
    Serial.println(LED);

   
    Serial.println("Select Action (0=No More Moles, 1 = Still Dispensing, 2 = Finished, Need to Turn Left, \n3 = Finished, Need to Turn Right, 4 = Finished, Continue Straight");
      while (Serial.available() == 0) {
        }
      int action_state_7 = Serial.parseInt();


    if (action_state_7 = 0){
      state = S0;
    }
    else if (action_state_7 == 1){
      state = S7;
    }
    else if (action_state_7 == 2){
      state = S5;
    }
    else if (action_state_7 == 3){
      state = S6;
    }
    else if (action_state_7 == 4) {
      state = S0;
    }
    else {
      state = S7;
    }

    break;
    
  }
  delay(2000);
}
