#include "Arduino.h"
#include "HtmlBTCPriceProvider.h"

HtmlBTCPriceProvider::HtmlBTCPriceProvider(int samples)
{
	BTCTicker_payload = "";
	float fSamples = samples;
	sampleSizeOld = (fSamples - 1.0f)/ fSamples;
	sampleSizeNew = 1 / fSamples;
}

HtmlBTCPriceProvider::~HtmlBTCPriceProvider()
{

}

void HtmlBTCPriceProvider::UpdatePrice()
{
	HTTPClient http;
	http.begin("https://api.coindesk.com/v1/bpi/currentprice/USD.json");
	int httpCode = http.GET();
	if (httpCode == HTTP_CODE_OK)
	{
		BTCTicker_payload = http.getString();
		ParsePrice();
	}
	else
	{
		BTCTicker_payload = "";
	}
}

float HtmlBTCPriceProvider::GetAveragedPrice()
{
	return averagedBTCPrice;
}

bool HtmlBTCPriceProvider::IsConnectionOk()
{
	return (BTCTicker_payload != "");
}

bool HtmlBTCPriceProvider::IsTrendingUp()
{
	return currentBTCPrice > averagedBTCPrice;
}

void HtmlBTCPriceProvider::ParsePrice()
{
	DynamicJsonBuffer jsonBuffer(bufferSize);
	char json[800];
	BTCTicker_payload.toCharArray(json, 800);

	JsonObject& root = jsonBuffer.parseObject(json);

	JsonObject& bpi_USD = root["bpi"]["USD"];
	float bpi_USD_rate_float = bpi_USD["rate_float"]; //Current Up-to-date price
	currentBTCPrice = bpi_USD_rate_float;

	if (averagedBTCPrice == -1)
	{
		averagedBTCPrice = bpi_USD_rate_float;	
	}
	else
	{
		averagedBTCPrice = (averagedBTCPrice * sampleSizeOld) + (averagedBTCPrice * sampleSizeNew);
	}
}
