#include "Registry.h"

Registry::Registry()
{
	registerAllBasicGeometry();
	registerAllGuiGeometry();
	registerAllTextures();
	registerAllCharAtlas();
}

Registry::~Registry()
{
	unregisterAllBasicGeometry();
	unregisterAllGuiGeometry();
	unregisterAllTextures();
	unregisterAllCharAtlas();
}

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


	float points_c[12];
	points_c[0] = -1.0f; points_c[1] = 1.0f;
	points_c[2] = 1.0f; points_c[3] = 1.0f;
	points_c[4] = 1.0f; points_c[5] = -1.0f;
	points_c[6] = 1.0f; points_c[7] = -1.0f;
	points_c[8] = -1.0f; points_c[9] = -1.0f;
	points_c[10] = -1.0f; points_c[11] = 1.0f;

	Geometry::registerGeometry(new Geometry(Geometry::TRIANGLES, points_c, 12), "Rectangle");

	//Force a small load factor about 6 / 36 ~ 0.167
	Geometry::rehash(36);
}

void Registry::unregisterAllBasicGeometry()
{
	Geometry::unregisterGeometry("Line");
	Geometry::unregisterGeometry("Point");
	Geometry::unregisterGeometry("Triangle");
	Geometry::unregisterGeometry("Circunference");
	Geometry::unregisterGeometry("Circle");
	Geometry::unregisterGeometry("Rectangle");
}

void Registry::registerAllGuiGeometry()
{
}

void Registry::unregisterAllGuiGeometry()
{
}

void Registry::registerAllTextures()
{

}

void Registry::unregisterAllTextures()
{
}

void Registry::registerAllCharAtlas()
{
	CharAtlas::registerCharAtlas(new CharAtlas("assets/fonts/PantonRustHeavy.ttf"), "PantonRustHeavy");
	CharAtlas::registerCharAtlas(new CharAtlas("assets/fonts/LucidaConsole.ttf"), "LucidaConsole");
}

void Registry::unregisterAllCharAtlas()
{
	CharAtlas::unregisterCharAtlas("PantonRustHeavy");
	CharAtlas::unregisterCharAtlas("LucidaConsole");
}
