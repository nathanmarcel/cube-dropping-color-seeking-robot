

// INITIALIZING INSTANCES OF THE ENCODER CLASSES

const int LEFT_MTR_ENA_PIN = 4; // output
const int LEFT_MTR_ENC_PIN = 2; // input
const int LEFT_MTR_IN1_PIN = 10;
const int LEFT_MTR_IN2_PIN = 11;

const int RIGHT_MTR_ENA_PIN = 5; // output
const int RIGHT_MTR_ENC_PIN = 3; // input
const int RIGHT_MTR_IN1_PIN = 12; // output
const int RIGHT_MTR_IN2_PIN = 13;

unsigned long right_pulses = 0;
unsigned long left_pulses = 0;

// stop is IN1 & IN2 are LOW
// ccw (backwards probably) IN1 is HIGH IN2 is LOW
// cw IN1 is LOW and IN2 is HIGH

unsigned long time_old;
const unsigned long ENCODER_READ_TIME = 10;



void setup() {
   pinMode(LEFT_MTR_ENA_PIN, OUTPUT); 
   pinMode(LEFT_MTR_ENC_PIN, INPUT); 
   pinMode(LEFT_MTR_IN1_PIN, OUTPUT);
   pinMode(LEFT_MTR_IN2_PIN, OUTPUT);

   
   pinMode(RIGHT_MTR_ENA_PIN, OUTPUT); 
   pinMode(RIGHT_MTR_ENC_PIN, INPUT); 
   pinMode(RIGHT_MTR_IN1_PIN, OUTPUT);
   pinMode(RIGHT_MTR_IN2_PIN, OUTPUT);

   attachInterrupt(digitalPinToInterrupt(LEFT_MTR_ENC_PIN), add_left_pulse, FALLING);
   attachInterrupt(digitalPinToInterrupt(RIGHT_MTR_ENC_PIN), add_right_pulse, FALLING);

   Serial.begin(9600);
}
 
void loop() {
  forward(100);

  Serial.println("Done forward, starting turn left");

  turn_left(100);


  Serial.println("Done turning, stopping motors");
  
  stop_motors(3000);
  
}


void forward(int desired_pulses) {
  
  long initial_left_pulses = left_pulses;
  long initial_right_pulses = right_pulses;

  while (((left_pulses - initial_left_pulses) < desired_pulses) && ((right_pulses - initial_right_pulses) < desired_pulses)) {
    // testing ccw
    digitalWrite(LEFT_MTR_IN1_PIN, HIGH);
    digitalWrite(LEFT_MTR_IN2_PIN, LOW);
    
    digitalWrite(RIGHT_MTR_IN1_PIN, HIGH);
    digitalWrite(RIGHT_MTR_IN2_PIN, LOW);
    
    digitalWrite(LEFT_MTR_ENA_PIN, HIGH);
    digitalWrite(RIGHT_MTR_ENA_PIN, HIGH);
    
  }
}
 
void turn_left(int desired_pulses) {
  
  long initial_left_pulses = left_pulses;
  long initial_right_pulses = right_pulses;

  while ((right_pulses - initial_right_pulses) < desired_pulses) {

    // testing ccw
    digitalWrite(LEFT_MTR_IN1_PIN, LOW);
    digitalWrite(LEFT_MTR_IN2_PIN, HIGH);
   
    digitalWrite(RIGHT_MTR_IN1_PIN, HIGH);
    digitalWrite(RIGHT_MTR_IN2_PIN, LOW);

    digitalWrite(LEFT_MTR_ENA_PIN,HIGH);
    digitalWrite(RIGHT_MTR_ENA_PIN,HIGH);
  }
}
 
void stop_motors(int time_to_stop) {
  int start_time = millis();

  while(millis() - start_time < time_to_stop) {
    digitalWrite(LEFT_MTR_IN1_PIN, LOW);
    digitalWrite(LEFT_MTR_IN2_PIN, LOW);
    
    digitalWrite(RIGHT_MTR_IN1_PIN, LOW);
    digitalWrite(RIGHT_MTR_IN2_PIN, LOW);

    digitalWrite(LEFT_MTR_ENA_PIN,LOW);
    digitalWrite(RIGHT_MTR_ENA_PIN,LOW);
  }
}

void add_left_pulse(){
  left_pulses++;
}
void add_right_pulse(){
  right_pulses++;
}

int motorEncoder(){
  if(millis()-time_old >= ENCODER_READ_TIME){
    detachInterrupt(digitalPinToInterrupt(LEFT_MTR_ENC_PIN));
    detachInterrupt(digitalPinToInterrupt(RIGHT_MTR_ENC_PIN));
    time_old = millis();

    attachInterrupt(digitalPinToInterrupt(LEFT_MTR_ENC_PIN), add_left_pulse, FALLING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_MTR_ENC_PIN), add_right_pulse, FALLING);
  }
}
