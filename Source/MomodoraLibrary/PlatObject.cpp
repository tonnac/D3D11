#include "PlatObject.h"
#include "Rendering.h"

bool PlatObject::Init()
{
	Object::Init();
	return true;
}
bool PlatObject::Frame()
{
	Object::Frame();
	return true;
}
bool PlatObject::Render()
{
	Object::Render();
	return true;
}
bool PlatObject::Release()
{
	if (m_pRendering)
	{
		m_pRendering->Release();
		delete m_pRendering;
	}
	m_pRendering = nullptr;
	return true;
}