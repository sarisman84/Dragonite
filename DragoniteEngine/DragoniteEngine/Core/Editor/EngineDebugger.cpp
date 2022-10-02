#include "EngineDebugger.h"

Dragonite::EngineDebugger::EngineDebugger() : GUIWindow("Engine Debugger")
{
}

void Dragonite::EngineDebugger::OnWindowRender()
{
	ImGui::Text("Frames per second: %.1f", ImGui::GetIO().Framerate);

}

void Dragonite::EngineDebugger::OnEnable()
{

}

void Dragonite::EngineDebugger::OnDisable()
{
}
