//==============================================================================
//title              :one_stepper_rotation_v01.ino
//description        :Control one stepper motor with A4988
//                   :Inspired by https://how2electronics.com/control-stepper-motor-with-a4988-driver-arduino/
//author             :John Bigeon  (https://github.com/JohnBigeon/PinAr_Lidar)
//date               :20220206
//version            :for arduino MEGA (tested on 1.8.19 @ubuntu)    
//usage              :
//script_version     :0.9-release
//Output             :Control stepper motor
//notes              :MS1, MS2 and MS3 are LOW
//==============================================================================

const int dirPin = 6;
const int stepPin = 5;
const int stepsPerRevolution = 200;
 
void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}
void loop()
{
  // Set motor direction clockwise
  digitalWrite(dirPin, HIGH);
 
  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  delay(1000); // Wait a second
  
  // Set motor direction counterclockwise
  digitalWrite(dirPin, LOW);
 
  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second
}
