#pragma once
#include <iostream>
#include "Transform.h"

class GraphicComponent
{
public:
	GraphicComponent(const std::string& geometry) : assigned_geometry(geometry) {  };
	virtual ~GraphicComponent() = default;

	inline Transform& transform()
	{
		return assigned_transform;
	}

	inline const std::string& geometryName() const
	{
		return assigned_geometry;
	}

	inline const glm::vec4& getColor() const
	{
		return assigned_color;
	}

	inline const void setColor(const glm::vec4& color)
	{
		assigned_color = color;
	}

private:
	Transform assigned_transform;
	std::string assigned_geometry;
	glm::vec4 assigned_color = glm::vec4(1.0);
};

