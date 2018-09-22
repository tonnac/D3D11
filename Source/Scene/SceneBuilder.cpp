#include "SceneBuilder.h"


bool SceneBuilder::Initialize(const std::tstring& ScriptPath)
{
	FileExceptErr(m_Script, ScriptPath);
}