#pragma once

#include <Adafruit_NeoPixel.h>

class NeoPixelRingLightController
{
public:
	NeoPixelRingLightController(int pin, int numberOfPixels);
	~NeoPixelRingLightController();
	void SetRingLightColor(int position, uint32_t Color);
	void SetAllLightsColor(uint32_t color);
	void UpdateRingLight();
	uint32_t GetColor(uint8_t r, uint8_t g, uint8_t b);
	void Begin();
private:
	Adafruit_NeoPixel pixels;
	int assignedPin;
	int assignedPixels;
};

