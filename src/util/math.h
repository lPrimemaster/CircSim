#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#include <glm/glm.hpp>
#include <random>
#include "../geometry/Transform.h"

#define PI_F 3.141592654f
#define RAD_2_DEG 360.0f / (2 * PI_F)
#define DEG_2_RAD 1.0f / (RAD_2_DEG)
#define UP glm::vec3(0.0f, 0.0f, 1.0f)

template<typename T>
inline std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << i;
	return stream.str();
}

template< typename tPair >
struct second_t {
	typename tPair::second_type operator()(const tPair& p) const { return p.second; }
};

namespace math
{
	struct pvec2
	{
		float angle;
		float distance;
	};

	glm::vec2 screenToWorld(glm::vec2 wdim, glm::vec2 coord, glm::mat4 ipvmat);

	//TODO: Convert this to output ivec2 to avoid problems with equalitites later on
	glm::vec2 snapToGrid(glm::vec2 location, float gridSX, float gridSY);

	glm::vec2 polarToCartesian(float angle, float distance);
	pvec2 cartesianToPolar(glm::vec2 vector);

	bool isInsideRadius(glm::vec2 point, glm::vec2 center, float radius);

	inline std::string generateHEX()
	{
		static int id = 0;
		return int_to_hex(id++);
	}

	inline std::string generateRandomID() {
		static std::random_device dev;
		static std::mt19937 rng(dev());

		std::uniform_int_distribution<int> dist(0, 15);

		const char* v = "0123456789abcdef";
		const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

		std::string res;
		for (int i = 0; i < 16; i++) {
			if (dash[i]) res += "-";
			res += v[dist(rng)];
			res += v[dist(rng)];
		}
		return res;
	}

	template<typename T>
	inline void removeDuplicates(std::vector<T>& vec)
	{
		//FIX: std::unique assumes the range is already sorted ??
		//std::unique assumes the range is already sorted ??
		vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
	}

	struct SATExtents
	{
#ifdef max
#undef max
#endif
		float min = std::numeric_limits<float>::max();
		float max = std::numeric_limits<float>::lowest();
	};

	inline SATExtents SATprojectExtents(glm::vec2 axis, std::vector<glm::vec2> set)
	{
		SATExtents extents;
		for (auto point : set)
		{
			float proj = glm::dot(point, axis);
			extents.min = (proj < extents.min ? proj : extents.min);
			extents.max = (proj > extents.max ? proj : extents.max);
		}
		return extents;
	}
	
	inline bool isBetweenOrdered(float val, float lowerBound, float upperBound)
	{
		return lowerBound < val && val < upperBound;
	}

	inline bool SAToverlap(SATExtents first, SATExtents second)
	{
		return isBetweenOrdered(second.min, first.min, first.max) || isBetweenOrdered(first.min, second.min, second.max);
	}

	template<typename tMap>
	inline second_t<typename tMap::value_type> gMapSecond(const tMap& m) { return second_t<typename tMap::value_type>(); }

	struct Bounding
	{
		virtual inline bool intersect(const glm::vec2& point) = 0;
	};

	struct BRect : public Bounding
	{
		BRect() { pivot = glm::vec2(0.0f); angle = 0.0f; xlAxis = 0.0f; ylAxis = 0.0f; }

		BRect(float angle, glm::vec2 pivot, float xLength, float yLength)
		{
			this->angle = angle;
			this->pivot = pivot;
			xlAxis = xLength;
			ylAxis = yLength;
		}

	private:
		glm::vec2 pivot;
		float angle;
		float xlAxis;
		float ylAxis;

	private:
		::Transform debug_t;

	public:
		inline bool intersect(const glm::vec2& point) override
		{
			glm::vec2 rxL = polarToCartesian(angle, 1.0);
			glm::vec2 ryL = polarToCartesian(90 * DEG_2_RAD + angle , 1.0);

			glm::vec2 v = point - pivot;

			float lxProj =  glm::dot(v, rxL);
			float lyProj = -glm::dot(v, ryL);

			if (0.0f < lxProj && lxProj < xlAxis)
			{
				if (0.0f < lyProj && lyProj < ylAxis)
				{
					return true;
				}
			}
			return false;
		}

		//FIX: 90d angle error on intersect
		inline bool intersect(const BRect& other)
		{
			const float pi_2rads = 90 * DEG_2_RAD;

			glm::vec2 xdir = polarToCartesian(angle, 1.0);
			glm::vec2 ydir = polarToCartesian(pi_2rads + angle, 1.0);

			auto pset = toPointSet();
			auto pset_other = other.toPointSet();

			SATExtents yExtents = SATprojectExtents(ydir, pset);
			SATExtents yExtents_other = SATprojectExtents(ydir, pset_other);

			if (!SAToverlap(yExtents, yExtents_other)) return false;

			SATExtents xExtents = SATprojectExtents(xdir, pset);
			SATExtents xExtents_other = SATprojectExtents(xdir, pset_other);

			if (!SAToverlap(xExtents, xExtents_other)) return false;

			//If not pick other's directions
			xdir = polarToCartesian(other.angle, 1.0);
			ydir = polarToCartesian(pi_2rads + other.angle, 1.0);

			SATExtents yExtents_2 = SATprojectExtents(ydir, pset);
			SATExtents yExtents_other_2 = SATprojectExtents(ydir, pset_other);

			if (!SAToverlap(yExtents_2, yExtents_other_2)) return false;

			SATExtents xExtents_2 = SATprojectExtents(xdir, pset);
			SATExtents xExtents_other_2 = SATprojectExtents(xdir, pset_other);

			//Finally
			return SAToverlap(xExtents_2, xExtents_other_2);
		}

		inline std::vector<glm::vec2> toPointSet() const
		{
			std::vector<glm::vec2> set;

			glm::vec2 rxL = polarToCartesian(angle, xlAxis);
			glm::vec2 ryL = polarToCartesian(90 * DEG_2_RAD + angle, ylAxis);

			set.push_back(pivot);
			set.push_back(pivot + rxL);
			set.push_back(pivot + rxL - ryL);
			set.push_back(pivot - ryL);

			return set;
		}

		inline Transform& getOBBTransform()
		{
			glm::vec2 hxL = polarToCartesian(angle, xlAxis / 2);
			glm::vec2 hyL = polarToCartesian(90 * DEG_2_RAD + angle, ylAxis / 2);
			glm::vec2 center = pivot + hxL - hyL;

			debug_t.update(center, angle * RAD_2_DEG);
			debug_t.scale(glm::vec2(xlAxis / 2, ylAxis / 2));

			return debug_t;
		}
	};
}