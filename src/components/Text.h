#pragma once
#include "../core/ECS.h"
#include <glm/glm.hpp>
#include <cstdarg>

class Text : public FCS::Component
{
public:
	void addText(const std::string& str)
	{
		text += str;
	}

	void formatText(const char* fmt, ...)
	{
		char buffer[512];
		va_list args;
		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		va_end(args);
		text = std::string(buffer);
	}

	std::string getString() const
	{
		return text;
	}

	Color getColor() const
	{
		return color;
	}

	std::string getFontName() const
	{
		return font;
	}

private:
	std::string text;
	std::string font = "test_cmap";
	Color color = Color(1.0f);
	float font_scale = 1.0f;

	FCS_COMPONENT(Text);
};

