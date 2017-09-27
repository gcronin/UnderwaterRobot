/* ////////////////////////////////////////////
 *  Arduino UNO Code for ROV
 *  
 *  Reads a Serial string passed from Arduino Micro
 *  Parses String to get send values
 *  
 *  motorA : Port Horizonal
 *  motorB : Starboard Horizontal
 *  motorC : Port 45deg
 *  motorD : Starboard 45 deg
 *  
 *  pulses[0]:  throttle = up/down (C/D same direction)
 *  pulses[1]:  yaw = rotate on vertical axis (A/B opposite directions)
 *  pulses[2]:  pitch = forward/backward (A/B same direction)
 *  pulses[3]:  strafe = side to side (C/D opposite directions)
 *  pulses[4]:  camera pitch servo
 *  pulses[5]:  gripper open/close
 *  pulses[6]:  gripper rotate
 */
 
#include <Servo.h>
#include <BN0055I2C.h>

BN0055I2C imu;
// set true to display progress and calibration status
boolean debug = true;

int pulses[7] = {0, 0, 0, 0, 0, 0, 0};

char commandEndString = 'Z';
char commandBeginString = '$';
char commandDelimiterString = ',';
String readPulses = "";

int cameraPitchPin = 8;
int gripperOpenPin = 13;
int gripperRotatePin = 12;
int motorAPin = 14;
int motorBPin = 15;
int motorCPin = 16;
int motorDPin = 17;
int LEDpin = 7;

Servo cameraPitch, gripperOpen, gripperRotate, motorA, motorB, motorC, motorD;

void setup() {
  Serial.begin(9600);
  cameraPitch.attach(cameraPitchPin);
  gripperOpen.attach(gripperOpenPin);
  gripperRotate.attach(gripperRotatePin);
  motorA.attach(motorAPin);
  motorB.attach(motorBPin);
  motorC.attach(motorCPin);
  motorD.attach(motorDPin);
  pinMode(LEDpin, OUTPUT);  // LED lights up when BN0055 is fully calibrated
  if(imu.setup(debug)) { Serial.println("Setup BN0055 Successful$"); }
  if(imu.useCrystal(debug)) { Serial.println("BN0055 Using External Clock$"); }
  while(imu.readGyroCalibration() != 3) {}  // let the gyro sit for a bit to calibrate
}

void loop() {
  readIMU(debug);
  readCommandString(debug);
  setServoPositions();
  setMotorSpeeds();
}


void readCommandString(boolean debug)
{ 
  
  if(Serial.available()>0)
  {
    readPulses = Serial.readStringUntil(commandEndString);
    if(readPulses.indexOf(commandBeginString) < 5)   // found $ sign near beginning 
    {
      readPulses.remove(0, readPulses.indexOf(commandBeginString)+1);  //strip off up to the intro character
      for(int i = 0; i < 7; i++)
      {
        // read and store up to the next delimiter
        pulses[i] = readPulses.substring(0, readPulses.indexOf(commandDelimiterString)).toInt();
        // trim off the part that was just read plus the delimiter
        readPulses.remove(0, readPulses.indexOf(commandDelimiterString) + 1);
      }
    }
  }
}

void setServoPositions()
{
  cameraPitch.writeMicroseconds(checkPulse(pulses[5]));
  gripperOpen.writeMicroseconds(checkPulse(pulses[4]));
  gripperRotate.writeMicroseconds(checkPulse(pulses[6]));
}

void setMotorSpeeds()
{
  motorA.writeMicroseconds(checkPulse(pulses[0]));
  motorB.writeMicroseconds(checkPulse(pulses[1]));
  motorC.writeMicroseconds(checkPulse(pulses[2]));
  motorD.writeMicroseconds(checkPulse(pulses[3]));
}

int checkPulse(int pulse)
{
  if(pulse > 2200 || pulse < 800) pulse = 1500;
  return pulse;
}

void readIMU(boolean debug)
{
  imu.readEuler();
  if(debug)
  {
    Serial.print("X: ");
    Serial.print(imu.head);
    Serial.print(" Y: ");
    Serial.print(imu.pitch);
    Serial.print(" Z: ");
    Serial.print(imu.roll);
    Serial.println("$"); //end character
  }
  imu.getCalibration(false);
  if(imu.calibrated) digitalWrite(LEDpin, HIGH);
  else digitalWrite(LEDpin, LOW); 
}
