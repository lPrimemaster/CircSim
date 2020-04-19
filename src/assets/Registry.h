#pragma once
#include "../core/ECS.h"
#include "Asset.h"
#include "Geometry.h"
#include "Texture.h"
#include <unordered_map>
#include <vector>
//#include "render/texture/Texture.h"
//#include "render/chars/CharAtlas.h"

class Registry
{
public:
	Registry();
	~Registry();

private:
	static void registerAllBasicGeometry();
	static void registerAllGuiGeometry();
	static void registerAllTextures();

public:
	template<typename A, typename... Args>
	inline static void RegisterAsset(const std::string& asset_name, Args&&... args);
	template<typename A>
	inline static void UnregisterAsset(const std::string& asset_name);
	inline static void UnregisterAllAssets();

	template<typename A>
	inline static A* GetAsset(const std::string& asset_name);

	/*static void registerAllCharAtlas();
	static void unregisterAllCharAtlas();*/

private:
	inline static std::unordered_map<std::string, std::pair<Asset*, std::type_index>> assets;
};

template<typename A, typename... Args>
inline void Registry::RegisterAsset(const std::string& asset_name, Args&&... args)
{
	// First check if the asset already exists
	auto found = assets.find(asset_name);
	if (found == assets.end())
	{
		// Not found or duplicate chosen name - Create the asset
		A* asset = new A(args...);

		// Just like that
		assets.emplace(asset_name, std::make_pair(asset, FCS::getType<A>()));
	}
}

template<typename A>
inline void Registry::UnregisterAsset(const std::string& asset_name)
{
	auto found = assets.find(asset_name);
	if (found != assets.end())
	{
		// Found! Call dtor
		delete found->second.first;
		assets.erase(asset_name);
	}
}

template<typename A>
inline A* Registry::GetAsset(const std::string& asset_name)
{
	return dynamic_cast<A*>(assets.find(asset_name)->second.first);
}

inline void Registry::UnregisterAllAssets()
{
	for (auto a : assets)
	{
		delete a.second.first;
	}
	assets.clear();
}
