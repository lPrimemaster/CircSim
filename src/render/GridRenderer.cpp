#include "GridRenderer.h"
#include "../util/perf_counter.h"

//TODO : Adding other shader stages should be automated by searching in the directory for extensions
//TODO : Adding pre shader calculation to reduce cpu memory usage -> redundant values
//GridRenderer::GridRenderer()
//{
//	generateGrid();
//	offsets = (float*)malloc(sizeof(float) * totalGenLines * 4); //C type cast
//}
//
//GridRenderer::~GridRenderer()
//{
//	Geometry::unregisterGeometry("Grid");
//	free(offsets);
//}
//
//void GridRenderer::render()
//{
//	p.bind();
//	p.loadMatrix4f("PView", pvm);
//	p.loadFloat("lineWidth", lw);
//	glDisable(GL_BLEND);
//	p.loadVector4f("color", glm::vec4(0.2f));
//	static Geometry* g = Geometry::getRegisteredGeometry("Grid"); //Avoid multiple calls
//	g->bind();
//	glDrawArrays(GL_LINES, 0, totalGenLines * 2);
//	glEnable(GL_BLEND);
//}
//
//float& GridRenderer::getLW()
//{
//	return lw;
//}

void GridRenderer::initialize(FCS::Scene* scene)
{
	Registry::RegisterAsset<Geometry>("Grid", Geometry::LINES, nullptr, 100);
	grid = Registry::GetAsset<Geometry>("Grid");
	mat = new Material();
	mat->setShader("aa_lines", ADD_GEOM_SHADER);
}

void GridRenderer::deinitialize(FCS::Scene* scene)
{
	delete mat;
}

void GridRenderer::update(FCS::Scene* scene, float deltaTime)
{
	TIMED_BLOCK;
	auto p = mat->getProgram();
	auto pvmatrix = scene->getAllWith<Camera>()[0]->getComponent<Camera>()->getPVMatrix();

	p->bind();
	p->loadMatrix4f("PView", pvmatrix);
	p->loadFloat("lineWidth", lw);
	p->loadVector4f("color", glm::vec4(0.2f));
	glDisable(GL_BLEND);

	grid->bind();
	glDrawArrays(GL_LINES, 0, totalGenLines * 2);
}

void GridRenderer::onEvent(FCS::Scene* scene, const FCS::Event::EntityCreated& event)
{
}

void GridRenderer::updateGrid(float zscale, glm::vec2 tvec, glm::vec2 sdim)
{
	float aspect_ratio = sdim.x / sdim.y;

	float wspace_width = 2.0f + zscale * 2.0f;
	float wspace_height = wspace_width / aspect_ratio;

	float half_width = wspace_width / 2.0f;
	float half_height = wspace_height / 2.0f;

	float barrierXLow = tvec.x - half_width;
	float barrierYLow = tvec.y - half_height;
	float barrierXHigh = tvec.x + half_width;
	float barrierYHigh = tvec.y + half_height;

	unsigned gridXCardinality = (unsigned)ceilf(wspace_width * 5.0f) + 1;
	unsigned gridYCardinality = (unsigned)ceilf(wspace_height * 5.0f) + 1;

	//This does not take into account the scaleV factor grid dimming / vanishing effect
	totalGenLines = gridXCardinality + gridYCardinality;

	/* Calculate where the lines should be on the screen -> minimizing rendering */
	/* Find the nearest 0.2 multiple on the borders */

	float ubarrierXLow = floorf(barrierXLow);
	float ubarrierYLow = floorf(barrierYLow);
	float ubarrierXHigh = floorf(barrierXHigh);
	float ubarrierYHigh = floorf(barrierYHigh);

	float decimalBarrierLX = barrierXLow - ubarrierXLow;
	float decimalBarrierLY = barrierYLow - ubarrierYLow;

	float decimalBarrierHX = barrierXHigh - ubarrierXHigh;
	float decimalBarrierHY = barrierYHigh - ubarrierYHigh;

	float nearestLowX = (float)((int)ceilf(decimalBarrierLX * 5) / 5.0f) + ubarrierXLow;
	float nearestLowY = (float)((int)ceilf(decimalBarrierLY * 5) / 5.0f) + ubarrierYLow;

	float nearestHighX = (float)((int)floorf(decimalBarrierHX * 5) / 5.0f) + ubarrierXHigh;
	float nearestHighY = (float)((int)floorf(decimalBarrierHY * 5) / 5.0f) + ubarrierYHigh;


	std::vector<float> offsets;
	offsets.resize(totalGenLines * 4);

	for (int i = 0; i < gridXCardinality; i++)
	{
		float xval = (float)i * 0.2f + nearestLowX;
		offsets[4 * i] = xval;
		offsets[4 * i + 1] = barrierYHigh;
		offsets[4 * i + 2] = xval;
		offsets[4 * i + 3] = barrierYLow;
	}

	for (int i = 0; i < gridYCardinality; i++)
	{
		float yval = (float)i * 0.2f + nearestLowY;
		offsets[4 * (i + gridXCardinality)] = barrierXLow;
		offsets[4 * (i + gridXCardinality) + 1] = yval;
		offsets[4 * (i + gridXCardinality) + 2] = barrierXHigh;
		offsets[4 * (i + gridXCardinality) + 3] = yval;
	}

	grid->updateDynamicGeometry(offsets);
}
