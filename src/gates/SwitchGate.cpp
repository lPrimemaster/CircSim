#include "SwitchGate.h"
#include "../util/math.h"

SwitchGate::SwitchGate()
{
	defineConnectorDependencies();

	//Defaults
	components[0] = new GraphicComponent("Line"); //Output Line

	components[1] = new GraphicComponent("Line"); //Rect outline Top
	components[2] = new GraphicComponent("Line"); //Rect outline Bottom
	components[3] = new GraphicComponent("Line"); //Rect outline Side
	components[4] = new GraphicComponent("Line"); //Rect outline Side
	components[5] = new GraphicComponent("Rectangle"); //Rectangle

	components[6] = new GraphicComponent("Circle"); //Circle fill state
	components[7] = new GraphicComponent("Circunference"); //Circle outline

	//Create component acnhor points and usable location dependencies based upon the 'center/out' variables
	const float offset = -90.0f;

	glm::vec2 direction = out - center;
	glm::vec2 normalized_direction = direction / sqrt(direction.x * direction.x + direction.y * direction.y);
	glm::vec2 weighted_direction = 0.2f * normalized_direction;
	glm::vec2 normalized_perpendicular = glm::vec2(normalized_direction.y, -normalized_direction.x);
	glm::vec2 weighted_perpendicular = 0.2f * normalized_perpendicular;

	float turn_deg = std::acosf(glm::dot(normalized_direction, glm::vec2(1.0f, 0.0f))) * RAD_2_DEG;

	glm::vec2 right_side = center + weighted_direction;
	glm::vec2 left_side = center - weighted_direction;

	glm::vec2 top_left = left_side + weighted_perpendicular;
	glm::vec2 bottom_left = left_side - weighted_perpendicular;
	glm::vec2 top_right = right_side + weighted_perpendicular;
	glm::vec2 bottom_right = right_side - weighted_perpendicular;

	components[0]->transform().update(right_side, out);

	components[1]->transform().update(top_left, top_right);
	components[2]->transform().update(bottom_left, bottom_right);
	components[3]->transform().update(bottom_left, top_left);
	components[4]->transform().update(bottom_right, top_right);
	components[5]->transform().update(center, turn_deg, Tscale); //Check this
	components[5]->setColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

	components[6]->transform().update(center, 0.0f, Cscale);
	components[6]->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	components[7]->transform().update(center, 0.0f, Cscale);
	components[7]->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	updateConnectors();

	bounding_limits = math::BRect(math::cartesianToPolar(direction).angle, bottom_left, glm::distance(left_side, right_side), 2 * glm::length(weighted_perpendicular));
	true_bounding_limits = math::BRect(math::cartesianToPolar(direction).angle, bottom_left, glm::distance(left_side, out), 2 * glm::length(weighted_perpendicular));
}

SwitchGate::~SwitchGate()
{
	delete components[0];
	delete components[1];
	delete components[2];
	delete components[3];
	delete components[4];
	delete components[5];
	delete components[6];
	delete components[7];

	delete out_ports[0];
	delete in_ports[0];
}

void SwitchGate::update(const glm::vec2 center, const glm::vec2 out)
{
	this->center = center;
	this->out = out;

	const float offset = -90.0f;

	glm::vec2 direction = this->out - this->center;
	glm::vec2 normalized_direction = direction / sqrt(direction.x * direction.x + direction.y * direction.y);
	glm::vec2 weighted_direction = 0.2f * normalized_direction;
	glm::vec2 normalized_perpendicular = glm::vec2(normalized_direction.y, -normalized_direction.x);
	glm::vec2 weighted_perpendicular = 0.2f * normalized_perpendicular;

	float turn_rad = normalized_direction.y > 0 ? std::acosf(glm::dot(normalized_direction, glm::vec2(1.0f, 0.0f))) 
		: -std::acosf(glm::dot(normalized_direction, glm::vec2(1.0f, 0.0f)));

	glm::vec2 right_side = center + weighted_direction;
	glm::vec2 left_side = center - weighted_direction;

	glm::vec2 top_left = left_side + weighted_perpendicular;
	glm::vec2 bottom_left = left_side - weighted_perpendicular;
	glm::vec2 top_right = right_side + weighted_perpendicular;
	glm::vec2 bottom_right = right_side - weighted_perpendicular;

	components[0]->transform().update(right_side, out);

	components[1]->transform().update(top_left, top_right);
	components[2]->transform().update(bottom_left, bottom_right);
	components[3]->transform().update(bottom_left, top_left);
	components[4]->transform().update(bottom_right, top_right);
	components[5]->transform().update(center, turn_rad * RAD_2_DEG, Tscale); //Check this

	components[6]->transform().update(center, 0.0f, Cscale);
	components[7]->transform().update(center, 0.0f, Cscale + 0.002f);

	updateConnectors();

	bounding_limits = math::BRect(turn_rad, bottom_left, glm::distance(left_side, right_side), 2 * glm::length(weighted_perpendicular));
	true_bounding_limits = math::BRect(math::cartesianToPolar(direction).angle, bottom_left, glm::distance(left_side, out), 2 * glm::length(weighted_perpendicular));
}

void SwitchGate::updateInput(const unsigned state)
{
	if (state)
	{
		components[6]->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		components[0]->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		components[6]->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		components[0]->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	//state ? components[6]->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) : components[6]->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}
