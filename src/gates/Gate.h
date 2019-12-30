#pragma once
#include <typeinfo>
#include <glm/glm.hpp>
#include "../geometry/Component.h"

//Wrapper class
class Gate
{
public:
	virtual ~Gate() {  };

	virtual void update(const glm::vec2 A, const glm::vec2 B) = 0;
	virtual void updateInput(const unsigned state) = 0;

	virtual inline Component* getComponent(int index) = 0;

	virtual inline Component** getComponentList() = 0;

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
};

