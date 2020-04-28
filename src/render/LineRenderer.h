#pragma once

#include "../components/Line.h"
#include "../components/Transform.h"
#include "../components/Material.h"
#include "../components/Camera.h"
#include "../assets/Registry.h"

class LineRenderer : public FCS::System<FCS::Event::EntityCreated>
{
public:
	void initialize(FCS::Scene* scene) override;
	void deinitialize(FCS::Scene* scene) override;

	void update(FCS::Scene* scene, float deltaTime) override;

	void onEvent(FCS::Scene* scene, const FCS::Event::EntityCreated& event) override;

private:
	Geometry* line;
	Material* mat;

	FCS_SYSTEM(LineRenderer, 1);
};

