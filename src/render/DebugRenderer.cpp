#include "DebugRenderer.h"
#include "../util/perf_counter.h"
#include "../components/Text.h"
#include "../components/Transform.h"
#include "../util/perf_counter.h"

void DebugRenderer::initialize(FCS::Scene* scene)
{
}

void DebugRenderer::deinitialize(FCS::Scene* scene)
{
}

void DebugRenderer::update(FCS::Scene* scene, float deltaTime)
{
	int diff = frame_debug_text - debug_text.size(); // This should not be negative for the meantime... So its ok to do just this!
	if (diff > 0)
	{
		glm::vec2 lpos = glm::vec2(0.0f);
		float scale = 20.0f;
		for (int i = 0; i < diff; i++)
		{
			auto e = scene->instantiate();
			e->addComponent<Text>();
			auto t = e->addComponent<Transform>();
			t->isOnScreen() = true;
			t->translate(lpos);
			t->scale(glm::vec2(0.3f));
			lpos.y += scale;
			debug_text.push_back(e);
		}
	}

	for (int i = 0; i < frame_debug_text; i++)
	{
		debug_record* record = debug_records + i;
		debug_text[i]->getComponent<Text>()->formatText("(%d) %-25s: %-15llu cy/h [%d hits]", record->line, record->funcname, record->cycles / (unsigned long long)record->hitCount, record->hitCount);
	}

	frame_debug_text = 0;
}

void DebugRenderer::onEvent(FCS::Scene* scene, const Events::OnPCounterInitDebugInfo& event)
{
	frame_debug_text++;
}
