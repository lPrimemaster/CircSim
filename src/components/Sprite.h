#pragma once
#include <iostream>
#include "../core/ECS.h"
#include "Transform.h"
#include "../assets/Texture.h"
#include "../assets/Registry.h" 

class Sprite : public FCS::Component
{
public:
	enum TextureType
	{
		DIFFUSE = 0, // Always uses texture unit 0
		NORMAL = 1	 // Always uses texture unit 1, and so on...
	};

	inline bool& alphaBlend()
	{
		return enable_blend;
	}

	// TODO: Change this name later
	template<TextureType type>
	inline void addTexture(const std::string& tex_name)
	{
		textures.emplace(type, Registry::GetAsset<Texture>(tex_name));
	}

	template<TextureType type>
	inline void removeTexture()
	{
		textures.erase(type);
	}

	inline void prepare() const
	{
		for (auto t : textures)
		{
			t.second->bind((int)t.first);
		}
	}

	inline void done() const
	{
		for (auto t : textures)
		{
			t.second->unbind((int)t.first);
		}
	}

private:
	bool enable_blend = false;
	std::unordered_map<TextureType, Texture*> textures;

	FCS_COMPONENT(Sprite);
};

