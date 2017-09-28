import processing.opengl.*;
import processing.serial.*;

final static int SERIAL_PORT_NUM = 0;
final static int SERIAL_PORT_BAUD_RATE = 9600;

float yaw = 0.0f;
float pitch = 0.0f;
float roll = 0.0f;
float yawOffset = 0.0f;

PFont font;
Serial serial;

boolean connectedFlag = false;  //tells us whether we are connected to the Arduino..
long timeLastConnected;

void setup() {
  // Setup graphics
  size(640, 480, OPENGL);
  smooth();
  noStroke();
  frameRate(50);
  
  // Load font
  font = loadFont("Univers-66.vlw");
  textFont(font);
  
  // Setup serial port I/O
  String portName = Serial.list()[SERIAL_PORT_NUM];
  serial = new Serial(this, portName, SERIAL_PORT_BAUD_RATE);
  serial.bufferUntil('\n');  //input buffer is filled until a newline is received, then serialEvent is triggered
}

void draw() {
  background(0);
  lights();
  ShowConnectionStatus();
  SerialTimeoutCheck();
}

/////////////////////////////////SERIAL METHODS////////////////////////////////////////////
void SerialTimeoutCheck()
{
  if(millis() - timeLastConnected > 500) 
    connectedFlag = false;
}


void ShowConnectionStatus()
{
  if(connectedFlag) {
    fill(0,255,0);
    text("Connected", width/2, height/2, -200); }
  else {
    fill(255,0,0);
    text("Disconnected", width/2, height/2, -200); }
}

void serialEvent(Serial serial)  // this executes whenever \n character is received
{   
  // read the serial buffer:
  String myString = serial.readStringUntil('$');
  myString = myString.substring(0, myString.indexOf('$'));
  String[] inData = split(myString, ' ');
  if(inData.length > 5)
  { 
    yaw = float(inData[1]);
    pitch = float(inData[3]);
    roll = float(inData[5]);
    println(yaw + " " + pitch + " " + roll);
  }
  connectedFlag = true;
  timeLastConnected = millis();
}