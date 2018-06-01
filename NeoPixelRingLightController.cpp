#include "Arduino.h"
#include "NeoPixelRingLightController.h"


NeoPixelRingLightController::NeoPixelRingLightController(int pin, int numberOfPixels)
{
	assignedPin = pin;
	assignedPixels = numberOfPixels;
	rotatingPos = RotatingIntegerProvider(numberOfPixels, 0);
	cosineValue = CosineIntegerProvider(0, 255);
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

void NeoPixelRingLightController::UpdateRingLight(double timeStep)
{
	int maxIntensity = 200;
	int intensity = cosineValue.Value();
	uint32_t Red = pixels.Color(255, 0, 0);
	uint32_t Green = pixels.Color(0, 255, 0);

	switch (mode)
	{
		case MANUAL:
			pixels.show();
			break;
		case ANIM_PROCESSING:
			SetAllLightsColor(pixels.Color(0, 0, 0));
			SetRingLightColor(rotatingPos.Value(), pixels.Color(0, 0, maxIntensity));
			SetRingLightColor(rotatingPos.Previous(1), pixels.Color(0, 0, maxIntensity / 2));
			SetRingLightColor(rotatingPos.Previous(2), pixels.Color(0, 0, maxIntensity / 8));
			SetRingLightColor(rotatingPos.Previous(3), pixels.Color(0, 0, maxIntensity / 16));
			SetRingLightColor(rotatingPos.Previous(4), pixels.Color(0, 0, maxIntensity / 32));
			rotatingPos.Step();
			pixels.show();
			break;
		case ANIM_PULSING:
			cosineValue.Update(timeStep);
			SetAllLightsColor(pixels.Color(intensity, 0, 0));
			pixels.show();
			break;
		case ANIM_NORMAL:
			//This is used to show the normal application's animation.
			SetAllLightsColor(Red);
			SetRingLightColor(1, Green);
			SetRingLightColor(5, Green);
			SetRingLightColor(12, Green);
			SetRingLightColor(8, Green);
			pixels.show();
			break;
		default:
			pixels.show();
			break;
	}
}

void NeoPixelRingLightController::ChangeLightMode(LIGHTMODE newMode)
{
	//Reject duplicate commands.
	if (mode == newMode) return;
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
	SetAllLightsColor(pixels.Color(0, 0, 0));
	pixels.show();
}
