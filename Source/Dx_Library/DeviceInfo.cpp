#include "DeviceInfo.h"

void DeviceInfo::CreateDeviceInfo()
{
	IDXGIFactory* pFactory = nullptr;
	ThrowifFailed(CreateDXGIFactory(IID_PPV_ARGS(&pFactory)));
	AdapterList(pFactory);
	
	RELEASE(pFactory);
}

void DeviceInfo::AdapterList(IDXGIFactory* pDXGIFactory)
{
	UINT iAdapter = 0;
	IDXGIAdapter * pAdapter = nullptr;

	std::tofstream fp(L"DeviceInfo.txt");
	if (!fp.is_open())
	{
		MessageBox(nullptr, std::tstring(L"FILE: " + AnsiToTString(__FILE__) + L"\nLine: " + std::to_tstring(__LINE__)).c_str(), L"File Open Error", MB_OK);
		exit(0);
	}
	
	while (SUCCEEDED(pDXGIFactory->EnumAdapters(iAdapter, &pAdapter)))
	{
		DXGI_ADAPTER_DESC AdapterDesc;
		ThrowifFailed(pAdapter->GetDesc(&AdapterDesc));

		if (iAdapter != 0) fp << L"\n";
		fp << L"Description" << L'[' << iAdapter << L"]: " << AdapterDesc.Description;

		OutputList(pAdapter, fp);

		RELEASE(pAdapter);
		++iAdapter;
	}
}
void DeviceInfo::OutputList(IDXGIAdapter* pAdapter, std::tofstream& fp)
{
	UINT iOutput = 0;
	IDXGIOutput* pOutput = nullptr;

	while (SUCCEEDED(pAdapter->EnumOutputs(iOutput, &pOutput)))
	{
		DXGI_OUTPUT_DESC outputDesc;
		DXGI_MODE_DESC modeDesc[256];
		ZeroMemory(modeDesc, sizeof(DXGI_MODE_DESC) * 256);
		UINT NumMode = 256;

		ThrowifFailed(pOutput->GetDesc(&outputDesc));

		fp << L"\n\t Output" << L'[' << iOutput << L"] Device Name: " << outputDesc.DeviceName << L"\n\t DesktopCoorinates: 0, 0" <<
			L", " << outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left << L", " << outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;

		ThrowifFailed(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING, &NumMode, modeDesc));
		int modeDescNum = 0;

		do
		{
			fp << L"\n\t\tDisplayMode[" << modeDescNum << L"] Width: " << modeDesc[modeDescNum].Width 
				<< L" Height: " << modeDesc[modeDescNum].Height << L" Format: " << modeDesc[modeDescNum].Format 
				<< L" RefreshRate: " << modeDesc[modeDescNum].RefreshRate.Numerator / modeDesc[modeDescNum].RefreshRate.Denominator;
		} while (modeDesc[++modeDescNum].Height != 0);

		RELEASE(pOutput);
		++iOutput;
	}
}