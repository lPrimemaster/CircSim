#pragma once
#include <iostream>
#include "../core/ECS.h"
#include "Transform.h"
#include "../assets/Texture.h"
#include "../assets/Registry.h"

enum SpriteBitField
{
	WRAP_MODE_NORMAL = 1 << 0,
	WRAP_MODE_SLICING_3 = 1 << 1,
	WRAP_MODE_SLICING_9 = 1 << 2,
	WRAP_MODE_STRETCH_BOUNDARY = 1 << 3,

	BLEND_ALPHA = 1 << 3
};

inline SpriteBitField operator| (SpriteBitField lhs, SpriteBitField rhs)
{
	typedef typename std::underlying_type<SpriteBitField>::type underlying;
	return static_cast<SpriteBitField>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

inline SpriteBitField operator&(SpriteBitField lhs, SpriteBitField rhs)
{
	typedef typename std::underlying_type<SpriteBitField>::type underlying;
	return static_cast<SpriteBitField>(static_cast<underlying>(lhs)& static_cast<underlying>(rhs));
}

class Sprite : public FCS::Component
{
public:
	enum TextureType
	{
		DIFFUSE = 0, // Always uses texture unit 0
		NORMAL = 1	 // Always uses texture unit 1, and so on...
	};


	inline bool enable(SpriteBitField sbf)
	{
		bool success = false;
		// Wrapping
		if (sbf & WRAP_MODE_NORMAL)
		{
			wrap_mode = 0;
			success = true;
		}
		else if (sbf & WRAP_MODE_SLICING_3)
		{
			wrap_mode = 1;
			success = true;
		}
		else if (sbf & WRAP_MODE_SLICING_9)
		{
			wrap_mode = 2;
			success = true;
		}
		else if (sbf & WRAP_MODE_STRETCH_BOUNDARY)
		{
			wrap_mode = 3;
			success = true;
		}

		// Blending
		if (sbf & BLEND_ALPHA)
		{
			enable_blend = true;
			success = true;
		}

		return success;
	}

	inline bool disable(SpriteBitField sbf)
	{
		// Blending
		if (sbf & BLEND_ALPHA)
		{
			enable_blend = false;
			return true;
		}

		return false;
	}

	inline bool hasAlphaBlend() const
	{
		return enable_blend;
	}

	inline int getWrapMode() const
	{
		return wrap_mode;
	}

	inline bool isAnimated() const
	{
		return is_animated;
	}

	// TODO: Change this name later
	template<TextureType type>
	inline void addTexture(const std::string& tex_name)
	{
		textures[type] = Registry::GetAsset<Texture>(tex_name);

		if (textures[type]->getType() == GL_TEXTURE_2D_ARRAY)
		{
			is_animated = true;
		}
	}

	inline void selectSprite(unsigned int layer)
	{
		if (is_animated)
		{
			enabled_sprite_layer = layer;
		}
	}

	inline unsigned int getSpriteActiveLayer()
	{
		return enabled_sprite_layer;
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
	unsigned int enabled_sprite_layer = 0;
	int wrap_mode = 0;
	bool enable_blend = false;
	bool is_animated = false;
	std::unordered_map<TextureType, Texture*> textures;

	FCS_COMPONENT(Sprite);
};
