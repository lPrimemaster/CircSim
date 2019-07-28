#pragma once
#include <iostream>

class Backgrid
{
public:
	Backgrid();
	~Backgrid();

	//evaluate a certain size of grid and return in worldspace pov
	void evaluate(float gridX, float gridY);
};

