#pragma once
#include "Core.h"

struct Output
{
	DXGI_OUTPUT_DESC desc;
	std::vector<DXGI_MODE_DESC> m_ModList;
	std::vector<IDXGIOutput*> m_OutputList;
};

struct Adapter
{
	IDXGIAdapter* pAdapter;
	DXGI_ADAPTER_DESC desc;
	std::vector<Output> m_OutputList;
};

class Sample : public Core
{
public:
	Sample();
public:
	bool		PreInit();
	bool		Init() override;
	bool		Render() override;
	bool		Release() override;
private:
	std::vector<Adapter> m_AdapterList;
};