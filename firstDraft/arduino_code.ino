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
  pinMode(10, OUTPUT);
  pinMode(9, INPUT);
  pinMode(8, INPUT);
}

// --------------------------------------
// Global Variables
// --------------------------------------
double speed = 55.5; //Velocidad media
char slope[] = "FLAT";
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
   int ligth_speed = map (speed, 0, 70, 0, 255);
   digitalRead(10, ligth_speed);

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
    speed_req();
    slope_req();
    show_speed();
    check_slope();
}
