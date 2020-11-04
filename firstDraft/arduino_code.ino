// --------------------------------------
// Include files
// --------------------------------------
#include <string.h>
#include <stdio.h>
#include <Wire.h>

// --------------------------------------
// Global Constants
// --------------------------------------
#define SLAVE_ADDR 0x8
#define MESSAGE_SIZE 8

void setup() 
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, INPUT);
  pinMode(8, INPUT);
}

// --------------------------------------
// Global Variables
// --------------------------------------
double speed = 55.5; //Velocidad media
char slope[] = "FLAT";
int acc = 0;
int breack = 0;
int mixer = 0;
bool request_received = false; 
bool answer_requested = false;
char request[MESSAGE_SIZE+1];
char answer[MESSAGE_SIZE+1];

// --------------------------------------
// Handler function: receiveEvent
// --------------------------------------
void receiveEvent(int num)
{
   char aux_str[MESSAGE_SIZE+1];
   int i=0;

   // read message char by char
   for (int j=0; j<num; j++) {
      char c = Wire.read();
      if (i < MESSAGE_SIZE) {
         aux_str[i] = c;
         i++;
      }
   }
   aux_str[i]='\0';

   // if message is correct, load it
   if ((num == MESSAGE_SIZE) && (!request_received)) {
      memcpy(request, aux_str, MESSAGE_SIZE+1);
      request_received = true;
   }
}

// --------------------------------------
// Handler function: requestEvent
// --------------------------------------
void requestEvent()
{
   // if there is an answer send it, else errorshow_speed
   if (answer_requested) {
      Wire.write(answer,MESSAGE_SIZE);
      memset(answer,'\0', MESSAGE_SIZE+1);
   } else {
      Wire.write("MSG: ERR",MESSAGE_SIZE);
   }
   
   // set answer empty
   request_received = false;
   answer_requested = false;
   memset(request,'\0', MESSAGE_SIZE+1);
   memset(answer,'0', MESSAGE_SIZE);
}

// --------------------------------------
// Function: speed_req
// --------------------------------------
int speed_req()
{
   // while there is enough data for a request
   if ( (request_received) &&
        (0 == strcmp("SPD: REQ",request)) ) {
      // send the answer for speed request
      char num_str[5];
      dtostrf(speed,4,1,num_str);
      sprintf(answer,"SPD:%s",num_str);

      // set buffers and flags
      memset(request,'\0', MESSAGE_SIZE+1);
      request_received = false;
      answer_requested = true;
   }
   return 0;
}

// --------------------------------------
// Function: slope_req
// --------------------------------------
int slope_req()
{
   // while there is enough data for a request
   if ( (request_received) &&
        (0 == strcmp("SLP: REQ",request)) ) {
      // send the answer for slope request
      sprintf(answer,"SLP:%s",slope);

      // set buffers and flags
      memset(request,'\0', MESSAGE_SIZE+1);
      request_received = false;
      answer_requested = true;
   }
   return 0;
}

// --------------------------------------
// Function: acc_req
// --------------------------------------
int acc_req()
{
      if ( (request_received) &&
        (0 == strcmp("GAS: SET",answer)) ) {
           acc = 1;
           digitalWrite(13, acc);
           request_received = false;
           answer_requested = true;
        }

      else if( (request_received) &&
        (0 == strcmp("GAS: CLR",answer)) ){
           acc = 0;
           digitalWrite(13, acc);
           request_received = false;
           answer_requested = true;
        }
   return 0;
}

// --------------------------------------
// Function: break_req
// --------------------------------------
int break_req()
{
      if ( (request_received) &&
        (0 == strcmp("BRK: SET",answer)) ) {
           breack = 1;
           digitalWrite(12, breack);
           request_received = false;
           answer_requested = true;
        }

      else if( (request_received) &&
        (0 == strcmp("BRK: CLR",answer)) ){
           breack = 0;
           digitalWrite(12, breack);
           request_received = false;
           answer_requested = true;
        }
   return 0;
}

// --------------------------------------
// Function: mixer_req
// --------------------------------------
int mixer_req()
{
   if ( (request_received) &&
        (0 == strcmp("MIX: SET",answer)) ) {
           mixer = 1;
           digitalWrite(10, mixer);
           request_received = false;
           answer_requested = true;
        }

      else if( (request_received) &&
        (0 == strcmp("MIX: CLR",answer)) ){
           mixer = 0;
           digitalWrite(10, mixer);
           request_received = false;
           answer_requested = true;
        }
   return 0;
}

// --------------------------------------
// Function: get_slope
// --------------------------------------
int get_slope()
{
   int value = 0;
   if(digitalRead(9)==1){
      slope = "DOWN"
   }

   if(digitalRead(8)==1){
      slope = "UP"
   } 

   else{
      slope = "FLAT"
   }

   return 0;
}

// --------------------------------------
// Function: show_speed
// --------------------------------------
int show_speed()
{
   if(acc==1){
      //V = Vo + A T; A = 0.5
      speed = speed + 0.5*t;
   }
   if(breack==1){
      //V = Vo + A T; A= -0.5
      speed = speed + (-0.5)*t;
   }
   if(0 == strcmp("UP",slope){
      //V = Vo + A T; A= -0.25
      speed = speed + (-0.25)*t;
   }
   if(0 == strcmp("DOWN",slope){
      //V = Vo + A T; A= 0.25
      speed = speed + 0.25*t;
   }

   int ligth_speed = map (speed, 0, 70, 0, 255);
   digitalWrite(10, ligth_speed);

   return 0;
}

// --------------------------------------
// Function: setup
// --------------------------------------
void setup()
{
  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);
  
  // Function to run when data requested from master
  Wire.onRequest(requestEvent);
  
  // Function to run when data received from master
  Wire.onReceive(receiveEvent);
}

// --------------------------------------
// Function: loop
// --------------------------------------
void loop()
{
   //para cuando se lo piden, devolverlo
    speed_req();
    slope_req();
   //Chechealr internamente los datos - Pagar luces, leer slope, velociad, etc.
    show_speed();
    check_slope();
}
