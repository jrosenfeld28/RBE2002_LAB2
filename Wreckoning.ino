//#include <wiring.h>
#include <avr/wdt.h>
#include "Arduino.h"
#include "button.h"
#include "Zumo32U4.h"
#include "event_timer.h"  //include your shiny, new event timer class
#include <Zumo32U4Motors.h>
#include <Zumo32U4Encoders.h>

EventTimer timerA(2000);
DebouncedButton buttonA(14, HIGH);

Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
Zumo32U4LCD lcd;






enum ROBOT_STATE {DRIVING, IDLING};
ROBOT_STATE state = IDLING;

struct Segment
{
  int leftMotorEffort;
  int rightMotorEffort;
  int duration; //in ms

};

int const numberOfSegments = 4; //wait, fwd, turn, fwd
Segment* segments = new Segment{numberOfSegments};
int iSegment = 0;

void Drive(int Number)
{
  motors.setSpeeds(segments[Number].leftMotorEffort, segments[Number].rightMotorEffort);
}


void setup()
{
  Serial.begin(115200);
  Serial.println("Hello.");

  buttonA.Init();

  segments[0] = Segment {0, 0, 1000};
  segments[1] = Segment {150, 150, 2500};
  segments[2] = Segment {150, -150, 540};
  segments[3] = Segment {150, 150, 1400};
  
}

void loop()
{
    //event-driven structure
    //if(SomeEvent()) HandleSomeEvent()
    
  if (buttonA.CheckButtonPress()) handleButtonPress(); // need check button and hadle button
  if (timer.CheckExpired()) handleTimer(); // need check timer and handle timer
  Serial.print('\n');
  Serial.print(Zumo32U4Encoders::getCountsLeft());
  Serial.print('\n');
  Serial.print('\t');
  

}


//add in handlers:

//handle button press>>

void handleButtonPress()
{
  iSegment = 1;
  Drive(iSegment);
  state = DRIVING;
}

//handle timer expiring>>

void handleTimer()
{
  switch (state)
  {
    case DRIVING:
      Drive(iSegment);
      iSegment++;
      state = IDLING;
      break;

     case IDLING:
      Zumo32U4Motors::setLeftSpeed(0);
      Zumo32U4Motors::setRightSpeed(0);
      timer.set(1000);
      state = DRIVING;
      break;
  }
}
