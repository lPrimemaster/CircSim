#pragma once
#include <typeinfo>
#include <glm/glm.hpp>
#include "../geometry/Component.h"
#include "../util/math.h"

//Wrapper class
class Gate
{
public:
	virtual ~Gate() {  }

	virtual void update(const glm::vec2 A, const glm::vec2 B) = 0;
	virtual void updateInput(const unsigned state) = 0;

	virtual inline glm::vec4 getAB() = 0;

	virtual inline GraphicComponent* getComponent(int index) = 0;

	virtual inline GraphicComponent** getComponentList() = 0;

	virtual inline void changeAlpha(float alpha) = 0;

	virtual inline void changeColor(glm::vec3 color) = 0;

	virtual const inline size_t getComponentListSize() = 0;

	inline const std::type_info& childType() const
	{
		return typeid(*this);
	}

	template<typename Derived>
	inline Derived* convert()
	{
		assert(typeid(Derived) == childType());
		return dynamic_cast<Derived*>(this);
	}

	inline math::BRect getBoundings() const
	{
		return bounding_limits;
	}

	inline math::BRect getTrueBoundings() const
	{
		return true_bounding_limits;
	}

	inline bool& isFixed()
	{
		return fixed;
	}

protected:
	bool fixed = false;
	math::BRect bounding_limits;
	math::BRect true_bounding_limits;
};

