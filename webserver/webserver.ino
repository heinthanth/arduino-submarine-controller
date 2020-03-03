// codes for a mini arduino webserver to connect with submarine controller client.
// H31iUMx49 @ 1.3.2020

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "includes/server_pages.h"

const char *ssid = "4cc3ss p0int h3r3";
const char *password = "p@ssw0rd5 h3r3";

const int WiFi_Indicator = 2;
const int Server_Indicator = 16;
const int Server_Port = 80;

// change pin here
const int servo_1_pin = 0;
const int servo_2_pin = 0;
const int servo_3_pin = 0;
const int main_motor_pin = 0;
const int main_motor_direction_pin = 0;

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
	pinMode(servo_1_pin, OUTPUT);
	pinMode(servo_1_pin, OUTPUT);
	pinMode(servo_1_pin, OUTPUT);
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
		if (server.args() == 4)
		{
			float servo_1 = server.arg("servo_1").toFloat();
			float servo_2 = server.arg("servo_2").toFloat();
			float servo_3 = server.arg("servo_3").toFloat();
			float main_motor = server.arg("main_motor").toFloat();
			// parse it
			float servo_1_analog = servo2analog(servo_1);
			float servo_2_analog = servo2analog(servo_2);
			float servo_3_analog = servo2analog(servo_3);
			float main_motor_analog = main2analog(main_motor);
			// let 1 be front;
			int main_motor_direction = (main_motor >= 0) ? 1 : 0;

			Send2PinOut(servo_1_analog, servo_2_analog, servo_2_analog, main_motor_analog, main_motor_direction);
		}
		else
		{
			server.send(422, "text/html", svr_builtin::page_422(server.uri(), WiFi.localIP(), Server_Port));
		}
	});
}

float servo2analog(float deg) {
	// analog output 0 ~ 255
	// deg input 0 ~ 180
	// 
	// 255 => 180;
	// x => (x / 255) * 180
	return (deg / 255) * 180;
}

float main2analog(float threadshold) {
	const int MAX_RANGE = 1;
	// input range be MAX_RANGE ~ 0 ~ -MAX_RANGE
	// motor range 0 ~ 255
	//
	// MAX_RANGE => 255
	// x => ( |threadshold| / MAX_RANGE) * 255
	return ( abs(threadshold) / MAX_RANGE ) * 255;
}

void Send2PinOut(float servo_1, float servo_2, float servo_3, float main_motor, int main_motor_direction) {
	analogWrite(servo_1_pin, servo_1);
	analogWrite(servo_2_pin, servo_2);
	analogWrite(servo_3_pin, servo_3);
	analogWrite(main_motor_pin, main_motor);
	digitalWrite(main_motor_direction_pin, main_motor_direction);
}