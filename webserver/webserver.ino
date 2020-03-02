// codes for a mini arduino webserver to connect with submarine controller client.
// H31iUMx49 @ 1.3.2020

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "includes/server_pages.h"

const char *ssid = "acc3ss_p0int";
// const char *ssid = "acc3ss_p0int";
const char *password = "p@ssw0rd";

const int WiFi_Indicator = 2;
const int Server_Indicator = 16;
const int Server_Port = 80;

// init server with port
ESP8266WebServer server(Server_Port);

void setup()
{
	Serial.begin(9600);

	pinMode(WiFi_Indicator, OUTPUT);
	pinMode(Server_Indicator, OUTPUT);

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
	digitalWrite(Server_Indicator, 0x0);
}

void loop()
{
	server.handleClient();
}

void connect_access_point()
{
	// make connection to access point
	Serial.print("Connecting to ");
	Serial.print(ssid);
	Serial.println(" ");
	WiFi.begin(ssid, password);

	// print connecting message until wifi is connected!
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		digitalWrite(WiFi_Indicator, !digitalRead(WiFi_Indicator));
		delay(500);
	}
	// 0x0 = Low to turn the LED on
	digitalWrite(WiFi_Indicator, 0x0);
	// successfully connected to AP and return Local IP Address
	Serial.println("");
	Serial.print("connected! Local IP address is ");
	Serial.println(WiFi.localIP());
}

void init_board()
{
	// 0x1 = HIGH to turn LED off
	digitalWrite(WiFi_Indicator, 0x1);
	digitalWrite(Server_Indicator, 0x1);
}

void route()
{
	// send 404 page - inspired by Apache httpd 404 page
	server.onNotFound([]() -> void {
		server.send(404, "text/html", svr_builtin::page_404(server.uri(), WiFi.localIP(), Server_Port));
	});
	// handle root route -> send hello world! for example
	server.on("/", []() -> void {
		server.send(200, "text/html", "<h1>Hello, World!</h1>");
	});
}