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

// --------------------------------------
// Global Variables
// --------------------------------------
double speed = 55.5; //Velocidad media
char slope[] = "FLAT";
int ligth = 0;
int lamp = 0;
int acc = 0;
int brk = 0;
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
      if(0 == strcmp("UP",slope){
         sprintf(answer,"SLP:  %s",slope);
      } else{
         sprintf(answer,"SLP:%s",slope);
      }

      // set buffers and flags
      memset(request,'\0', MESSAGE_SIZE+1);
      request_received = false;
      answer_requested = true;
   }
   return 0;
}

// --------------------------------------
// Function: ligth_req
// --------------------------------------
int ligth_req()
{
   // while there is enough data for a request
   if ( (request_received) &&
        (0 == strcmp("LIT: REQ",request)) ) {
      char num_str[5];
      dtostrf(ligth,4,1,num_str);
      // send the answer for slope request
      sprintf(answer,"LIT:%s",ligth);

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
        (0 == strcmp("GAS: SET",request)) ) {
           acc = 1;
           digitalWrite(13, acc);
           request_received = false;
           answer_requested = true;
        }

      else if( (request_received) &&
        (0 == strcmp("GAS: CLR",request)) ){
           acc = 0;
           digitalWrite(13, acc);
           request_received = false;
           answer_requested = true;
        }
   sprintf(answer,"GAS:  OK");
   return 0;
}

// --------------------------------------
// Function: break_req
// --------------------------------------
int break_req()
{
      if ( (request_received) &&
        (0 == strcmp("BRK: SET",request)) ) {
           brk = 1;
           digitalWrite(12, brk);
           request_received = false;
           answer_requested = true;
        }

      else if( (request_received) &&
        (0 == strcmp("BRK: CLR",request)) ){
           brk = 0;
           digitalWrite(12, brk);
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
        (0 == strcmp("MIX: SET",request)) ) {
           mixer = 1;
           digitalWrite(10, mixer);
           request_received = false;
           answer_requested = true;
        }

      else if( (request_received) &&
        (0 == strcmp("MIX: CLR",request)) ){
           mixer = 0;
           digitalWrite(10, mixer);
           request_received = false;
           answer_requested = true;
        }
   return 0;
}

// --------------------------------------
// Function: lamp_req
// --------------------------------------
int lamp_req()
{
   if ( (request_received) &&
        (0 == strcmp("LAM: SET",request)) ) {
           lamp = 1;
           digitalWrite(7, lamp);
           request_received = false;
           answer_requested = true;
        }

      else if( (request_received) &&
        (0 == strcmp("LAM: CLR",request)) ){
           lamp = 0;
           digitalWrite(7, lamp);
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
   double a = 0;
   if(acc==1){
      //V = Vo + A T; A = 0.5
      a = a + 0.5;
   }
   if(brk==1){
      //V = Vo + A T; A= -0.5
       a = a - 0.5;
   }
   if(0 == strcmp("UP",slope)){
      //V = Vo + A T; A= -0.25
      a = a - 0.25;
   }
   if(0 == strcmp("DOWN",slope)){
      //V = Vo + A T; A= 0.25
      a = a + 0.25;
   }

   speed = speed + a*t;

   int ligth_speed = map (speed, 0, 70, 0, 255);
   analogWrite(10, ligth_speed);

   return 0;
}

// --------------------------------------
// Function: get_ligth
// --------------------------------------
int get_ligth()
{
  int value = 0;
  value = analogRead(0);
  ligth = map (value, 0, 1023, 0, 100);
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

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, INPUT);
  pinMode(8, INPUT);
  pinMode(7, OUTPUT);
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
