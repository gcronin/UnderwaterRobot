#include <Adafruit_SoftServo.h>  // SoftwareServo (works on non PWM pins)

Adafruit_SoftServo gripper;

void setup() {
    // Set up the interrupt that will refresh the servo for us automagically
  OCR0A = 0xAF;            // any number is OK
  TIMSK |= _BV(OCIE0A);    // Turn on the compare interrupt (below!)
  
  // put your setup code here, to run once:
  pinMode(0, INPUT_PULLUP);
  gripper.attach(1);
  gripper.write(90);
  delay(15);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(0))
  {
    gripper.write(90);
  }
  else
  {
    gripper.write(180);
  }
    
}


// We'll take advantage of the built in millis() timer that goes off
// to keep track of time, and refresh the servo every 20 milliseconds
volatile uint8_t counter = 0;
SIGNAL(TIMER0_COMPA_vect) {
  // this gets called every 2 milliseconds
  counter += 2;
  // every 20 milliseconds, refresh the servos!
  if (counter >= 20) {
    counter = 0;
    gripper.refresh();
   }
}
