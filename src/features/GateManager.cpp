#include "GateManager.h"

std::vector<Gate*> GateManager::global_gate_list;

void GateManager::updateAny(Gate* gate, glm::vec2 translation)
{
	const std::type_info& ti = gate->childType();
	if (ti == typeid(NotGate))
	{
		NotGate* ng = gate->convert<NotGate>();
		glm::vec4 old_translation = ng->getAB();
		ng->update(glm::vec2(old_translation.x, old_translation.y) + translation, glm::vec2(old_translation.z, old_translation.w) + translation);
	}
	else if (ti == typeid(SwitchGate))
	{
		SwitchGate* sg = gate->convert<SwitchGate>();
		glm::vec4 old_translation = sg->getAB();
		sg->update(glm::vec2(old_translation.x, old_translation.y) + translation, glm::vec2(old_translation.z, old_translation.w) + translation);
	}
}
