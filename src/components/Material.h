#pragma once
#include "../core/ECS.h"
#include "../render/Program.h"

typedef glm::vec4 Color;

class Material : public FCS::Component
{
public:
	inline void setShader(const std::string& shader, bitfield flags)
	{
		auto found = registered_programs.find(shader);
		if (found == registered_programs.end())
		{
			p = new Program(shader, flags);
			registered_programs.try_emplace(shader, p);
		}
		else
		{
			p = found->second;
		}
	}

	inline Program* getProgram()
	{
		return p;
	}

	inline Color& getColor()
	{
		return color;
	}

	inline void setColor(Color c)
	{
		color = c;
	}

	inline static void DeleteMaterials()
	{
		for (auto p : registered_programs)
		{
			delete p.second;
		}
		registered_programs.clear();
	}

private:
	Color color = Color(1.0f);
	std::string name;
	Program* p = nullptr;

private:
	inline static std::unordered_map<std::string, Program*> registered_programs;

	FCS_COMPONENT(Material);
};

