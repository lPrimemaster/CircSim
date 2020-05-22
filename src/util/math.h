#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#include <glm/glm.hpp>
#include <random>
#include "../components/Transform.h"
#include "../components/Collider.h"

#define PI_F 3.141592654f
#define RAD_2_DEG 57.2957795
#define DEG_2_RAD 0.0174532925
#define UP glm::vec3(0.0f, 0.0f, 1.0f)

namespace math
{
	struct pvec2
	{
		float angle;
		float distance;
	};

	template<typename T>
	inline T map(T value, T inMin, T inMax, T outMin, T outMax)
	{
		return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}

	glm::vec2 polarToCartesian(float angle, float distance);
	pvec2 cartesianToPolar(glm::vec2 vector);

	bool isInsideRadius(glm::vec2 point, glm::vec2 center, float radius);


	/*inline std::string generateRandomID() {
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
	}*/

	//template<typename T>
	//inline void removeDuplicates(std::vector<T>& vec)
	//{
	//	//FIX: std::unique assumes the range is already sorted ??
	//	//std::unique assumes the range is already sorted ??
	//	vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
	//}

	namespace internal
	{

		inline bool operator>(const glm::vec2& lhs, const glm::vec2& rhs)
		{
			return (lhs.x > rhs.x && lhs.y > rhs.y);
		}

		inline bool operator<(const glm::vec2& lhs, const glm::vec2& rhs)
		{
			return (lhs.x < rhs.x && lhs.y < rhs.y);
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
	}

	template< typename tPair >
	struct second_t {
		typename tPair::second_type operator()(const tPair& p) const { return p.second; }
	};

	template<typename tMap>
	inline second_t<typename tMap::value_type> gMapSecond(const tMap& m) { return second_t<typename tMap::value_type>(); }

	struct OBB;

	struct AABB
	{
		friend struct OBB;
		friend class Collider;

		AABB(glm::vec2 pivot, glm::vec2 dims)
		{
			this->pivot = pivot;
			dimensions = dims;
		}

		AABB(glm::vec2 pivot, float w, float h) : AABB(pivot, glm::vec2(w, h)) {  }

		inline bool intersect(const glm::vec2& point)
		{
			using namespace internal;
			return ((point > pivot) && (point < pivot + dimensions));
		}

		inline bool intersect(const AABB& aabb)
		{
			if (!(pivot.x > aabb.pivot.x + aabb.dimensions.x || pivot.x + dimensions.x < aabb.pivot.x))
				if (!(pivot.y > aabb.pivot.y + aabb.dimensions.y || pivot.y + dimensions.y < aabb.pivot.y))
					return true;
			return false;
		}

		inline bool intersect(OBB& obb)
		{
			return obb.intersect(*this);
		}

		// Usefull for conversions later
		/*inline Transform& getOBBTransform()
		{
			glm::vec2 hxL = polarToCartesian(angle, xlAxis / 2);
			glm::vec2 hyL = polarToCartesian(90 * DEG_2_RAD + angle, ylAxis / 2);
			glm::vec2 center = pivot + hxL - hyL;

			debug_t.update(center, angle * RAD_2_DEG);
			debug_t.scaleIncrement(glm::vec2(xlAxis / 2, ylAxis / 2));

			return debug_t;
		}*/

	private:
		glm::vec2 pivot;
		glm::vec2 dimensions;
	};

	struct OBB
	{
		friend class Collider;

		OBB(glm::vec2 pivot, glm::vec2 dims, float angle)
		{
			this->pivot = pivot;
			this->angle = angle;
			dimensions = dims;
		}

		OBB(glm::vec2 pivot, float w, float h, float angle) : OBB(pivot, glm::vec2(w, h), angle) {  }

		inline bool intersect(const glm::vec2& point)
		{
			glm::vec2 rxL = polarToCartesian(angle, 1.0f);
			glm::vec2 ryL = polarToCartesian(90 * DEG_2_RAD + angle, 1.0f);

			glm::vec2 v = point - pivot;

			float lxProj = glm::dot(v, rxL);
			float lyProj = -glm::dot(v, ryL);

			if (0.0f < lxProj && lxProj < dimensions.x)
				if (0.0f < lyProj && lyProj < dimensions.y)
					return true;
			return false;
		}

		inline bool intersect(const AABB& aabb)
		{
			return intersect(OBB(aabb.pivot, aabb.dimensions, 0.0f));
		}

		inline bool intersect(const OBB& obb)
		{
			const float pi_2rads = 90 * DEG_2_RAD;

			glm::vec2 xdir = polarToCartesian(angle, 1.0f);
			glm::vec2 ydir = polarToCartesian(pi_2rads + angle, 1.0f);

			auto pset = toPointSet();
			auto pset_other = obb.toPointSet();

			internal::SATExtents yExtents = internal::SATprojectExtents(ydir, pset);
			internal::SATExtents yExtents_other = internal::SATprojectExtents(ydir, pset_other);

			if (!internal::SAToverlap(yExtents, yExtents_other)) return false;

			internal::SATExtents xExtents = internal::SATprojectExtents(xdir, pset);
			internal::SATExtents xExtents_other = internal::SATprojectExtents(xdir, pset_other);

			if (!internal::SAToverlap(xExtents, xExtents_other)) return false;

			//If not pick other OBB directions
			xdir = polarToCartesian(obb.angle, 1.0);
			ydir = polarToCartesian(pi_2rads + obb.angle, 1.0);

			internal::SATExtents yExtents_2 = internal::SATprojectExtents(ydir, pset);
			internal::SATExtents yExtents_other_2 = internal::SATprojectExtents(ydir, pset_other);

			if (!internal::SAToverlap(yExtents_2, yExtents_other_2)) return false;

			internal::SATExtents xExtents_2 = internal::SATprojectExtents(xdir, pset);
			internal::SATExtents xExtents_other_2 = internal::SATprojectExtents(xdir, pset_other);

			//Finally
			return internal::SAToverlap(xExtents_2, xExtents_other_2);
		}

	private:
		inline std::vector<glm::vec2> toPointSet() const
		{
			std::vector<glm::vec2> set;

			glm::vec2 rxL = polarToCartesian(angle, dimensions.x);
			glm::vec2 ryL = polarToCartesian(90 * DEG_2_RAD + angle, dimensions.y);

			set.push_back(pivot);
			set.push_back(pivot + rxL);
			set.push_back(pivot + rxL - ryL);
			set.push_back(pivot - ryL);

			return set;
		}

	private:
		glm::vec2 pivot;
		glm::vec2 dimensions;
		float angle;
	};

	template<typename T>
	inline T swapMax(T last, T current)
	{
		return current > last ? current : last;
	}

	template<typename T>
	inline T swapMin(T last, T current)
	{
		return current < last ? current : last;
	}

	__forceinline void flipImageVertically(unsigned char* image, int w, int h, int bytes_per_pixel)
	{
		int row;
		size_t bytes_per_row = (size_t)w * bytes_per_pixel;
		unsigned char temp[2048];

		for (row = 0; row < (h >> 1); row++) {
			unsigned char* row0 = image + row * bytes_per_row;
			unsigned char* row1 = image + (h - row - 1) * bytes_per_row;
			// swap row0 with row1
			size_t bytes_left = bytes_per_row;
			while (bytes_left) {
				size_t bytes_copy = (bytes_left < sizeof(temp)) ? bytes_left : sizeof(temp);
				memcpy(temp, row0, bytes_copy);
				memcpy(row0, row1, bytes_copy);
				memcpy(row1, temp, bytes_copy);
				row0 += bytes_copy;
				row1 += bytes_copy;
				bytes_left -= bytes_copy;
			}
		}
	}
}