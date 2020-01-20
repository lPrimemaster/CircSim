#include "Button.h"

Button::Button()
{
	quad = new Component("Rectangle");
}

Button::~Button()
{
	delete quad;
}
