#include <Joystick.h>

Joystick_ Joystick(0x04,
	JOYSTICK_TYPE_JOYSTICK, 5, 0,
	false, false, false, false, false, false,
	false, false, false, false, false);

static long baud = 9600;
static long newBaud = baud;
int butValue1;
int butValue2;
int butValue3;

void lineCodingEvent(long baud, byte databits, byte parity, byte charFormat)
{
	newBaud = baud;
}

void readButtons() {
	butValue1 = !digitalRead(2);
	Joystick.setButton(0, butValue1);
  
  butValue2 = !digitalRead(3);
  Joystick.setButton(1, butValue2);
  
  butValue3 = !digitalRead(4);
  Joystick.setButton(2, butValue3);
}

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

	Serial.begin(baud);
	Serial1.begin(baud);

	Joystick.begin();

	while (!Serial) {
		// Refresh gamepad even if the serial port not open
		readButtons();
	}

}

int readSize = 0;
int endofOutcomingMessageCount = 0;

void loop() {

	readButtons();
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
