#pragma once
#include <typeinfo>

//Wrapper class
class Gate
{
public:
	virtual ~Gate() {  };

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

