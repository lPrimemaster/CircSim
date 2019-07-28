#include "Registry.h"

void Registry::registerAllBasicGeometry()
{
	Geometry::registerGeometry(new Geometry(Geometry::LINES, { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f) }), "Line");
	Geometry::registerGeometry(new Geometry(Geometry::POINTS, { glm::vec2(0.0f, 0.0f) }), "Point");
	Geometry::registerGeometry(new Geometry(Geometry::TRIANGLES, { glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f) }), "Triangle");

	float points_a[200];

	for (int i = 0; i < 100; i++)
	{
		float x = cosf(2 * 3.141592654 / 100.0f * i);
		float y = sinf(2 * 3.141592654 / 100.0f * i);
		points_a[i * 2] = x;
		points_a[i * 2 + 1] = y;
	}

	Geometry::registerGeometry(new Geometry(Geometry::LINE_LOOP, points_a, 200), "Circunference");

	float points_b[202];
	points_b[0] = 0.0f;
	points_b[1] = 0.0f;
	std::copy(&points_a[0], &points_a[199], &points_b[2]);
	points_b[200] = 1.0f;
	points_b[201] = 0.0f;

	Geometry::registerGeometry(new Geometry(Geometry::FAN, points_b, 202), "Circle");

	//Force a small load factor about 5 / 30 ~ 0.167
	Geometry::rehash(30);
	Geometry::getStatistics();
}

void Registry::unregisterAllBasicGeometry()
{
	Geometry::unregisterGeometry("Line");
	Geometry::unregisterGeometry("Point");
	Geometry::unregisterGeometry("Triangle");
	Geometry::unregisterGeometry("Circunference");
	Geometry::unregisterGeometry("Circle");
}