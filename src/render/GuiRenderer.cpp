#include "GuiRenderer.h"
#include "../util/math.h"

#define MAT4_ID glm::mat4(1.0f)

#define EXPANDCOLOR(r, g, b, a) r, g, b, a
#define GENERATE10(x) x, x, x, x, x, x, x, x, x, x
#define GENERATE40(x) GENERATE10(x), GENERATE10(x), GENERATE10(x), GENERATE10(x)
#define GET_PAIR(angle, cornerX, cornerY, cornerR) cosf(PI * angle / 180.0f) * cornerR + cornerX, sinf(PI * angle / 180.0f) * cornerR + cornerY

GuiRenderer::GuiRenderer() : Renderer("gui")
{
	text_renderer.setDefaultFont("PantonRustHeavy");
	text_renderer.addText("CSim 1.0", glm::vec2(20.0f, 20.0f), glm::vec3(1.0f), 0.4f);
	unsigned int indices[138] =
	{
		0, 1, 3, 1, 2, 3,
		
		1, 4, 5, 1, 5, 6, 1, 6, 7, 1, 7, 8, 1, 8, 9, 1, 9, 10, 1, 10, 11, 1, 11, 12, 1, 12, 13,
		1, 13, 2, 13, 14, 2,

		2, 14, 15, 2, 15, 16, 2, 16, 17, 2, 17, 18, 2, 18, 19, 2, 19, 20, 2, 20, 21, 2, 21, 22, 2, 22, 23,
		2, 23, 3, 23, 24, 3,

		3, 24, 25, 3, 25, 26, 3, 26, 27, 3, 27, 28, 3, 28, 29, 3, 29, 30, 3, 30, 31, 3, 31, 32, 3, 32, 33,
		3, 33, 0, 33, 34, 0,

		0, 34, 35, 0, 35, 36, 0, 36, 37, 0, 37, 38, 0, 38, 39, 0, 39, 40, 0, 40, 41, 0, 41, 42, 0, 42, 43,
		0, 43, 4, 0, 4, 1
	};

	//Geometry::registerGeometry(new Geometry(Geometry::TRIANGLES, vertices, vertex_size), "guiGeo");
	Geometry::registerGeometry(new Geometry(), "guiGeo");
	Geometry::addBuffer("guiGeo", indices, 138, Geometry::EBO, GL_ELEMENT_ARRAY_BUFFER);

	gui_geom = Geometry::getRegisteredGeometry("guiGeo");
}

GuiRenderer::~GuiRenderer()
{
	Geometry::unregisterGeometry("guiGeo");
	for (auto w : windows)
	{
		//FIX: Clean this up - use unregisterGeometry : which in turn should remove the geometry from the stack, not only free memory
		delete w->geometry;
	}
}

void GuiRenderer::render()
{
	p.bind();
	p.loadMatrix4f("proj", pvm);

	//Think of using Attrib divisor but for now just keep these drawcalls
	for (auto w : windows)
	{
		w->geometry->bind();
		gui_geom->bindBuffer(Geometry::EBO);
		p.loadMatrix4f("model", glm::translate(MAT4_ID, glm::vec3(w->center_pos, 0.0f)));
		glDrawElements(GL_TRIANGLES, 138, GL_UNSIGNED_INT, nullptr);
	}

	text_renderer.render();
}

void GuiRenderer::setPVMatrix(const glm::mat4& pvm)
{
	this->pvm = pvm;
	text_renderer.setPVMatrix(pvm);
}

void GuiRenderer::addWindow(GuiWindow* w)
{
	static unsigned int wID = 0;

	const float cornerX = w->extents.x / 2;
	const float cornerY = w->extents.y / 2;
	const float borderS = w->border_thickness;

	float color_data[176] =
	{
		w->background_color.r, w->background_color.g, w->background_color.b, w->background_alpha, //0
		w->background_color.r, w->background_color.g, w->background_color.b, w->background_alpha, //1
		w->background_color.r, w->background_color.g, w->background_color.b, w->background_alpha, //2
		w->background_color.r, w->background_color.g, w->background_color.b, w->background_alpha, //3
		GENERATE40(EXPANDCOLOR(w->border_color.r, w->border_color.g, w->border_color.b, w->border_alpha))
	};

	float vertex_data[88] =
	{
		-cornerX, -cornerY, //0
		-cornerX,  cornerY, //1
		 cornerX,  cornerY, //2
		 cornerX, -cornerY, //3

		 GET_PAIR(180, -cornerX, cornerY, borderS), // 4
		 GET_PAIR(170, -cornerX, cornerY, borderS), // 5
		 GET_PAIR(160, -cornerX, cornerY, borderS), // 6
		 GET_PAIR(150, -cornerX, cornerY, borderS), // 7
		 GET_PAIR(140, -cornerX, cornerY, borderS), // 8
		 GET_PAIR(130, -cornerX, cornerY, borderS), // 9
		 GET_PAIR(120, -cornerX, cornerY, borderS), //10
		 GET_PAIR(110, -cornerX, cornerY, borderS), //11
		 GET_PAIR(100, -cornerX, cornerY, borderS), //12
		 GET_PAIR( 90, -cornerX, cornerY, borderS), //13

		 GET_PAIR(90, cornerX, cornerY, borderS), //14
		 GET_PAIR(80, cornerX, cornerY, borderS), //15
		 GET_PAIR(70, cornerX, cornerY, borderS), //16
		 GET_PAIR(60, cornerX, cornerY, borderS), //17
		 GET_PAIR(50, cornerX, cornerY, borderS), //18
		 GET_PAIR(40, cornerX, cornerY, borderS), //19
		 GET_PAIR(30, cornerX, cornerY, borderS), //20
		 GET_PAIR(20, cornerX, cornerY, borderS), //21
		 GET_PAIR(10, cornerX, cornerY, borderS), //22
		 GET_PAIR( 0, cornerX, cornerY, borderS), //23

		 GET_PAIR(360,  cornerX, -cornerY, borderS), //24
		 GET_PAIR(350,  cornerX, -cornerY, borderS), //25
		 GET_PAIR(340,  cornerX, -cornerY, borderS), //26
		 GET_PAIR(330,  cornerX, -cornerY, borderS), //27
		 GET_PAIR(320,  cornerX, -cornerY, borderS), //28
		 GET_PAIR(310,  cornerX, -cornerY, borderS), //29
		 GET_PAIR(300,  cornerX, -cornerY, borderS), //30
		 GET_PAIR(290,  cornerX, -cornerY, borderS), //31
		 GET_PAIR(280,  cornerX, -cornerY, borderS), //32
		 GET_PAIR(270,  cornerX, -cornerY, borderS), //33

		 GET_PAIR(270, -cornerX, -cornerY, borderS), //34
		 GET_PAIR(260, -cornerX, -cornerY, borderS), //35
		 GET_PAIR(250, -cornerX, -cornerY, borderS), //36
		 GET_PAIR(240, -cornerX, -cornerY, borderS), //37
		 GET_PAIR(230, -cornerX, -cornerY, borderS), //38
		 GET_PAIR(220, -cornerX, -cornerY, borderS), //39
		 GET_PAIR(210, -cornerX, -cornerY, borderS), //40
		 GET_PAIR(200, -cornerX, -cornerY, borderS), //41
		 GET_PAIR(190, -cornerX, -cornerY, borderS), //42
		 GET_PAIR(180, -cornerX, -cornerY, borderS)  //43
	};

	std::string geo_name = "wGeo_" + std::to_string(wID++);
	Geometry::registerGeometry(new Geometry(Geometry::TRIANGLES, vertex_data, 88), geo_name);
	Geometry::addBuffer(geo_name, color_data, 176, Geometry::VBO1, GL_ARRAY_BUFFER, 1, 4, GL_FLOAT);
	w->geometry = Geometry::getRegisteredGeometry(geo_name);

	//FIX: Don't hardcode this value
	glm::vec2 val = glm::vec2(1280.0f, 720.0f);
	w->locked_positions[0] = val.x - w->center_pos.x; //RIGHT LOCK
	w->locked_positions[1] = val.y - w->center_pos.y; //TOP LOCK

	windows.push_back(w);
}

void GuiRenderer::adjustWindows(LockType lock)
{
	//Keep locked position the same
	for (auto w : windows)
	{
		glm::vec2 viewportSize = glm::vec2(2 / pvm[0][0], 2 / pvm[1][1]);

		if (lock & GUI_LOCK_TOP)
		{
			w->center_pos.y = viewportSize.y - w->locked_positions[1];
		}
		if (lock & GUI_LOCK_RIGHT)
		{
			w->center_pos.x = viewportSize.x - w->locked_positions[0];
		}
	}
}
