// codes for a mini arduino webserver to connect with submarine controller client.
// H31iUMx49 @ 1.3.2020

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "includes/server_pages.h"
#include <Servo.h>

Servo servo_1, servo_2, servo_3;

const char *ssid = "";
const char *password = "";

const int WiFi_Indicator = 2;
const int Server_Indicator = 16;
const int Server_Port = 80;

// change pin 
const uint8_t servo_1_pin = 12; // D6
const uint8_t servo_2_pin = 14; // D5
const uint8_t servo_3_pin = 15; // D8
const uint8_t main_motor_pin = 4; // D2
const int main_motor_direction_pin = 5;

// init server with port
ESP8266WebServer server(Server_Port);

void setup()
{
	Serial.begin(9600);

	init_board();
	connect_access_point();

	if (MDNS.begin("esp8266"))
	{
		Serial.println("MDNS responder started ...");
	}

	route();
	servo_1.attach(servo_1_pin);
	servo_2.attach(servo_2_pin);
	servo_3.attach(servo_3_pin);

	server.begin();
	Serial.print("Server started at http://");
	Serial.print(WiFi.localIP());
	Serial.print(":");
	Serial.print(Server_Port);
	Serial.println("/ ...");
	digitalWrite(Server_Indicator, LOW);
}

void loop()
{
	server.handleClient();
}

void connect_access_point()
{
	Serial.println("");
	// make connection to access point
	Serial.print("Connecting to ");
	Serial.print(ssid);
	Serial.println(" ");
	WiFi.begin(ssid, password);

	int timer = 0;
	// print connecting message until wifi is connected!
	while (WiFi.status() != WL_CONNECTED)
	{
		// if ESP can't connect with AP within 60s, will reset.
		if (timer == 60000)
		{
			Serial.print("Cannot connect to ");
			Serial.print(ssid);
			Serial.println(". Restarting ...");
			ESP.reset();
		}
		Serial.print(".");
		digitalWrite(WiFi_Indicator, !digitalRead(WiFi_Indicator));
		timer += 500;
		delay(500);
	}
	// 0x0 = Low to turn the LED on
	digitalWrite(WiFi_Indicator, LOW);
	// successfully connected to AP and return Local IP Address
	Serial.println("");
	Serial.print("connected! Local IP address is ");
	Serial.println(WiFi.localIP());
}

void init_board()
{
	Serial.println("Wait ... Booting ESP8266 ...");
	// init pins
	pinMode(WiFi_Indicator, OUTPUT);
	pinMode(Server_Indicator, OUTPUT);

	// pinMode(servo_1_pin, OUTPUT);
	// pinMode(servo_2_pin, OUTPUT);
	// pinMode(servo_3_pin, OUTPUT);
	pinMode(main_motor_pin, OUTPUT);
	pinMode(main_motor_direction_pin, OUTPUT);

	// 0x1 = HIGH to turn LED off
	digitalWrite(WiFi_Indicator, HIGH);
	digitalWrite(Server_Indicator, HIGH);
}

void route()
{
	// send 404 page - inspired by Apache httpd 404 page
	server.onNotFound([]() -> void {
		server.send(404, "text/html", svr_builtin::page_404(server.uri(), WiFi.localIP(), Server_Port));
	});
	// handle root route -> send hello world! for example
	server.on("/", []() -> void {
		server.send(200, "text/html", svr_builtin::page_home(server.uri(), WiFi.localIP(), Server_Port));
	});
	// handle control requests
	server.on("/api", []() -> void {
		if (server.args() <= 4)
		{
			// int main_motor_direction = server.arg("main_motor").toInt() >= 0 ? 1 : 0;
			// int main_motor_speed = map(server.arg("main_motor").toInt(), 0, 100, 0, 1024);

			// int servo_1 = map(server.arg("servo_1").toInt(), 0, 180, 0, 1024);
			// int servo_2 = map(server.arg("servo_2").toInt(), 0, 180, 0, 1024);
			// int servo_3 = map(server.arg("servo_3").toInt(), 0, 180, 0, 1024);

			// // send to motor controller
			// analogWrite(main_motor_pin, main_motor_speed);
			// digitalWrite(main_motor_direction_pin, main_motor_direction);

			// analogWrite(servo_1_pin, servo_1);
			// analogWrite(servo_2_pin, servo_2);
			// analogWrite(servo_3_pin, servo_3);
			servo_1.write(server.arg("servo_1").toInt());
			servo_2.write(server.arg("servo_2").toInt());
			servo_3.write(server.arg("servo_3").toInt());
			server.send(200, "text/plain", "accepted!");
			Serial.println("send!");
		}
		else
		{
			server.send(422, "text/html", svr_builtin::page_422(server.uri(), WiFi.localIP(), Server_Port));
		}
	});
}
