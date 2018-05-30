#include "RotatingIntegerProvider.h"

RotatingIntegerProvider::RotatingIntegerProvider()
{

}

RotatingIntegerProvider::RotatingIntegerProvider(int size, int start = 0)
{
	maximum = size;
	position = start%size;
}

RotatingIntegerProvider::~RotatingIntegerProvider()
{
}

void RotatingIntegerProvider::Step(int steps)
{
	position = position + steps;
	position = position % maximum;
}

int RotatingIntegerProvider::Value()
{
	return position;
}

int RotatingIntegerProvider::Previous(int steps = 1)
{
	return (position + steps) % maximum;
}

int RotatingIntegerProvider::Next(int steps = 1)
{
	return (position - steps) % maximum;
}
