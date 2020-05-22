#pragma once
#include <iostream>

#include "../core/ECS.h"
#include "../assets/Geometry.h"
#include "../components/Material.h"

#include "../assets/Registry.h"

#include "../components/Camera.h"


class GridRenderer : public FCS::System<FCS::Event::EntityCreated>
{
public:
	void initialize(FCS::Scene* scene) override;
	void deinitialize(FCS::Scene* scene) override;

	void update(FCS::Scene* scene, float deltaTime) override;

	void onEvent(FCS::Scene* scene, const FCS::Event::EntityCreated& event) override;

private:
	void updateGrid(float zscale, glm::vec2 tvec, glm::vec2 sdim);

private:
	Geometry* grid;
	Material* mat;
	float lw = 0.005f;

	float* offsets = nullptr;
	unsigned totalGenLines = 20; //Ensure 20 lines at least

private:
	FCS_SYSTEM(GridRenderer, 98);
};
