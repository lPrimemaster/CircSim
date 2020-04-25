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
	UnregisterAllAssets();
}

void Registry::registerAllBasicGeometry()
{
	std::vector<glm::vec2> p = { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f) };
	RegisterAsset<Geometry>("Line", Geometry::LINES, p);

	p = { glm::vec2(0.0f, 0.0f) };
	RegisterAsset<Geometry>("Point", Geometry::POINTS, p);

	p = { glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
	RegisterAsset<Geometry>("Triangle", Geometry::TRIANGLES, p);
	
	float points_a[200];
	for (int i = 0; i < 100; i++)
	{
		float x = cosf(2 * 3.141592654f / 100.0f * i);
		float y = sinf(2 * 3.141592654f / 100.0f * i);
		points_a[i * 2] = x;
		points_a[i * 2 + 1] = y;
	}

	RegisterAsset<Geometry>("Circunference", Geometry::LINE_LOOP, points_a, 200);

	float points_b[202];
	points_b[0] = 0.0f;
	points_b[1] = 0.0f;
	std::copy(&points_a[0], &points_a[199], &points_b[2]);
	points_b[200] = 1.0f;
	points_b[201] = 0.0f;

	RegisterAsset<Geometry>("Circle", Geometry::FAN, points_b, 202);


	float points_c[12];
	points_c[0] = -1.0f; points_c[1] = 1.0f;
	points_c[2] = 1.0f; points_c[3] = 1.0f;
	points_c[4] = 1.0f; points_c[5] = -1.0f;
	points_c[6] = 1.0f; points_c[7] = -1.0f;
	points_c[8] = -1.0f; points_c[9] = -1.0f;
	points_c[10] = -1.0f; points_c[11] = 1.0f;

	RegisterAsset<Geometry>("Rectangle", Geometry::TRIANGLES, points_c, 12);
	Geometry* rect = GetAsset<Geometry>("Rectangle");

	float texCoords[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	rect->addBuffer(texCoords, 12, Geometry::BufferType::VBO1, GL_ARRAY_BUFFER, 1, 2, GL_FLOAT);

	RegisterAsset<Geometry>("TextRenderGeometry", Geometry::TRIANGLES, nullptr, 1, 4); // Buffer will be resized later GL_DYNAMIC_DRAW
	Geometry* trg = GetAsset<Geometry>("TextRenderGeometry");
	trg->addBuffer<float>(nullptr, 1, Geometry::BufferType::VBO1, GL_ARRAY_BUFFER, 1, 3, GL_FLOAT, 6); //Divisor 6 for the color attr
}

void Registry::registerAllGuiGeometry()
{
}

void Registry::registerAllTextures()
{
	RegisterAsset<Texture>("test_texture", GL_TEXTURE_2D, "test.png");
}

void Registry::registerAllCharAtlas()
{
	RegisterAsset<Charmap>("test_cmap", "assets/fonts/LucidaConsole.ttf");
}

