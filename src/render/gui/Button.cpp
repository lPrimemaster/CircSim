#include "Button.h"

Button::Button()
{
	border = new Component("Rectangle");
}

Button::~Button()
{
	delete border;
}
