//==============================================================================
//title              :lidar_v13.ino
//description        :Control two stepper motor with A4988 for mapping
//                   :Read distance from VL53LXX-V2
//author             :John Bigeon	(https://github.com/JohnBigeon/PinAr_Lidar)
//date               :20220421
//version            :for arduino MEGA (tested on 1.8.19 @ubuntu)    
//usage              :library required: VL53L0X by Pololu (version 1.3.0)
//script_version     :0.9-release
//Output             :Control stepper motor
//notes              :Problem with conversion of floats to string with old version (native with linux !!!)
//==============================================================================
// -*- coding: utf-8 -*-/
#include <Wire.h>
#include <VL53L0X.h>

//==============================================================================
// Globals
//LIDARLite lidarLite;
VL53L0X sensor;

//Variable declaration
int microstepping   = 1;       //XXX The stepper driver configuration (microstepping=8, 4, 2 or 1)
int cal_cnt = 0;
int gear_reduc = 6;
int fullcycle_theta = 200*microstepping*gear_reduc;    // number of step for full cycle for 360deg/0.9deg=200steps
int fullcycle_phi   = 200*microstepping*gear_reduc;    // number of step for full cycle for 360deg/0.9deg=200steps
float resol         = 0.9/(microstepping*gear_reduc);    // resolution of stepper
int pos1_init       = 0;        // position initial of first  motor
int pos2_init       = 0;        // position initial of second motor
int pos1            = 0;        // position of first  motor
int pos2            = 0;        // position of second motor
float pos1r         = 0;        // position of first  motor in degrees
float pos2r         = 0;        // position of second  motor in degrees
int mydelay         = 2000;       // (microseconds) time we are waiting at precise position
int delay_switch    = 5;        // (ms) time between two pulses send to motor for stepping
int incomingByte    = 0;    // for incoming serial data
int delaybackward   = 1000;
String action;   //Declare a String variable to hold your name

//==============================================================================
// For stepper 1
//#define pin1Enable 13 // Activation du driver/pilote
#define pin1Step    3   // Step
#define pin1Dir     4   // Direction 
#define pin1MS1     44
#define pin1MS2     46

// For stepper 2
#define pin2Step    6   // Step
#define pin2Dir     5   // Direction 
#define pin2MS1     26
#define pin2MS2     24

// Pin 13 has an LED connected on most Arduino boards
int led = 13;

//==============================================================================

void setup(){
    Serial.begin(9600);  // turn on Serial Port
//    lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
//    lidarLite.configure(0); // Change this number to try out alternate configurations
    Wire.begin();

    sensor.setTimeout(500);
    if (!sensor.init())
    {
        Serial.println("Failed to detect and initialize sensor!");
        while (1) {}
    }
    Serial.println("Welcome aboard to Air PinAr_Lidar, 3D positioning");
    
    // initialize the digital pin as an output
    pinMode(led, OUTPUT);  
  
    // Initialisation of stepper1
    pinMode(pin1Dir , OUTPUT);
    pinMode(pin1Step, OUTPUT);
    pinMode(pin1MS1, OUTPUT);
    pinMode(pin1MS2, OUTPUT);
  
    // Initialisation of stepper2
    pinMode(pin2Dir, OUTPUT);
    pinMode(pin2Step, OUTPUT);
    pinMode(pin2MS1, OUTPUT);
    pinMode(pin2MS2, OUTPUT);

    Serial.setTimeout(10); // dont wait too long for emergency case
}

//==============================================================================
//Here comes the sun
//==============================================================================

void loop(){
    int dist;
    int i = 0;
    int j = 0;
    
    // To display and communicate
    String stringpos1txt = "motor1 pos: "; 
    String stringpos2txt = "motor2 pos: "; 
    String sensorvaltxt  = "sensor val: ";
    String separator     = ", ";
    
    // Stepper initialisation
    digitalWrite(pin1Step, LOW);
    digitalWrite(pin2Step, LOW);

    // Init. MS1 & MS2
    microstepping_config(microstepping);
    
    // Blink to say Hello
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);               // wait for a second

 

    // Select a command to do
    Serial.println("Ignition, what is my purpose ?");
    Serial.println("Test (T) ? Fast 2D (F) ? Precise 3D (P) ? nb: Emergency case (X)");

    while(Serial.available()==0){       // Wait for User to input data
    // send data only when you receive data:
    }

    action=Serial.readString();         //Read user input into action
    Serial.println(action);
 
    
    if (action == "T" || action == "F" || action == "P"){


        if (action == "T"){
          
          digitalWrite(pin1Dir, HIGH); // Direction forward
          
          for (i=0; i<fullcycle_theta + 1; i++){
              
              pos1 = pos1_init + i;
              pos2 = 0;
       
              // Read the value from the sensor
              dist = sensor.readRangeSingleMillimeters();
             
              // Display real degree, and not step value
              pos1r = pos1 * resol;   pos2r = pos2 * resol;
              String stringpos     = pos1r + separator + pos2r + separator + dist;
              Serial.println(stringpos);
              
              digitalWrite(pin1Step, HIGH);
              delayMicroseconds(mydelay);
              digitalWrite(pin1Step, LOW);
              delayMicroseconds(mydelay);
          }
          delay(1000); // Wait a second
          
          digitalWrite(pin2Dir, HIGH); 
                   
          for(j=0; j<fullcycle_phi/4 + 1; j++){
               
              pos2 = pos2_init + j;
                      
              // Read the value from the sensor
              dist = sensor.readRangeSingleMillimeters();     
                      
              // Display real degree, and not step value
              pos1r = pos1 * resol;   pos2r = pos2 * resol;
              String stringpos     = pos1r + separator + pos2r + separator + dist;
              Serial.println(stringpos);
                    
              digitalWrite(pin2Step, HIGH);
              delayMicroseconds(mydelay);
              digitalWrite(pin2Step, LOW);
              delayMicroseconds(mydelay);
          }
          digitalWrite(led, LOW); Serial.println(""); Serial.println("Job done !");
          homing(i, j);        
        }
        
        if (action == "F") {
           delay(2500);
           String config_com  = "Config microstepping used: ";
           String config_used = config_com + microstepping + " " ;
           Serial.println(config_used);

            delay(5000);  // Wait for User
            while(true) { // never stop the music

                //Serial.println("GogogadjetoFoton !");
                digitalWrite(led, HIGH);   // turn the LED on
                digitalWrite(pin1Dir, HIGH); // Direction forward

                // Fullcycle step for first motor
                for (i=0; i<fullcycle_theta/2 + 1; i++){
          
                    pos1 = pos1_init + i;
                    pos2 = 0;
          
                    // Read the value from the sensor
                    dist = sensor.readRangeSingleMillimeters();

                    // Display real degree, and not step value
                    pos1r = pos1 * resol;   pos2r = pos2 * resol;
                    String stringpos     = pos1r + separator + pos2r + separator + dist;
                    Serial.println(stringpos);

                    
                    digitalWrite(pin1Step, HIGH);
                    delayMicroseconds(mydelay);
                    digitalWrite(pin1Step, LOW);
                    delayMicroseconds(mydelay);
                }

                digitalWrite(pin1Dir, LOW); // Direction backward
                
                // Fullcycle step for first motor
                for (i=0; i<fullcycle_theta/2+1; i++){
          
                    pos1 = fullcycle_theta/2 - (pos1_init + i);
                    pos2 = 0;
          
                    // Read the value from the sensor
                    dist = sensor.readRangeSingleMillimeters();
                    
                    // Display real degree, and not step value
                    pos1r = pos1 * resol;   pos2r = pos2 * resol;
                    String stringpos     = pos1r + separator + pos2r + separator + dist;
                    Serial.println(stringpos);
                    
                    
                    digitalWrite(pin1Step, HIGH);
                    delayMicroseconds(mydelay);
                    digitalWrite(pin1Step, LOW);
                    delayMicroseconds(mydelay);
                }   // Fullcycle step for first motor
                
                // Emergency break
                action=Serial.readString();         //Read user input into action
                if (action == "X"){
                    //homing(i, 0);
                    break;
                }
        }
        digitalWrite(led, LOW); Serial.println("Job done !");
    }
        
///////////////////////////////////////////////////////////////////////////////////////////
        if (action == "P") {
            delay(1000);  //wait user

            Serial.println("GogogadjetoFoton !");
            digitalWrite(led, HIGH);   // turn the LED on

        // Fullcycle step for first motor
            for (i=0; i<fullcycle_theta + 1; i++){
          
                pos1 = pos1_init + i;
                
          
                // Fullcycle step for second motor
                for(j=0; j<fullcycle_phi/4 + 1; j++){
            
                //For even ==> forward, odd, backward //A number is "even" when you can divide it by two and have zero remainder
                    if ((i % 2) == 0) {
                        digitalWrite(pin2Dir, HIGH); pos2 = pos2_init + j;} 
                    else {
                        digitalWrite(pin2Dir,  LOW); pos2 = fullcycle_phi/4 - j;}
                    //pos2 = pos2_init + j;
            
                    // Read the value from the sensor
                    dist = sensor.readRangeSingleMillimeters();
            
                    // Display real degree, and not step value
                    pos1r = pos1 * resol;   pos2r = pos2 * resol;
                    String stringpos     = pos1r + separator + pos2r + separator + dist;
                    Serial.println(stringpos);         
          
                    digitalWrite(pin2Step, HIGH);
                    delayMicroseconds(mydelay);
                    digitalWrite(pin2Step, LOW);
                    delayMicroseconds(mydelay);
                } // Fullcycle step for second motor
                
                digitalWrite(pin1Dir, HIGH); // Direction forward
                digitalWrite(pin1Step, HIGH);
                delayMicroseconds(mydelay);
                digitalWrite(pin1Step, LOW);
                delayMicroseconds(mydelay);
                
                // Emergency break
                action=Serial.readString();         //Read user input into action
                if (action == "X"){
                    homing(i, j);
                    break;
                }
                //Serial.println("");       
            }   // Fullcycle step for first motor
                digitalWrite(led, LOW); Serial.println(""); Serial.println("Job done !");
                homing(i, j);
        }
    }
else {Serial.println("What do you mean ???");}
}

void homing(int cycle_theta, int cycle_phi){
    int i = 0;
    int j = 0;
    Serial.println("Coming Back");
    // Start Homing procedure of Stepper Motor at startup
    delay(delaybackward);
    Serial.println("Start Homing procedure of Stepper Motors at origin");
      
    // Direction modification
    digitalWrite(pin1Dir, LOW); // Direction backward
    digitalWrite(pin2Dir, LOW); // Direction backward
      
    for(i=0; i<cycle_theta; i++){
        String stheta     = "theta ";
        pos1r = (cycle_theta - i) * resol;
        String postheta   = stheta + pos1r;
//        Serial.println(postheta); // XXXDebug
        digitalWrite(pin1Step, HIGH);
        delay(delay_switch);
        digitalWrite(pin1Step, LOW);
        delay(delay_switch);
    }
      
    for(j=0; j<cycle_phi; j++){
    //For even ==> forward, odd, backward //A number is "even" when you can divide it by two and have zero remainder
        if ((cycle_phi % 2) == 0) 
            {digitalWrite(pin2Dir, LOW);} 
        else 
            {digitalWrite(pin2Dir, HIGH);}
      
        String sphi = "phi ";
        pos2r = (cycle_phi - j) * resol;
        String postheta   = sphi + pos2r;
//        Serial.println(postheta); // XXXDebug
    
        digitalWrite(pin2Step, HIGH); delayMicroseconds(mydelay);; digitalWrite(pin2Step, LOW); delayMicroseconds(mydelay);;
    }
    digitalWrite(pin1Dir, LOW); digitalWrite(pin1Step, LOW); digitalWrite(pin2Dir, LOW); digitalWrite(pin2Step, LOW);
}

void microstepping_config(int microstep_pin){
    // Initialisation of MS1&MS2
    if (microstep_pin==1){
        digitalWrite(pin1MS1, LOW);
        digitalWrite(pin1MS2, LOW);
        digitalWrite(pin2MS1, LOW);
        digitalWrite(pin2MS1, LOW);
    }
  
    if (microstep_pin==2){
        digitalWrite(pin1MS1, HIGH);
        digitalWrite(pin1MS2, LOW);
        digitalWrite(pin2MS1, HIGH);
        digitalWrite(pin2MS1, LOW);
    }  
  
    if (microstep_pin==4){
        digitalWrite(pin1MS1, LOW);
        digitalWrite(pin1MS2, HIGH);
        digitalWrite(pin2MS1, LOW);
        digitalWrite(pin2MS1, HIGH);
    }
  
  if (microstep_pin==8){
        digitalWrite(pin1MS1, HIGH);
        digitalWrite(pin1MS2, HIGH);
        digitalWrite(pin2MS1, HIGH);
        digitalWrite(pin2MS1, HIGH);
    }
}
