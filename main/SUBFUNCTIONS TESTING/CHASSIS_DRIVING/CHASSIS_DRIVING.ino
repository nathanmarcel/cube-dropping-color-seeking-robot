#include "SparkFun_TB6612.h"
#include "TimerOne.h"

unsigned int counter=0;
unsigned int counter2=1;

// Set up pins
#define AIN1 6
#define BIN1 4
#define AIN2 5
#define BIN2 3
#define PWMA 7
#define PWMB 2
#define STBY 4

// Set up motors
const int offsetA = 1;
const int offsetB = 1;
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

//Speed Encoder Block
void docount()  // counts from the speed sensor
{
  counter++;  // increase +1 the counter value
} 

void docount2()  // counts from the speed sensor
{
  counter2++;  // increase +1 the counter value
}

void timerIsr()
{
  Timer1.detachInterrupt();  //stop the timer
  Serial.print("Motor1 Speed: "); 
  int rotation = (counter / 20);  // divide by number of holes in Disc
  Serial.print(rotation,DEC);  
  Serial.println(" Rotation per seconds"); 
  counter=0;  //  reset counter to zero
  Serial.print("Motor2 Speed: "); 
  int rotation2 = (counter2 / 20);  // divide by number of holes in Disc
  Serial.print(rotation2,DEC);  
  Serial.println(" Rotation per seconds"); 
  counter2=0;  //  reset counter to zero
  Timer1.attachInterrupt( timerIsr );  //enable the timer
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Timer1.initialize(1000000); // set timer for 1sec
  attachInterrupt(12, docount, RISING);  // increase counter when speed sensor pin goes High
  attachInterrupt(12, docount2, RISING);  // increase counter when speed sensor pin goes High
  Timer1.attachInterrupt( timerIsr ); // enable the timer
}

void loop() {
  // put your main code here, to run repeatedly:
  // Drive motors forward at max speed
  forward(motor1, motor2, 255);
  delay(1000);
  left(motor1, motor2, 100);
  delay(1000);
  right(motor1, motor2, 100);
  delay(1000);
  brake(motor1, motor2);
  delay(1000);
}
