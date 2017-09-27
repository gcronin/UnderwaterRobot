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

boolean synched = false;

void drawArrow(float headWidthFactor, float headLengthFactor) {
  float headWidth = headWidthFactor * 200.0f;
  float headLength = headLengthFactor * 200.0f;
  
  pushMatrix();
  
  // Draw base
  translate(0, 0, -100);
  box(100, 100, 200);
  
  // Draw pointer
  translate(-headWidth/2, -50, -100);
  beginShape(QUAD_STRIP);
    vertex(0, 0 ,0);
    vertex(0, 100, 0);
    vertex(headWidth, 0 ,0);
    vertex(headWidth, 100, 0);
    vertex(headWidth/2, 0, -headLength);
    vertex(headWidth/2, 100, -headLength);
    vertex(0, 0 ,0);
    vertex(0, 100, 0);
  endShape();
  beginShape(TRIANGLES);
    vertex(0, 0, 0);
    vertex(headWidth, 0, 0);
    vertex(headWidth/2, 0, -headLength);
    vertex(0, 100, 0);
    vertex(headWidth, 100, 0);
    vertex(headWidth/2, 100, -headLength);
  endShape();
  
  popMatrix();
}

void drawBoard() {
  pushMatrix();

  rotateY(-radians(yaw - yawOffset));
  rotateX(-radians(pitch));
  rotateZ(radians(roll)); 

  // Board body
  fill(255, 0, 0);
  box(250, 20, 400);
  
  // Forward-arrow
  pushMatrix();
  translate(0, 0, -200);
  scale(0.5f, 0.2f, 0.25f);
  fill(0, 255, 0);
  drawArrow(1.0f, 2.0f);
  popMatrix();
    
  popMatrix();
}

// Skip incoming serial stream data until token is found
boolean readToken(Serial serial, String token) {
  // Wait until enough bytes are available
  if (serial.available() < token.length())
    return false;
  
  // Check if incoming bytes match token
  for (int i = 0; i < token.length(); i++) {
    if (serial.read() != token.charAt(i))
      return false;
  }
  
  return true;
}

// Global setup
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
}

void setupRazor() {
  println("Trying to setup and synch IMU...");
  
  // Synch with Razor
  serial.clear();  // Clear input buffer up to here
}

float readFloat(Serial s) {
  // Convert from little endian (Razor) to big endian (Java) and interpret as float
  return Float.intBitsToFloat(s.read() + (s.read() << 8) + (s.read() << 16) + (s.read() << 24));
}

void draw() {
   // Reset scene
  background(0);
  lights();

  // Sync with Razor 
  if (!synched) {
    textAlign(CENTER);
    fill(255);
    text("Connecting to IMU...", width/2, height/2, -200);
    
    if (frameCount == 2)
      setupRazor();  // Set ouput params and request synch tokens
    else if (frameCount > 2)
      synched = readToken(serial, "X: ");  // Look for synch token
    return;
  }
  
  text("Connected", width/2, height/2, -200);
  while (serial.available() > 0) {
    byte[] inBuffer = new byte[20];
    int inByte = serial.read();
    if(inByte == 88)   //ASCII X
    {
        serial.readBytesUntil(13, inBuffer);  // read until a new line
        String myString = new String(inBuffer);
        if(myString.indexOf("Y") != -1)
        {
          println("No Y");
          //yaw = float(myString.substring(0, myString.indexOf("Y")));
          //println(yaw);
        }
        if(myString.indexOf("X") != -1)
        {
          println("No X");
        }
        if(myString.indexOf("Z") != -1)
        {
          println("No Z");
        }
    }
  }
/*  
  // Read angles from serial port
  while (serial.available() >= 12) {
    yaw = readFloat(serial);
    pitch = readFloat(serial);
    roll = readFloat(serial);
  }

  // Draw board
  pushMatrix();
  translate(width/2, height/2, -350);
  drawBoard();
  popMatrix();
  
  textFont(font, 20);
  fill(255);
  textAlign(LEFT);

  // Output info text
  text("Align ROV and press 'a' to align", 10, 25);

  // Output angles
  pushMatrix();
  translate(10, height - 10);
  textAlign(LEFT);
  text("Yaw: " + ((int) yaw), 0, 0);
  text("Pitch: " + ((int) pitch), 150, 0);
  text("Roll: " + ((int) roll), 300, 0);
  popMatrix();
  */
}

void keyPressed() {
  switch (key) {
    case 'a':  // Align screen with Razor
      yawOffset = yaw;
  }
}