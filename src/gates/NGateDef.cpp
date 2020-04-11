#include "NGateDef.h"

NotGate::NotGate()
{
	defineConnectorDependencies();

	//Defaults
	components[0] = new Component("Line"); //Input  Line
	components[1] = new Component("Line"); //Output Line

	components[2] = new Component("Triangle"); //Body Triangle
	components[3] = new Component("Line"); //Triangle outline Side
	components[4] = new Component("Line"); //Triangle outline Top
	components[5] = new Component("Line"); //Triangle outline Bottom

	components[6] = new Component("Circle"); //Circle fill state
	components[7] = new Component("Circunference"); //Circle outline

	//Create component acnhor points and usable location dependencies based upon the 'in/out' variables
	const float offset = -90.0f;

	glm::vec2 direction = out - in;
	glm::vec2 normalized_direction = direction / sqrt(direction.x * direction.x + direction.y * direction.y);
	glm::vec2 weighted_direction = 0.2f * normalized_direction;
	glm::vec2 normalized_perpendicular = glm::vec2(normalized_direction.y, -normalized_direction.x);
	glm::vec2 weighted_perpendicular = 0.2f * normalized_perpendicular;

	glm::vec2 tCenter = getTriangleCenter();

	glm::vec2 in_line_ending = tCenter - weighted_direction;
	glm::vec2 out_line_start = tCenter + weighted_direction;

	components[0]->transform().update(in, in_line_ending);
	components[1]->transform().update(out_line_start, out);

	components[2]->transform().update(in_line_ending, out_line_start, Tscale);
	components[2]->transform().rotate(offset);
	components[2]->transform().scale(glm::vec2(0.5f, 1.0f));
	components[2]->setColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	components[3]->transform().update(in_line_ending + weighted_perpendicular, out_line_start);
	components[4]->transform().update(in_line_ending - weighted_perpendicular, out_line_start);
	components[5]->transform().update(in_line_ending - weighted_perpendicular, in_line_ending + weighted_perpendicular);

	components[6]->transform().update(out_line_start, 0.0f, Cscale);
	components[6]->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	components[7]->transform().update(out_line_start, 0.0f, Cscale);
	components[7]->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	updateConnectors();

	bounding_limits = math::BRect(math::cartesianToPolar(direction).angle, in_line_ending - weighted_perpendicular, glm::distance(in_line_ending, out_line_start + Cscale * normalized_direction), 2 * glm::length(weighted_perpendicular));
	true_bounding_limits = math::BRect(math::cartesianToPolar(direction).angle, in - weighted_perpendicular, glm::distance(in, out), 2 * glm::length(weighted_perpendicular));
	
}

NotGate::~NotGate()
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

void NotGate::update(const glm::vec2 in, const glm::vec2 out)
{
	this->in = in;
	this->out = out;

	const float offset = -90.0f;

	glm::vec2 direction = this->out - this->in;
	glm::vec2 normalized_direction = direction / sqrt(direction.x * direction.x + direction.y * direction.y);
	glm::vec2 weighted_direction = 0.2f * normalized_direction;
	glm::vec2 normalized_perpendicular = glm::vec2(normalized_direction.y, -normalized_direction.x);
	glm::vec2 weighted_perpendicular = 0.2f * normalized_perpendicular;

	glm::vec2 tCenter = getTriangleCenter();

	glm::vec2 in_line_ending = tCenter - weighted_direction;
	glm::vec2 out_line_start = tCenter + weighted_direction;

	components[0]->transform().update(this->in, in_line_ending);
	components[1]->transform().update(out_line_start, this->out);

	components[2]->transform().update(in_line_ending, out_line_start, Tscale);
	components[2]->transform().rotate(offset);
	components[2]->transform().scale(glm::vec2(0.5f, 1.0f));

	components[3]->transform().update(in_line_ending + weighted_perpendicular, out_line_start);
	components[4]->transform().update(in_line_ending - weighted_perpendicular, out_line_start);
	components[5]->transform().update(in_line_ending - weighted_perpendicular, in_line_ending + weighted_perpendicular);

	components[6]->transform().update(out_line_start, 0.0f, Cscale);

	components[7]->transform().update(out_line_start, 0.0f, Cscale + 0.002f);

	updateConnectors();

	bounding_limits = math::BRect(math::cartesianToPolar(direction).angle, in_line_ending - weighted_perpendicular, glm::distance(in_line_ending, out_line_start + Cscale * normalized_direction), 2 * glm::length(weighted_perpendicular));
	true_bounding_limits = math::BRect(math::cartesianToPolar(direction).angle, in - weighted_perpendicular, glm::distance(in, out), 2 * glm::length(weighted_perpendicular));
}

void NotGate::updateInput(const unsigned state)
{
	if (state)
	{
		components[6]->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		components[0]->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//components[1]->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		components[1]->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		components[6]->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//components[0]->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		components[0]->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		components[1]->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	//state ? components[6]->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) : components[6]->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

glm::vec2 NotGate::getTriangleCenter()
{
	return (out + in) / 2.0f;
}
