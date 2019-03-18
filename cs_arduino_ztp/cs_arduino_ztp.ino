/***********************
 * Name: new thermopile sensor compressive sensing 
 * Author: Libo Wu, libo.wu@tamu.edu, libo.wu1992@gmail.com
 * 
 */

#include <SoftwareSerial.h>
#include <Stepper.h>

#include <Wire.h>
#include <Adafruit_ADS1015.h>


//---( Number of steps per revolution of INTERNAL motor in 4-step mode )---
#define STEPS_PER_MOTOR_REVOLUTION 32
// Number of steps per revolution of OUTPUT SHAFT (= gear reduction; 2048 steps)
#define STEPS_PER_OUTPUT_REVOLUTION 32*64

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
//The pin connections need to be pins 8,9,10,11 connected
// to Motor Driver In1, In2, In3, In4 
// Then the pins are entered here in the sequence 1-3-2-4 for proper sequencing
Stepper my_stepper(STEPS_PER_MOTOR_REVOLUTION, 8, 10,9, 11);

// attach softwareserial of BT
// SoftwareSerial BTserial(5,4); //set Bluetooth Rx=D5,Tx=D4

Adafruit_ADS1115 ads;

int samples_per_move = 10;  //when using a new mask, the samples to collect
int opticalpin = 2;  //use pin 2 (D2) as external interrupt
int resetpin = 3;
int step2take[8]={254,254,254,254,254,254,254,128};
int step2take_usb0[8]={256,256,256,254,254,254,254,100};
                                                         
int initmoves=2048; // counter that uses when going to init position
int step4init=int(STEPS_PER_OUTPUT_REVOLUTION/initmoves); //in initalization, the tolarance of each steps to take is 2048/256=8
int incoming; // serial readin number, in ascii (0-127)
volatile bool motorflag;  //motorflag=true: stepper motor is running, false: is stopped
int myspeed=600;
float multiplier = 0.0625F; // multiplier is the 1bit resolution: 1x gain = 0.125mV
int16_t adc0, adc1, adc2, adc3;

// setup
void setup(){
  Serial.begin(38400);
   //                                                                ADS1015  ADS1115
  //                                                                -------  -------
   //ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
   //ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
   ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV                                                               -------  -------

  //Serial.println("Connected to BT.");
  
  pinMode(opticalpin, INPUT);
  digitalWrite(resetpin, HIGH);
  delay(100);
  pinMode(resetpin, OUTPUT);
  ads.begin();
}

//loop
void loop(){

  if (Serial.available()) {
    // read the incoming:
    incoming = Serial.read();
    //********************************//
    // Method 2: use external interrupt, 
    //********************************//
    if(incoming==51){  //ascii(51)=3, reset the arduino
      digitalWrite(resetpin, LOW);
    }
//*************************************************//    
    if(incoming==49){ //ascii(49)=1
      //BTserial.println(incoming);
      //BTserial.println("enter init");
      
      my_stepper.setSpeed(myspeed);                               
      motorflag=true;
//      BTserial.print("motorflag is:"); BTserial.println(motorflag);
      
      noInterrupts ();  //disable INT
      attachInterrupt (0, StopStepper, RISING);   //attach INT0
      interrupts ();  //enable INT
      
      delay(1000);  
      for (int i=0;i<2*initmoves;i++){  
        if(motorflag){
          my_stepper.step(step4init);
        }
        else{
          Serial.println("1,1,1,1");    //1,1,1,1 indicate the doing to the initial point
          break;
        }
      } //loop over 2*initmoves. Use 2*initmoves to avoid the case that the endstop is blocked, but motorflag is still true. 
      detachInterrupt(0);
      Serial.println("command 1 complete");
      return;
    } //end command 1
//*********************************************//    
    if(incoming==50){ //ascii(50)=2
      //BTserial.println(incoming);
      my_stepper.setSpeed(myspeed);
      for (int k=0;k<4;k++){ //record 4 rounds
        motorflag=true;
        noInterrupts ();  //disable INT
        attachInterrupt (0, StopStepper, RISING);   //attach INT0
        interrupts ();  //enable INT     
        delay(500);  
        for (int i=0;i<2*initmoves;i++){  
          if(motorflag){
            my_stepper.step(step4init);
          }
          else{
            Serial.println("2,2,2,2");  //1,1,1,1 indicate the doing to the initial point
            break;
          }
        } //loop over initmoves. Use 2*initmoves to avoid the case that the endstop is blocked, but motorflag is still true. 
        detachInterrupt(0);
        //my_stepper.step(-2*step4init);  //compensate off set due to inertial of the stepper motor
        delay(500);
        
        //start rotating and retriving data
        for (int i=0;i<8;i++){
          for (int j=0;j<samples_per_move;j++){   
            adc0 = ads.readADC_SingleEnded(0);
            adc1 = ads.readADC_SingleEnded(1);
            Serial.print(k+1); Serial.print(",");Serial.print(i+1); Serial.print(",");Serial.print(adc0); Serial.print(",");Serial.println(adc1);
            delay(25);
          } //samples per move is 30, each will have a delay of 25ms, total 750ms+2000ms=2.75 seconds
          my_stepper.step(step2take[i]);
          //my_stepper.step(step2take_usb0[i]);  // only use when upload to usb0 device, env node
          delay(500);
        } //end i loop, time=2.75*9=25 seconds,
      } //end k loop,  time is (25+1)*10=260 sec
      Serial.println("command 2 complete");
    }   //end command 2
  } //end serial.available
} //end loop



void findInitPos(){ 
  my_stepper.setSpeed(myspeed);                               
  for (int i=0;i<initmoves;i++){
    if(digitalRead(opticalpin)==LOW){
      my_stepper.step(step4init);
    }
    else{
      Serial.println("command 2: arrive init");
      break;
    }
  }
  my_stepper.step(-2);  //compensate degree
  delay(500);
  return;
}//run this function for every time all 9 masks are used. Similar idea of command1 except not using INT, but just polling opticalpin level.

void findInitPos_2(){
  motorflag=true;
      
  noInterrupts ();  //disable INT
  attachInterrupt (0, StopStepper, RISING);   //attach INT0
  interrupts ();  //enable INT
  
  delay(500);  
  for (int i=0;i<2*initmoves;i++){  
    if(motorflag){
      my_stepper.step(step4init);
    }
    else{
      Serial.println("command 2: arrive init");
      break;
    }
  } //loop over initmoves. Use 2*initmoves to avoid the case that the endstop is blocked, but motorflag is still true. 
  detachInterrupt(0);
  //my_stepper.step(-2*step4init);  //compensate off set due to inertial of the stepper motor
  delay(500);
  return;
} //use INT0 to find init position instead of polling 

void StopStepper(){
  motorflag=false;
} //ISR, when D2 has rising edge, set flag to false.

