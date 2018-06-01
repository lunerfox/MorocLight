#pragma once
#include <Arduino.h>

class RotatingIntegerProvider
{
public:
	RotatingIntegerProvider();
	RotatingIntegerProvider(int size, int start);
	~RotatingIntegerProvider();
	void Step(int steps = 1);
	int Value();
	int Previous(int steps);
	int Next(int steps);
private:
	int position;
	int maximum;
};

