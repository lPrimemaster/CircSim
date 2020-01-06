#pragma once
#include "geometry/Geometry.h"
#include "render/texture/Texture.h"
#include "render/chars/CharAtlas.h"

class Registry
{
public:
	static void registerAllBasicGeometry();
	static void unregisterAllBasicGeometry();

	static void registerAllGuiGeometry();
	static void unregisterAllGuiGeometry();

	static void registerAllTextures();
	static void unregisterAllTextures();

	static void registerAllCharAtlas();
	static void unregisterAllCharAtlas();
};

