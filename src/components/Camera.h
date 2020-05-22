#pragma once
#include "../core/ECS.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define UP glm::vec3(0.0f, 1.0f, 0.0f)

class Camera : public FCS::Component
{
public:
	inline void view2DInstant(const glm::vec2& translation, const float& scale)
	{
		this->translation = translation;
		this->scale = glm::vec2(scale, scale * ar);

		pvmat = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale * ar, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(translation, 0.0f));
	}

	inline void view2DInstantTranslation(const glm::vec2& translation)
	{
		this->translation = translation;

		pvmat = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(this->translation, 0.0f));
	}

	inline void view2DInstantScale(const float& scale)
	{
		this->scale = glm::vec2(scale, scale * ar);

		pvmat = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(this->translation, 0.0f));
	}

	inline void view2DIncrement(const glm::vec2& translation, const float& scale)
	{
		this->translation += translation;
		this->scale += glm::vec2(scale, scale * ar);
		
		pvmat = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(this->translation, 0.0f));
	}

	inline void view2DIncrementTranslation(const glm::vec2& translation)
	{
		this->translation += translation;

		pvmat = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(this->translation, 0.0f));
	}

	inline void view2DIncrementScale(const float& scale)
	{
		this->scale += glm::vec2(scale, scale * ar);

		pvmat = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale, 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(this->translation, 0.0f));
	}

	inline void view2D(const glm::mat4& matrix)
	{
		pvmat = matrix;
	}

	inline void updateScreen(float w, float h)
	{
		screenW = w;
		screenH = h;
		ar = screenW / screenH;
		gui_pmat = glm::ortho(0.0f, w, 0.0f, h);
	}

	inline glm::mat4 getPVMatrix()
	{
		return pvmat;
	}

	inline glm::mat4 getGuiMatrix()
	{
		return gui_pmat;
	}

	inline glm::vec2 screenToWorld(float x, float y)
	{
		float ndc_x = (x / screenW) * 2.0f - 1.0f;
		float ndc_y = (y / screenH) * 2.0f - 1.0f;
		
		//glm::vec4 coords = glm::inverse(pvmat) * glm::vec4(ndc_x, ndc_y, 0.0f, 1.0f);
		glm::vec4 coords = inversePVM() * glm::vec4(ndc_x, -ndc_y, 0.0f, 1.0f);

		return glm::vec2(coords);
	}

private:
	__forceinline glm::mat4 inversePVM()
	{
		return glm::translate(glm::mat4(1.0f), glm::vec3(-this->translation, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3( 1.0f / this->scale, 0.0f));
	}

private:
	glm::mat4 pvmat;
	glm::mat4 gui_pmat;

	glm::vec2 translation;
	glm::vec2 scale;
	float ar;

	float screenW = 0.0f;
	float screenH = 0.0f;

	FCS_COMPONENT(Camera);
};

