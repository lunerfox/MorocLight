#include "CosineIntegerProvider.h"

CosineIntegerProvider::CosineIntegerProvider(int min, int max)
{
	accumulator = PI;
	minVal = min;
	maxVal = max;
}

CosineIntegerProvider::~CosineIntegerProvider()
{
}

void CosineIntegerProvider::Update(double timeStep)
{
	Serial.println(timeStep);
	accumulator = (accumulator + timeStep);
	Serial.println(accumulator);
	if (accumulator > (2.0f * PI)) accumulator = accumulator - (2.0f * PI);
}

int CosineIntegerProvider::Value()
{
	return minVal + (cos(accumulator)*0.5 + 0.5) * (maxVal - minVal);
}
