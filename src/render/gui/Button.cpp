#include "Button.h"

Button::Button()
{
	quad = new Sprite("Rectangle");
}

Button::~Button()
{
	delete quad;
}
