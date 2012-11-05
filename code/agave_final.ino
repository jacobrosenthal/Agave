/*
  This code was written for the agave cactus implementation we did for the 
  Mesa Arts Center Creativity Festival, some more info here 
  http://doteki.com/?page_id=96

  The cactus is a set of 26 'motion' sensors attached to 26 lights. When the
  sensor is detected, the 'leaves' should blink ranomly and settle back down.
  
  The loop checks for any triggered 'leaves' and sets a random 
  integer into its triggered array while the timer function constantly shifts 
  bits to the right eventually clearing out the random on/off bits out, 
  settling back to 'off' state.
  
  Created by Jacob Rosenthal, March 7, 2012.
  Released into the public domain.
*/

#include <TimerOne.h>

//Do you want thelights on by default, or off?
#define defaultOn FALSE

//The bits are shifted 8 times, thus the lights will 'bounce' for 8*refresh rate ms
const int refreshRate = 125000;

int max = 24;
int min = 1;

static const uint8_t lights[26] =
{
  0,20,12,11,10,9,8,7,6,5,4,3,2,22,23,24,25,26,14,15,16,17,18,19,13,21 //throw away 0 index to make consistant and easy to address
};

static const uint8_t sensors[26] =
{
  0,51,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,28,52 //throw away 0 index to make consistant and easy to address
};

#ifdef defaultOn
static uint8_t triggered[26] =
{
  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255 //default start on
};
#else
static uint8_t triggered[26] =
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 //default start off
};
#endif

void setup() 
{
  //turn on sensor pin pull ups
  for (int i=1;i<max+1; i++){
  digitalWrite(sensors[i],HIGH);
  }

  //make sensor pins inputs
  for (int i=1;i<max+1; i++){
  pinMode(sensors[i],INPUT); 
  }
  
  //turn off lights to start
  for (int i=1;i<max+1; i++){
  digitalWrite(lights[i],LOW);
  }
  
  //make light pins output
  for (int i=1;i<max+1; i++){
  pinMode(lights[i],OUTPUT);
  }

  Timer1.initialize(refreshRate); // set a timer of length 125000 microseconds (or 0.125 sec, 8hz)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}
 
void loop()
{
  //capture any hits and load them into triggered array
  for(int i = min; i<max+1; i++){
   if(digitalRead(sensors[i])==LOW)
     triggered[i]=random(255); 
  }
}

void timerIsr()
{
  for (int i=min;i<max+1; i++){
    #ifdef defaultOn
    digitalWrite( lights[i], (triggered[i] & B00000001) ^1 ); //write the next bit to leds, default on
    #else
    digitalWrite( lights[i], (triggered[i] & B00000001) ); //write the next bit to leds, default off
    #endif
    triggered[i]=(unsigned int)triggered[i]>>1;
  }
}
