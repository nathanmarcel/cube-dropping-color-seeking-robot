long int count;
void setup() {
  Serial.begin(9600);
  digitalWrite(6, LOW);
  digitalWrite(5, HIGH);
  analogWrite(A3, 175);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Encoder Count: ");
  count += digitalRead(12);
  Serial.println(count);
  delay(10);
}
