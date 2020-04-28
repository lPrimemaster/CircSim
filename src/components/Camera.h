#pragma once
#include "../core/ECS.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define UP glm::vec3(0.0f, 1.0f, 0.0f)

class Camera : public FCS::Component
{
public:
	inline void view(const glm::vec2& look, const glm::vec3& up = UP)
	{
		glm::vec3 ilook = glm::vec3(look, 10.0f);
		glm::vec3 ilookc = glm::vec3(look, 0.0f);
		vmat = glm::lookAt(ilook, ilookc, up);
	}

	inline void view(const glm::mat4& view)
	{
		vmat = view;
	}

	inline void project(const glm::mat4& proj)
	{
		pmat = proj;
	}

	inline void updateScreen(float w, float h)
	{
		gui_pmat = glm::ortho(0.0f, w, 0.0f, h);
	}

	inline glm::mat4 getPVMatrix()
	{
		return pmat * vmat;
	}

	inline glm::mat4 getGuiMatrix()
	{
		return gui_pmat;
	}

private:
	glm::mat4 vmat;
	glm::mat4 pmat;
	glm::mat4 gui_pmat;

	FCS_COMPONENT(Camera);
};

