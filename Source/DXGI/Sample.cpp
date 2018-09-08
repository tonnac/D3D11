#include "Sample.h"

Sample::Sample()
{
}

bool Sample::PreInit()
{
	Device::CreateGIFactory();
	IDXGIFactory* pFactory = getGIFactory();
	int iAdapter = 0;
	HRESULT hr;
	while (true)
	{
		Adapter Adapter_;
		IDXGIAdapter* pAdapter = nullptr;
		if (FAILED(hr = pFactory->EnumAdapters(iAdapter, &pAdapter)))
		{
			break;
		}
		pAdapter->GetDesc(&Adapter_.desc);
		Adapter_.pAdapter = pAdapter;
		m_AdapterList.push_back(Adapter_);
		++iAdapter;
	}
	iAdapter = 0;

	while (iAdapter < m_AdapterList.size())
	{
		DXGI_MODE_DESC de[512];
		int iOutput = 0;
		while (true)
		{
			Output Output_;
			UINT NumModes = 512;
			IDXGIOutput* pOutput = nullptr;
			if (FAILED(hr = m_AdapterList.at(iAdapter).pAdapter->EnumOutputs(iOutput, &pOutput)))
			{
				break;
			}
			if (pOutput != nullptr)
			{
				pOutput->GetDesc(&Output_.desc);
				if (FAILED(hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
					DXGI_ENUM_MODES_SCALING,
					&NumModes, de)))
				{
					return false;
				}
			}
			Output_.m_OutputList.push_back(pOutput);
			m_AdapterList.at(iAdapter).m_OutputList.push_back(Output_);
			for (int iMode = 0; iMode < 512; ++iMode)
			{
				if (de[iMode].Width >= 800 && de[iMode].Width <= 1920 && de[iMode].Height <= 1080 && de[iMode].Height >= 600)
				{
					m_AdapterList.at(iAdapter).m_OutputList.at(iOutput).m_ModList.push_back(de[iMode]);
				}
			}
			++iOutput;
		}
		++iAdapter;
	}
	return true;
}
bool Sample::Init()
{
	PreInit();
	return true;
}
bool Sample::Render()
{
	return true;
}
bool Sample::Release()
{
	for (auto iter : m_AdapterList)
	{
		for (auto it : iter.m_OutputList)
		{
			for (auto itt : it.m_OutputList)
			{
				itt->Release();
			}
		}
		iter.pAdapter->Release();
	}
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"DXGI");
	wd.Run();
	return 0;
}