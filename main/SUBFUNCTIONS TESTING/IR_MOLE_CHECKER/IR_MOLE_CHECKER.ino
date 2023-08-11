const int IR_pin = 7;
int ir_reading;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IR_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ir_reading = digitalRead(IR_pin);
  Serial.print("Mole Loaded: ");
  Serial.println(!ir_reading);
}
