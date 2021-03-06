#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ThreadController.h>
#include <Thread.h>
#include <StaticThreadController.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "RotatingIntegerProvider.h"
#include "HtmlBTCPriceProvider.h"
#include "NeoPixelRingLightController.h"

const char* ssid = "Squishyland2ghz";
const char* password = "squishynchewy";

float curPrice;

//Workers
Thread SetupThread = Thread();
Thread OTAServiceThread = Thread();
Thread WebServerThread = Thread();
Thread BTCTickerThread = Thread();
Thread NeoPixelDisplayThread = Thread();

ESP8266WebServer server = ESP8266WebServer(80);
HtmlBTCPriceProvider ticker = HtmlBTCPriceProvider(120);
NeoPixelRingLightController pixels = NeoPixelRingLightController(D7, 16);

int timeSinceLastWifiConnect = 0;
bool BTCDataAvailable = false;

bool initialize_Wifi()
{
	const int maximum_attempts = 5;
	Serial.println("Configuring Wifi");
	WiFi.mode(WIFI_STA);

	Serial.println("Connecting...");
	WiFi.begin(ssid, password);
	timeSinceLastWifiConnect++;
	if (timeSinceLastWifiConnect > maximum_attempts)
	{
		Serial.println("No response. Resetting...");
		digitalWrite(D6, LOW);
		delay(5000);
		digitalWrite(D6, HIGH);
	}
	
	return (WiFi.status() == WL_CONNECTED);

}

void initialize_OTA()
{
	// Port defaults to 8266
	ArduinoOTA.setPort(8266);

	// Hostname defaults to esp8266-[ChipID]
	ArduinoOTA.setHostname("Moroc-Light");

	// No authentication by default
	// ArduinoOTA.setPassword((const char *)"123");

	ArduinoOTA.onStart([]() {
		Serial.println("Start");
	});
	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});
	ArduinoOTA.begin();
}

void initialize_WebServer()
{
	server.on("/", WebServerProcessRoot);
	server.begin();
}

void setup()
{
	pinMode(D0, OUTPUT);
	pinMode(D6, OUTPUT);
	digitalWrite(D6, HIGH);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(D7, OUTPUT);

	Serial.begin(115200);
	Serial.println("Booting");

	Serial.println("Setup Neo Pixels");
	pixels.Begin();

	SetupThread.onRun(SetupHandler);
	SetupThread.setInterval(2500);

	NeoPixelDisplayThread.onRun(NeoPixelDisplayHandler);
	NeoPixelDisplayThread.setInterval(100);

	OTAServiceThread.onRun(OTAServiceHandler);
	OTAServiceThread.setInterval(2000);
	
	WebServerThread.onRun(WebServerHandler);
	WebServerThread.setInterval(250);

	BTCTickerThread.onRun(BTCTickerHandler);
	BTCTickerThread.setInterval(30000);

	Serial.println("Starting Main Loop");
}

// Evaluate the threads to see which one needs to be run.
void loop()
{
	if (SetupThread.shouldRun()) SetupThread.run();
	if (NeoPixelDisplayThread.shouldRun()) NeoPixelDisplayThread.run();

	//Once it's setup, run the other web-dependent threads.
	if (!SetupThread.enabled)
	{
		if (WebServerThread.shouldRun()) WebServerThread.run();
		if (OTAServiceThread.shouldRun()) OTAServiceThread.run();
		if (BTCTickerThread.shouldRun()) BTCTickerThread.run();
		if (BTCDataAvailable) pixels.ChangeLightMode(NeoPixelRingLightController::ANIM_NORMAL);
	}
}

void SetupHandler()
{
	pixels.ChangeLightMode(NeoPixelRingLightController::ANIM_PULSING);
	
	Serial.println("Setup Handling");
	if (initialize_Wifi())
	{
		Serial.println("Connection Made");
		pixels.ChangeLightMode(NeoPixelRingLightController::ANIM_PROCESSING);
		// Port defaults to 8266
		ArduinoOTA.setPort(8266);

		// Hostname defaults to esp8266-[ChipID]
		ArduinoOTA.setHostname("Moroc-Light");

		// No authentication by default
		// ArduinoOTA.setPassword((const char *)"123");

		ArduinoOTA.onStart([]() {
			Serial.println("Start");
		});
		ArduinoOTA.onEnd([]() {
			Serial.println("\nEnd");
		});
		ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
			Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
		});
		ArduinoOTA.onError([](ota_error_t error) {
			Serial.printf("Error[%u]: ", error);
			if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
			else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
			else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
			else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
			else if (error == OTA_END_ERROR) Serial.println("End Failed");
		});
		ArduinoOTA.begin();

		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
		initialize_WebServer();
		Serial.println("WebServer Configured");

		pixels.SetAllLightsColor(pixels.GetColor(0, 0, 200));
		pixels.UpdateRingLight();
		SetupThread.enabled = false;
	}
}

void OTAServiceHandler()
{
	Serial.println("OTA Handling");
	digitalWrite(LED_BUILTIN, LOW);
	ArduinoOTA.handle();
	delay(50);
	digitalWrite(LED_BUILTIN, HIGH);
}

void WebServerHandler()
{
	Serial.println("Server Handling");
	server.handleClient();
}

void BTCTickerHandler()
{
	Serial.println("Ticker Handling");
	ticker.UpdatePrice();
	curPrice = ticker.currentBTCPrice;
	BTCDataAvailable = true;
}

void NeoPixelDisplayHandler()
{
	pixels.UpdateRingLight(0.1);
}

void WebServerProcessRoot()
{
	String response;
	if (ticker.IsConnectionOk())
	{
		response = "Moroc Light Reporting In [BTC / USD]: ";
		response += " Last Hour $" + String(ticker.GetAveragedPrice());
		response += " Real-time $" + String(ticker.currentBTCPrice);
	}
	else
	{
		response = "Error: Failed to retrieve data from API.";
	}

	server.send(200, "text/plain", response);
}