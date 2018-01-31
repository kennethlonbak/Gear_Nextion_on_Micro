#include <Joystick.h>

// Initilizing joystick
Joystick_ Joystick(0x04,
	JOYSTICK_TYPE_JOYSTICK, 2, 0,
	true, false, false, false, false, false,
	false, false, false, false, false);

// Declaring variabels ------------------------------------------------------ //
// Nextion
static long baud = 9600;
static long newBaud = baud;
int readSize = 0;
int endofOutcomingMessageCount = 0;

// Joystick
int A0_val; // Shift up
int A1_val;
int A2_val; // Shift down
int A3_val;
bool set_by_gear = false;

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
void read_and_set_buttons(){
    read_pins();
    //test_pin_input();
    set_buttons();
}
void read_pins(){
  A0_val = analogRead(0);
  A1_val = analogRead(1);
  A2_val = analogRead(2);
  A3_val = analogRead(3);
}

void set_buttons(){
  // Shift up
  if (A0_val < 350){Joystick.setXAxis(0);set_by_gear=true;} else {set_by_gear=false;} // engaging cluch at gear shift
  if (A0_val < 300){Joystick.setButton(0,1);} else {Joystick.setButton(0,0);} 
  // Shift down 
  if (A2_val > 650){Joystick.setXAxis(0);set_by_gear=true;} 
  if (A2_val > 700){Joystick.setButton(1,1);} else {Joystick.setButton(1,0);}
  // Clutch
  if (!set_by_gear) {Joystick.setXAxis(A2_val);}
}

void test_pin_input(){
  Serial.print("A0=");
  Serial.print(A0_val);
  Serial.print(", A1=");
  Serial.print(A1_val);
  Serial.print(", A2=");
  Serial.print(A2_val);
  Serial.print(", A3=");
  Serial.println(A3_val);
}

// Initial setup ----------------------------------------------------------- //
void setup() {
  // Initilizing pins 
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);

  // Initilizing Serial connection to Nextion and Virtual
	Serial.begin(baud);
	Serial1.begin(baud);

  // Initilizing Joystick
	Joystick.begin();
  Joystick.setXAxisRange(0, 500);
 
  // Refresh gamepad even if the serial port not open
	while (!Serial) {read_and_set_buttons();}
}

// Main loop ------------------------------------------------------------- //
void loop() {

  // Reading pin values and setting buttons
	read_and_set_buttons();

  // Serial comunication
  Serial_Read_and_Write();
}
