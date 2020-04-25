#pragma once
#include "GLWrapper.h"
#include "../assets/Registry.h"
#include "../assets/Geometry.h"
#include "../components/Material.h"
#include "../components/Text.h"
#include "../components/Transform.h"
#include "../components/Camera.h"

class TextRenderer : public FCS::System<FCS::Event::EntityCreated>
{
public:
	void initialize(FCS::Scene* scene) override;
	void deinitialize(FCS::Scene* scene) override;

	void update(FCS::Scene* scene, float deltaTime) override;

	void onEvent(FCS::Scene* scene, const FCS::Event::EntityCreated& event) override; //TODO: Maybe add onEvent textModified ???

private:
	Geometry* geo = nullptr;
	Material* mat = nullptr;

	FCS_SYSTEM(TextRenderer, 1);
};

