#pragma once

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class HtmlBTCPriceProvider
{
public:
	HtmlBTCPriceProvider(int samples);
	~HtmlBTCPriceProvider();
	void UpdatePrice();
	float GetAveragedPrice();
	float currentBTCPrice = -1;
	bool IsConnectionOk();
	bool IsTrendingUp();

private:
	float sampleSizeOld, sampleSizeNew;
	const size_t bufferSize = JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + 410;
	String BTCTicker_payload = "Retrieving Data";
	float averagedBTCPrice = -1;
	void ParsePrice();
};

