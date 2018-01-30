#include <Joystick.h>

// Initilizing joystick
Joystick_ Joystick(0x04,
	JOYSTICK_TYPE_JOYSTICK, 5, 0,
	false, false, false, false, false, false,
	false, false, false, false, false);

// Declaring variabels ------------------------------------------------------ //
// Nextion
static long baud = 9600;
static long newBaud = baud;
int readSize = 0;
int endofOutcomingMessageCount = 0;

// Joystick
int butValue1;
int butValue2;
int butValue3;

// Nextion ------------------------------------------------------------------- //
void lineCodingEvent(long baud, byte databits, byte parity, byte charFormat){	newBaud = baud;}

void Serial_Read_and_Write(){
  newBaud = Serial.baud();
  
  if (newBaud != baud) {
    baud = newBaud;
    Serial1.end();
    Serial1.begin(baud);
  }

  // copy from virtual serial line to uart and vice versa
  readSize = 0;
  while (Serial.available() && readSize < 32) {
    char c = (char)Serial.read();
    readSize++;
    Serial1.write(c);
  }
 
  readSize = 0;
  while (Serial1.available() && readSize < 32) {
    char c = (char)Serial1.read();
    readSize++;
    Serial.write(c);
  }
}
// Joystick/buttons --------------------------------------------------------- //
void readButtons() {
	butValue1 = !digitalRead(2);
	Joystick.setButton(0, butValue1);
  
  butValue2 = !digitalRead(3);
  Joystick.setButton(1, butValue2);
  
  butValue3 = !digitalRead(4);
  Joystick.setButton(2, butValue3);
}

void read_pins(){
  
}

void set_buttons(){
  
}

// Initial setup ----------------------------------------------------------- //
void setup() {
  // Initilizing pins 
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

  // Initilizing Serial connection to Nextion and Virtual
	Serial.begin(baud);
	Serial1.begin(baud);

  // Initilizing 
	Joystick.begin();
 
  // Refresh gamepad even if the serial port not open
	while (!Serial) {readButtons();}

}

// Main loop ------------------------------------------------------------- //
void loop() {

  // Reading pin values and setting buttons
	readButtons();

  // Serial comunication
  Serial_Read_and_Write();
}
