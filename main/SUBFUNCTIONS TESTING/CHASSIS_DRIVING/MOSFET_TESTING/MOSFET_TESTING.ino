//MOSFET OUTPUT
const int ARDUINO_PWM_OUTPUT_1 = 3;
const int ARDUINO_PWM_OUTPUT_2 = 5;
unsigned long lastMilli = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ARDUINO_PWM_OUTPUT_1, OUTPUT);
  pinMode(ARDUINO_PWM_OUTPUT_2, OUTPUT);
}  

void loop() {
  // put your main code here, to run repeatedly:
  straight();
  left_turn();
  straight();
  left_turn();
  analogWrite(ARDUINO_PWM_OUTPUT_1, 0);
  analogWrite(ARDUINO_PWM_OUTPUT_2, 0);
  delay(500);
}

void straight() {
  analogWrite(ARDUINO_PWM_OUTPUT_1, 255);
  analogWrite(ARDUINO_PWM_OUTPUT_2, 255);
  delay(2000);
}

void left_turn() {
  analogWrite(ARDUINO_PWM_OUTPUT_1, 50);
  analogWrite(ARDUINO_PWM_OUTPUT_2, 255);
  delay(2000);
}
