#include <Arduino.h>

void setup() {
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
}

void loop() {
	digitalWrite(7, HIGH);
	digitalWrite(8, LOW);

	delay(3000);

	digitalWrite(7, LOW);
	digitalWrite(8, HIGH);

	delay(3000);
}