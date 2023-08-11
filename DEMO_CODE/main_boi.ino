/* THE BRAINS OF THE OPERATION */
void setup() {
  /* PINMODE FOR ALL OUTPUT PINS */
  for (int i = 0; i < 15; i++) {
    pinMode(OUTPUT_PIN_LS[i], OUTPUT);
  }
  
  /* START SERIAL MONITOR */
  Serial.begin(9600);

  /* SET UP SENSORS */
  attachInterrupt(digitalPinToInterrupt(LEFT_MTR_ENC_PIN), count1, FALLING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_MTR_ENC_PIN), count2, FALLING);
  tcs.begin();

  /* SET UP TURNING ARRAY */
  for (int i=0; i<num_turns; i++){
    if (i==0) {
      long x_dist = currPos[0] - turnArr[i][0];
      long y_dist = currPos[1] - turnArr[i][1];
      x_dist = abs(x_dist);
      y_dist = abs(y_dist);
      turnArr[i][3] = (x_dist + y_dist) * dis2enc(30.48);
    }
    else {
      long x_dist = abs(turnArr[i][0] - turnArr[i-1][0]);
      long y_dist = abs(turnArr[i][1] - turnArr[i-1][1]);
      x_dist = abs(x_dist);
      y_dist = abs(y_dist);
      turnArr[i][3] = (x_dist + y_dist) * dis2enc(30.48);
    }
  }
}

void loop() {
  /* UPDATE INPUTS */
  // Start button
  SRT = game_mode_select();
//  Serial.print("Input varaibles: ");
//  Serial.print(SRT);
//  Serial.print(DRED);
//  Serial.print(DBLU);
//  Serial.print(DYLW);
//  Serial.print(RED);
//  Serial.print(BLU);
//  Serial.print(YLW);
//  Serial.print(ECDR);
//  Serial.print(LEFT);
//  Serial.print(RGHT);
//  Serial.print(MOLE);
//  Serial.print(WHCK);
//  Serial.print(TURN);
//  Serial.print("\tOutput variables: ");
//  Serial.print(LMTR);
//  Serial.print(RMTR);
//  Serial.print(PLCE);
//  Serial.println(STBY);
//  Serial.print("State variable: ");
//  Serial.println(state);

  // Color selection buttons
  colorSelection();

  // Analog IR sensor
  MOLE = checkForMole();

  // Color sensor
  readColor();

  // Get the current movement progress
  checkPos();

  /* FINITE STATE MACHINE */

  switch(state) {

    // ROBOT IN OFF STATE
    case S0:
      Serial.println("State 0");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      STBY = 0; 

      // Decide next state
      if (not SRT and (DRED or DBLU or DYLW)){
        state = S1;
      }
      else{
        state = S0;
      }
    
    break;

    // ROBOT IN COLOR CHOOSING MODE
    case S1:
      Serial.println("State 1");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      STBY = 0; 

      // Decide next state
      if (SRT and (DRED or DBLU or DYLW)){
        state = S2;
      }
      else{
        state = S1;
      }
    break;

    // ROBOT IN STANDBY MODE
    case S2:
      Serial.println("State 2");
      // Set outputs
      LMTR = 0;
      RMTR = 0;
      PLCE = 0;
      STBY = 1; 

      // Decide next state
      if (not SRT){
        state = S0;
      }
      else if (SRT and not ECDR and not LEFT and not RGHT) {
        state = S3;
      }
      else if (SRT and ((DRED and RED) or (DBLU and BLU) or (DYLW and YLW)) and not WHCK) {
        state = S6;
      }
      else if (SRT and LEFT and not RGHT and not TURN) {
        state = S4;
      }
      else if (SRT and not LEFT and RGHT and not TURN) {
        state = S5;
      }
      else{
        state = S2;
      }

    break;

    // ROBOT IN FORWARD DRIVING MODE
    case S3:
    Serial.println("State 3");
    // Set outputs
    LMTR = 1;
    RMTR = 1;
    PLCE = 0;
    STBY = 0; 

    // Decide next state
    if (SRT and not ECDR and not LEFT and not RGHT){
      state = S3;
    }
    else{
      state = S2;
    }

    break;

    // ROBOT IN LEFT TURN
    case S4:
    Serial.println("State 4");
    // Set outputs
    LMTR = 0;
    RMTR = 1;
    PLCE = 0;
    STBY = 0; 

    // Decide next state
//    if (SRT and not ECDR and LEFT and not RGHT and not TURN){
//      state = S4;
//    }
//    else{
      state = S2;
    //}
    
    break;

    // ROBOT IN RIGHT TURN
    case S5:
    Serial.println("State 5");
    // Set outputs
    LMTR = 1;
    RMTR = 0;
    PLCE = 0;
    STBY = 0; 

    // Decide next state
//    if (SRT and not ECDR and not LEFT and RGHT and not TURN){
//      state = S5;
//    }
//    else{
      state = S2;
//    }
    
    break;

    // ROBOT IN PLACING MODE
    case S6:
    Serial.println("State 6");
    // Set outputs
    LMTR = 0;
    RMTR = 0;
    PLCE = 1;
    STBY = 0; 

    // Decide next state
//    if (SRT and ((DRED and RED) or (DBLU and BLU) or (DYLW and YLW)) and MOLE and not WHCK){
//      state = S6;
//    }
//    else{
//      state = S2;
//    }
    state = S2;
    
    break;
  }
  
  /* IMPLEMENT OUPUTS */
  // Drive Motors
  if (LMTR and RMTR) {
    forward();
    WHCK = 0;
    TURN = 0;
  }
  else if (LMTR and not RMTR) {
    //delay(360);
    while ((pulses1 < left_dpulses1) and (pulses2 < left_dpulses2)) {
      turn_left();
      motorEncoder();
    }
    TURN = 1;
  }
  else if (not LMTR and RMTR) {
    while ((pulses1 < right_dpulses1) and (pulses2 < right_dpulses2)) {
      turn_right();
      motorEncoder();
    }
    //delay(360);
    TURN = 1;
  }
  else {
    stop_motors();
  }

  // Place Mole
  if (PLCE) {
    dispense(STEPS);
    WHCK = 1;
  }

  // Reset on Standby
  if (STBY) {
    ECDR = 0;
//    if (!LEFT and !RGHT) {
//      pulses_err = (signed long) pulses1 - (signed long) pulses2;
//      if (pulses1 > pulses2) {
//        fwd_dpulses2 += abs(pulses_err);
//      }
//      else if (pulses2 > pulses1) {
//        fwd_dpulses1 += abs(pulses_err);
//      }
//      Serial.print("Forward pulses: ");
//      Serial.print(fwd_dpulses1);
//      Serial.print("\t");
//      Serial.println(fwd_dpulses2);
//    }
    Serial.print("Left / Right Pulses: ");
    Serial.print(pulses1);
    Serial.print("\t");
    Serial.println(pulses2);
    pulses1 = 0;
    pulses2 = 0;
  }

  // Check the map for the current position and next movement
  checkMap();
  if (SRT and (prevPos[0]==lastSquare[0]) and (prevPos[1]==lastSquare[1])) {
    start_checker_sai = 1;
  }
  if (SRT and (currPos[0]==lastSquare[0]) and (currPos[1]==lastSquare[1]) and (start_checker_sai==1)){
    start_checker_sai = 2;
  }
  if (start_checker_sai==2) {
    SRT = 0;
  }

  // Display LEDs
  ledStates();
  
}
