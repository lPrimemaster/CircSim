#include "Button.h"

Button::Button()
{
	quad = new GraphicComponent("Rectangle");
}

Button::~Button()
{
	delete quad;
}
