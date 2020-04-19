#pragma once
#include <iostream>
#include "../core/ECS.h"
#include "Transform.h"
#include "../assets/Texture.h"
#include "../assets/Registry.h" 

class Sprite : public FCS::Component
{
public:
	inline bool& alphaBlend()
	{
		return enable_blend;
	}

	// TODO: Change this name later
	inline void addDifuseTest(const std::string& tex_name)
	{
		texture = Registry::GetAsset<Texture>(tex_name);
	}

	inline void bind()
	{
		texture->bind();
	}

private:
	bool enable_blend = false;
	Texture* texture = nullptr;

	FCS_COMPONENT(Sprite);
};

