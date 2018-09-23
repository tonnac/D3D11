#include "SceneBuilder.h"


void SceneBuilder::Initialize(const std::tstring& ScriptPath)
{
	FileExceptErr(m_Script, ScriptPath);
}
Scene*	SceneBuilder::CreateScene(std::tstring& Scenename, bool isPrevScene)
{

	return nullptr;
}