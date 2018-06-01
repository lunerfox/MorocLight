#pragma once
#include <Arduino.h>

class CosineIntegerProvider
{
public:
	CosineIntegerProvider(int min = 0, int max = 255);
	~CosineIntegerProvider();
	void Update(double timeStep);
	int Value();
private:
	double accumulator;
	int minVal, maxVal;
};

