#include "Arduino.h"
#include "NeoPixelRingLightController.h"

NeoPixelRingLightController::NeoPixelRingLightController(int pin, int numberOfPixels)
{
	assignedPin = pin;
	assignedPixels = numberOfPixels;
}

NeoPixelRingLightController::~NeoPixelRingLightController()
{

}

void NeoPixelRingLightController::SetRingLightColor(int position, uint32_t color)
{
	pixels.setPixelColor(position, color);
}

void NeoPixelRingLightController::SetAllLightsColor(uint32_t color)
{
	for (int i = 0; i < assignedPixels; i++)
	{
		SetRingLightColor(i, color);
	}
}

void NeoPixelRingLightController::UpdateRingLight()
{
		pixels.show();
}

uint32_t NeoPixelRingLightController::GetColor(uint8_t r, uint8_t g, uint8_t b)
{
	return pixels.Color(r, g, b);
}

void NeoPixelRingLightController::Begin()
{
	pixels = Adafruit_NeoPixel(assignedPixels, assignedPin, NEO_GRB + NEO_KHZ800);
	pixels.begin(); // This initializes the NeoPixel library.
	pixels.clear();
}
