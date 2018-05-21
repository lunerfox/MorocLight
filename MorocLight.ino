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
#include "HtmlBTCPriceProvider.h"

const char* ssid = "Squishyland2ghz";
const char* password = "squishynchewy";

float curPrice;

//Workers
Thread OTAThread = Thread();
Thread WebServerThread = Thread();
Thread BTCTickerThread = Thread();

ESP8266WebServer server = ESP8266WebServer(80);
HtmlBTCPriceProvider ticker = HtmlBTCPriceProvider(120);

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(115200);
	Serial.println("Booting");
	WiFi.mode(WIFI_STA);
	
	while (WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		WiFi.begin(ssid, password);
		Serial.println("Waiting for Connection");
		if (WiFi.waitForConnectResult() != WL_CONNECTED)
		{
			ESP.reset();
		}
	}

	Serial.println("Connection Made");

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
	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	OTAThread.onRun(OTAHandler);
	OTAThread.setInterval(2000);

	server.on("/", WebServerProcessRoot);
	server.begin();

	WebServerThread.onRun(WebServerHandler);
	WebServerThread.setInterval(250);

	BTCTickerThread.onRun(BTCTickerHandler);
	BTCTickerThread.setInterval(30000);

	BTCTickerThread.run();

	Serial.print("Starting Main Loop");
}

// Add the main program code into the continuous loop() function
void loop()
{
	if (WebServerThread.shouldRun()) WebServerThread.run();
	if (OTAThread.shouldRun()) OTAThread.run();
	if (BTCTickerThread.shouldRun()) BTCTickerThread.run();
}

void OTAHandler()
{
	digitalWrite(LED_BUILTIN, LOW);
	ArduinoOTA.handle();
	delay(50);
	digitalWrite(LED_BUILTIN, HIGH);
}

void WebServerHandler()
{
	server.handleClient();
}

void WebServerProcessRoot()
{
	String response;
	if (ticker.IsConnectionOk())
	{
		float curPrice = ticker.GetAveragedPrice();
		response = "Moroc Light Reporting In: [BTC/USD] $" + String(curPrice);	
	}
	else
	{ 
		response = "Error: Failed to retrieve data from API.";
	}

	server.send(200, "text/plain", response);
		
}

void BTCTickerHandler()
{
	ticker.UpdatePrice();
	curPrice = ticker.currentBTCPrice;
}