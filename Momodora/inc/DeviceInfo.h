#pragma once
#include "d3dUtil.h"

class DeviceInfo
{
public:
	static void CreateDeviceInfo();
private:
	static void AdapterList(IDXGIFactory* pDXGIFactory);
	static void OutputList(IDXGIAdapter* pAdapter, std::tofstream& fp);
};