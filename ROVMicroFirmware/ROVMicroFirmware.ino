/*  ////////////////////////////////////////////////
 *   Arduino Micro Code for ROV
 *   
 *   Reads RC information and forwards
 *   along over software Serial to Arduino
 *   on the ROV.
 *   
 *   Setup:  
 *   Software Serial TX:  11
 *   Software Serial RX:  10
 *   RC Throttle: 2
 *   RC Yaw: 3
 *   RC Pitch: 4
 *   RC Roll: 5
 *   RC Gear: 6
 *   
 */

#include <SoftwareSerial.h>

SoftwareSerial ROV(10, 11); // RX, TX

int pulses[7] = {0, 0, 0, 0, 0, 0, 0};

// Pins for RC Receiver
int RCThrottle = 2;
int RCYaw = 3;
int RCPitch = 4;
int RCRoll = 5;
int RCGear = 6;

String commandEndString = "Z";
String commandBeginString = "$";
String commandDelimiterString = ",";
char IMUdata[50];
long timestamp = 0;

void setup()
{
  Serial.begin(9600);  //connect to computer USB
  delay(100);
  ROV.begin(9600);  // connect to ROV
  delay(100);
  pinMode(RCThrottle, INPUT);
  pinMode(RCYaw, INPUT);
  pinMode(RCPitch, INPUT);
  pinMode(RCRoll, INPUT);
  pinMode(RCGear, INPUT);
}

void loop()
{  
  // read information from the ROV 
  //readIMUfromROV(IMUdata);
  Serial.println(IMUdata);
  if(millis() - timestamp > 250)
  {
    readPulses(pulses);
    sendPulses(true, false);
    timestamp = millis();
  }
}

void readPulses(int *pulses)
{
  pulses[0] = pulseIn(RCThrottle, HIGH);
  pulses[1] = pulseIn(RCYaw, HIGH);
  pulses[2] = pulseIn(RCPitch, HIGH);
  pulses[3] = pulseIn(RCRoll, HIGH);
  pulses[4] = pulseIn(RCGear, HIGH);
  pulses[5] = 1500;
  pulses[6] = 1500;
}

void sendPulses(boolean toROV, boolean toComputer)
{
  String commandString = commandBeginString;
  for(int i = 0; i < 7; i++)
  {
    commandString += pulses[i];
    commandString += commandDelimiterString;
  }
  commandString += commandEndString;
  if(toROV) { ROV.println(commandString);   }
  if(toComputer) { Serial.println(commandString); }
}

void readIMUfromROV(char *IMUdata)
{
  memset(&IMUdata[0], 0, sizeof(IMUdata));
  while(ROV.available()) {
    char c = ROV.read();
    if(c != commandBeginString.charAt(0))
    {
      IMUdata += c;  //don't think this will work... need to be indexing the char array?
    }
    else
    {
      break;  //exit while loop
    }
  } 
}


