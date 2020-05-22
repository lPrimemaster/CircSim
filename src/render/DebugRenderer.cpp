#include "DebugRenderer.h"
#include "../util/perf_counter.h"
#include "../components/Text.h"
#include "../components/Transform.h"
#include "../util/perf_counter.h"

void DebugRenderer::initialize(FCS::Scene* scene)
{
	debug_pos_text.push_back(scene->instantiate());
	debug_pos_text.push_back(scene->instantiate());

	float scale = 20.0f;

	debug_pos_text[0]->addComponent<Text>();
	auto t = debug_pos_text[0]->addComponent<Transform>();
	t->isOnScreen() = true;
	t->translateIncrement(glm::vec2(0.0f, 700.0f));
	t->scaleIncrement(glm::vec2(0.3f));

	debug_pos_text[1]->addComponent<Text>();
	t = debug_pos_text[1]->addComponent<Transform>();
	t->isOnScreen() = true;
	t->translateIncrement(glm::vec2(0.0f, 700.0f - scale));
	t->scaleIncrement(glm::vec2(0.3f));
}

void DebugRenderer::deinitialize(FCS::Scene* scene)
{
	for (auto e : debug_time_text)
	{
		scene->destroy(e);
	}

	for (auto e : debug_pos_text)
	{
		scene->destroy(e);
	}
}

void DebugRenderer::update(FCS::Scene* scene, float deltaTime)
{
	int diff = frame_debug_text - debug_time_text.size(); // This should not be negative for the meantime... So its ok to do just this!
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
			t->translateIncrement(lpos);
			t->scaleIncrement(glm::vec2(0.3f));
			lpos.y += scale;
			debug_time_text.push_back(e);
		}
	}

	for (int i = 0; i < frame_debug_text; i++)
	{
		debug_record* record = debug_records + i;
		debug_time_text[i]->getComponent<Text>()->formatText("(%d) %-25s: %-15llu cy/h [%d hits]", record->line, record->funcname, record->cycles / (unsigned long long)record->hitCount, record->hitCount);
	}

	frame_debug_text = 0;

	// Show mouse position
	debug_pos_text[0]->getComponent<Text>()->formatText("Mouse Position SS: %.1f %.1f", mouse_position_ss.x, mouse_position_ss.y);
	debug_pos_text[1]->getComponent<Text>()->formatText("Mouse Position WS: %.1f %.1f", mouse_position_ws.x, mouse_position_ws.y); //FIX y coord
}

void DebugRenderer::onEvent(FCS::Scene* scene, const Events::OnPCounterInitDebugInfo& event)
{
	frame_debug_text++;
}

void DebugRenderer::onEvent(FCS::Scene* scene, const Events::OnMouseMovement<Events::MovementType::WorldSpace>& event)
{
	mouse_position_ws = glm::vec2(event.x, event.y);
}

void DebugRenderer::onEvent(FCS::Scene* scene, const Events::OnMouseMovement<Events::MovementType::ScreenSpace>& event)
{
	mouse_position_ss = glm::vec2(event.x, event.y);
}
