/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
//Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
 Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 1);

int HOWMANY = 4;
int stepsForTenResistors = 150; // was 150 before 2024
int stepsForFourResistors = 40;
int microBackwardPin = 4;
int advancePin = 6;
int microForwardPin = 7;

int STATE = 0;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(85);  // 10 rpm  

   pinMode(advancePin, INPUT);
   pinMode(microForwardPin, INPUT);
   pinMode(microBackwardPin, INPUT);
}

void loop() {
  readButtons();
  
//  Serial.println("Single coil steps");
//  myMotor->step(400, FORWARD, SINGLE); 
//  myMotor->step(400, BACKWARD, SINGLE); 
//  myMotor.release();
//
//  Serial.println("Double coil steps");
//  myMotor->step(100, FORWARD, DOUBLE); 
//  myMotor->step(100, BACKWARD, DOUBLE);
//  
//  Serial.println("Interleave coil steps");
//  myMotor->step(100, FORWARD, INTERLEAVE); 
//  myMotor->step(100, BACKWARD, INTERLEAVE); 
//  
//  Serial.println("Microstep steps");
//  myMotor->step(50, FORWARD, MICROSTEP); 
//  myMotor->step(50, BACKWARD, MICROSTEP);
    switch (STATE){
      case 20:
        backMicro(2);
      break;
        
      case 10:
        forwardMicro(2);
      break;
      
      case 1:
        advance();
        forwardMicro(10); // was 2
      break;
      default:
       STATE = 0;
      break;
       
    }
}


void advance(){
    Serial.println("--> advance");
     myMotor->setSpeed(50); 
      myMotor->step(25, BACKWARD, SINGLE); 
      myMotor->setSpeed(65); 
      if ( HOWMANY == 4) {
myMotor->step(stepsForFourResistors, BACKWARD, SINGLE); 
      } else {
    myMotor->step(stepsForTenResistors, BACKWARD, SINGLE); 
      }
  
}

void backMicro(int steps){
     Serial.println("--> backMicro");
     myMotor->step(steps, BACKWARD, MICROSTEP); 
}

void forwardMicro(int steps){
    Serial.println("--> forwardMicro");
    myMotor->step(steps, FORWARD, MICROSTEP);
}

void readButtons(){
  
  STATE = 0;
  if ( digitalRead(microBackwardPin) == 1) STATE = 20; 
  if ( digitalRead(microForwardPin) == 1) STATE = 10;
  if ( digitalRead(advancePin) == 1) STATE = 1;
  Serial.print("state = ");
  Serial.println(STATE);
  
}
