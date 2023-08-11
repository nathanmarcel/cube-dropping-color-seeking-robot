// LIBRARIES USED
#include <QuadratureEncoder.h>

// INPUT ENCODER PINS
const int motor_1_en_1 = A2;
const int motor_1_en_2 = A3;
const int motor_2_en_1 = A5;
const int motor_2_en_2 = A4;
const int left_motor_pwm = 11; // change to d3
const int right_motor_pwm = 10;// change to d5

// INITIALIZING INSTANCES OF THE ENCODER CLASSES
Encoders leftEncoder(motor_1_en_1, motor_1_en_2);
Encoders rightEncoder(motor_2_en_1, motor_2_en_2);

unsigned long lastMilli = 0; /* For counting */
int refresh_time = 50;       /* Re-freshes encoder count every 50 ms */
long initial_left_encoder_pos = 0;
long initial_right_encoder_pos = 0;

// CODE
void setup() {
  Serial.begin(9600);
  pinMode(left_motor_pwm, OUTPUT);
  pinMode(right_motor_pwm, OUTPUT);
}

void loop() {

  //forward(3000, 240, 240);
  left_turn(3000, 240);
  //right_turn(3000, 240);
  Serial.println("finished");
  delay(1000);

}
// THIS FUNCTION MAKES BOTH ENCODERS TRAVEL FOR THE SAME DISTANCE
void forward(long desired_en, int left_duty_cycle, int right_duty_cycle) {
  initial_left_encoder_pos = leftEncoder.getEncoderCount();
  initial_right_encoder_pos = rightEncoder.getEncoderCount();
  
  long current_Encoder_count_L = initial_left_encoder_pos;
  long current_Encoder_count_R = initial_right_encoder_pos;

  while (((current_Encoder_count_R - initial_right_encoder_pos) < desired_en) && ((current_Encoder_count_L - initial_left_encoder_pos) < desired_en)) {
    // ADD INSTRUCTIONS TO EITHER DONT CUT POWER OFF, OR TURN TO TURN MOTOR POWER ON INSTEAD
    // USE A MOSFET
    analogWrite(left_motor_pwm, left_duty_cycle);
    analogWrite(right_motor_pwm, right_duty_cycle);
    current_Encoder_count_L = leftEncoder.getEncoderCount();
    current_Encoder_count_R = rightEncoder.getEncoderCount();


    Serial.print(initial_left_encoder_pos);
    Serial.print(" , ");
    Serial.print(current_Encoder_count_L);
    Serial.print("   ");
    Serial.print(initial_right_encoder_pos);
    Serial.print(" , ");
    Serial.println(current_Encoder_count_R);  

    delay(refresh_time);
  }
}

void left_turn(long desired_en, int right_duty_cycle) {
  //initial_left_encoder_pos = leftEncoder.getEncoderCount();
  initial_right_encoder_pos = rightEncoder.getEncoderCount();
  //long current_Encoder_count_L = initial_left_encoder_pos;
  long current_Encoder_count_R = initial_right_encoder_pos;

  while ((current_Encoder_count_R - initial_right_encoder_pos) < desired_en) {
    //analogWrite(left_motor_pwm, left_duty_cycle);
    analogWrite(right_motor_pwm, right_duty_cycle);
    //current_Encoder_count_L = leftEncoder.getEncoderCount();
    current_Encoder_count_R = rightEncoder.getEncoderCount();
    Serial.print(initial_right_encoder_pos);
    Serial.print(" , ");
    Serial.println(current_Encoder_count_R);
    delay(refresh_time);
  }
}


void right_turn(long desired_en, int left_duty_cycle) {
  initial_left_encoder_pos = leftEncoder.getEncoderCount();
  //initial_right_encoder_pos = rightEncoder.getEncoderCount();
  long current_Encoder_count_L = initial_left_encoder_pos;
  //long current_Encoder_count_R = initial_right_encoder_pos;
  
  while ((current_Encoder_count_L - initial_left_encoder_pos) < desired_en) {
    analogWrite(left_motor_pwm, left_duty_cycle);
    //analogWrite(right_motor_pwm, right_duty_cycle);
    current_Encoder_count_L = leftEncoder.getEncoderCount();
    //current_Encoder_count_R = rightEncoder.getEncoderCount();

    Serial.print(initial_left_encoder_pos);
    Serial.print(" , ");
    Serial.println(current_Encoder_count_L);
    
    delay(refresh_time);
  }

}
