#include "Arduino.h"
#include "NeoPixelRingLightController.h"


NeoPixelRingLightController::NeoPixelRingLightController(int pin, int numberOfPixels)
{
	assignedPin = pin;
	assignedPixels = numberOfPixels;
	rotatingPos = RotatingIntegerProvider(numberOfPixels, 0);
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
	switch (mode)
	{
		case MANUAL:
			pixels.show();
			break;
		case ANIM_PROCESSING:
			SetAllLightsColor(pixels.Color(0, 0, 0));
			SetRingLightColor(rotatingPos.Value(), pixels.Color(0, 0, 200));
			rotatingPos.Step();
			pixels.show();
			break;
		case ANIM_PULSING:
			break;
		case ANIM_NORMAL:
			break;
		default:
			pixels.show();
			break;
	}
}

void NeoPixelRingLightController::ChangeLightMode(LIGHTMODE newMode)
{
	mode = newMode;
	//Depending on the mode being entered, we should do some initialization.
	switch (mode)
	{
		case MANUAL:
			break;
		case ANIM_PROCESSING:
			break;
		case ANIM_PULSING:
			break;
		case ANIM_NORMAL:
			break;
		default:
			break;
	}
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
