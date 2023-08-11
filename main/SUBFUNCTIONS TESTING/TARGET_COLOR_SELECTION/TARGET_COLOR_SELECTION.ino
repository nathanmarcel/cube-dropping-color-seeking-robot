// PINS USED
int OUT_BLUE_LED = 13;
int OUT_RED_LED = 12;
int OUT_YELLOW_LED = 11;
int OUT_GAME_MODE = 9;
int IN_BUTTON_COLOR_SELECT = 10;
int IN_BUTTON_GAME_MODE = 7;
int OUT_CHOSEN;
int OUT_PREVIOUS;
int count = 0;
int mode = 0;
int color_pin_ls[3] = {OUT_BLUE_LED, OUT_RED_LED, OUT_YELLOW_LED};
int last_srt_reading = LOW;
int SRT = 0;
int LEDState=0;

void setup() {
  pinMode(OUT_BLUE_LED, OUTPUT);
  pinMode(OUT_RED_LED, OUTPUT);
  pinMode(OUT_YELLOW_LED, OUTPUT);
  pinMode(OUT_GAME_MODE, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  game_mode_select();
  color_select();
}

void color_select() {
  // FIGURE OUT WHICH COLOR IS CHOSEN
  delay(100);
  
  // WAIT FOR USER TO HOLD DOWN THE BUTTON SO THAT WE CAN CHOOSE THE TARGET COLOR
  while (digitalRead(IN_BUTTON_COLOR_SELECT)) {
    digitalWrite(color_pin_ls[count % 3], HIGH);
    digitalWrite(OUT_PREVIOUS, LOW);
    OUT_PREVIOUS = color_pin_ls[count % 3];
    count++;
    delay(1000);
    
    // TURN THE LEDs OFF
    for (int i = 0; i < 3; i++) {
      digitalWrite(color_pin_ls[i], LOW);
    }
  }
  OUT_CHOSEN = color_pin_ls[(count - 1) % 3]; // Update the color
  
  // SHOW THE USER WHAT COLOR WAS CHOSEN
  delay(200);
  digitalWrite(OUT_CHOSEN, HIGH);
}

void game_mode_select() {
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
