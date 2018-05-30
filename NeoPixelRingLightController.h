#pragma once

#include <Adafruit_NeoPixel.h>

class NeoPixelRingLightController
{
public:
	enum LIGHTMODE {MANUAL, ANIM_PROCESSING, ANIM_PULSING, ANIM_NORMAL};
	NeoPixelRingLightController(int pin, int numberOfPixels);
	~NeoPixelRingLightController();
	void SetRingLightColor(int position, uint32_t Color);
	void SetAllLightsColor(uint32_t color);
	void UpdateRingLight();
	void ChangeLightMode(LIGHTMODE newMode);
	uint32_t GetColor(uint8_t r, uint8_t g, uint8_t b);
	void Begin();
private:
	LIGHTMODE mode;
	Adafruit_NeoPixel pixels;
	int assignedPin;
	int assignedPixels;
};

