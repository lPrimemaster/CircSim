#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "../components/Sprite.h"
#include "../components/Transform.h"
#include "../components/Material.h"
#include "../components/Camera.h"
#include "../assets/Registry.h"

class SpriteRenderer : public FCS::System<FCS::Event::EntityCreated>
{
public:
	void initialize(FCS::Scene* scene) override;
	void deinitialize(FCS::Scene* scene) override;

	void update(FCS::Scene* scene, float deltaTime) override;

	void onEvent(FCS::Scene* scene, const FCS::Event::EntityCreated& event) override;

private:
	Geometry* quad;

	FCS_SYSTEM(SpriteRenderer, 2); // Highest priority - last call
};
