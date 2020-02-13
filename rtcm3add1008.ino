#include <AltSoftSerial.h>

/*
  Reads in a stream of RTCM3 data on the software serial
  port, outputs it to the built-in serial port.  If
  a message type of 1005 or 1006 is detected, a blank
  1008 message will be inserted into the output stream,
  which should allow a ZED-F9P RTK base station to work with
  Trimble roving receivers, which require 1008.

  Pin 8 is RX from the radio module (unused)
  Pin 9 is TX to the radio module

  Pin 0 is RX from the F9P
  Pin 1 is TX to the F9P (unused)

  For Uno or similar board. Other boards might require
  different pins for the AltSoftSerial port.  See the
  AltSoftSerial documentation for more information.
 */

// Blank RTCM3 type 1008 message
const char packet1008[12] = { 0xd3,0x00,0x06,0x3f,0x00,0x00,0x00,0x00,0x00,0x99,0x25,0xca };

AltSoftSerial mySerial(10,9); // RX, TX

void setup() {
	Serial.begin(57600);
	while (!Serial) {
	; // wait for serial port to connect. Needed for native USB port only
	}

	// set the data rate for the SoftwareSerial port
	mySerial.begin(57600);
}

void loop() {
	byte c;
	int length;
	unsigned int type;

	int count;
	c = 0;

	//Look for start of RTCM3 message
	while (c != 0xd3) {
		if (Serial.available()) {
			c = Serial.read();
			mySerial.write(c); //pass it through to the radio
		}
	}
  
	//Okay we might have found one, lets get the length
	count = 0;
	length = 0;
	while (count < 2) {
		if (Serial.available()) {
			c = Serial.read();
			mySerial.write(c);
			length = (length << 8) + c;
			count ++;
		}
	}
	length = length & 0x07ff; //isolate only the least significant 10 bits
 
	//identify the message type
	count = 0;
	type = 0;
	while (count < 2) {
		if (Serial.available()) {
			c = Serial.read();
			mySerial.write(c);
			type = (type << 8) + c;
			count ++;
		}
	}
	type = type >> 4; //isolate type form the most significant 12 bits

	//Now pass through the rest of the message
	count = 0;
	while (count < length + 1) {
		//read in the message body, less the 2 type bytes, and then
		//the 3 CRC bytes, so length + 1.
		if (Serial.available()) {
			c = Serial.read();
			mySerial.write(c);
			count ++;
		}
	}

	if (type == 1005 || type == 1006) {
		//inject a 1008 message
		mySerial.write(packet1008, 12);
	}
}
