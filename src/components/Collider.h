#pragma once
#include <iostream>
#include "../core/ECS.h"
#include "../util/math.h"

class Collider : public FCS::Component
{
public:
	inline void setBounding(const math::OBB& obb)
	{
		bounding = obb;
	}

	inline void updateBounding(const glm::vec2& p, const float& angle, const glm::vec2& dimensions = glm::vec2(0.0f))
	{
		bounding.pivot = p;
		bounding.angle = angle;
		if (dimensions.x != 0.0f)
			bounding.dimensions = dimensions;
	}

	inline math::OBB getBounding() const
	{
		return bounding;
	}

private:
	math::OBB bounding;

	FCS_COMPONENT(Collider);
};