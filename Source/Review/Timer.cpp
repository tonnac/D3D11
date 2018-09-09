#include "Timer.h"
#include "DirectWrite.h"

FLOAT g_fSecPerFrame = 0.0f;

Timer::Timer() : m_fSecPerFrame(0.0f), m_fGameTime(0.0f), m_FPS(0)
{
	ZeroMemory(&m_CurrentTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_BeforeTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OneSecTick, sizeof(LARGE_INTEGER));
	QueryPerformanceFrequency(&m_Frequency);
}

bool Timer::Init()
{
	QueryPerformanceCounter(&m_BeforeTick);

	if (m_BeforeTick.QuadPart == 0)
	{
		return false;
	}

	m_OneSecTick = m_BeforeTick;
	S_Write.AddText(L"FPS", TimeInfo.c_str(), D2D1::RectF(0, 0, 800, 600), D2D1::ColorF::SteelBlue, L"Gabriola", 20);
	S_Write.SetAlignment(L"FPS", DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	S_Write.AddLayout(L"FPS", { 800.0f,600.0f }, { DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7,1 });
	S_Write.SetFont(L"FPS", L"Mistral");
	S_Write.SetWeight(L"FPS", DWRITE_FONT_WEIGHT_HEAVY);
	S_Write.SetFontSize(L"FPS", 50);
	S_Write.SetStyle(L"FPS", DWRITE_FONT_STYLE_ITALIC);
	return true;
}
bool Timer::Frame()
{
	QueryPerformanceCounter(&m_CurrentTick);

	m_fSecPerFrame = g_fSecPerFrame = (m_CurrentTick.LowPart - m_BeforeTick.LowPart) / CASTING(FLOAT,m_Frequency.LowPart);
	m_fGameTime += m_fSecPerFrame;

	if ((m_CurrentTick.LowPart - m_OneSecTick.LowPart) / m_Frequency.LowPart >= 1)
	{


		TimeInfo = L"FPS : " + std::to_tstring(m_FPS) + L" Time : " + std::to_tstring(CASTING(int, m_fGameTime)) + L" SPF : " + std::to_tstring(m_fSecPerFrame);
		S_Write.SetText(L"FPS", TimeInfo);
		m_FPS = 0;
		m_OneSecTick = m_CurrentTick;
	}
	++m_FPS;

	m_BeforeTick = m_CurrentTick;
	return true;
}
bool Timer::Render()
{
	static FLOAT co[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	co[0] += m_fSecPerFrame / 10;
	co[1] += m_fSecPerFrame / 10;
	co[2] += m_fSecPerFrame / 10;

	if (co[0] >= 1.0f) co[0] = rand() % 10000 / 10000.0f;
	if (co[1] >= 1.0f) co[1] = rand() % 10000 / 10000.0f;
	if (co[2] >= 1.0f) co[2] = rand() % 10000 / 10000.0f;

	S_Write.SetColor(L"FPS", D2D1::ColorF(co[0], co[1], co[2], co[3]));
	S_Write.RenderText(L"FPS");
	return true;
}
bool Timer::Release()
{
	return true;
}
std::tstring Timer::getTimeInfo()
{
	return TimeInfo;
}