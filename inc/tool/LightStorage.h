#pragma once

#include "FrameResource.h"
#include "Timer.h"

struct LightProperty
{
	bool isClockwise = true;
	bool isRotate = false;
	bool isPrimary = false;
	DirectX::XMFLOAT3 Axis = DirectX::XMFLOAT3(0, 0, 0);
	float RotSpeed = 1.0f;

	Light light;
};

class LightStorage
{
private:
	LightStorage() = default;
public:
	static LightStorage* getLight()
	{
		static LightStorage light;
		return &light;
	}

public:
	void UpdateLight(const Timer& gt);

public:
	void AddDirectional(std::unique_ptr<LightProperty>& light);
	void AddPoint(std::unique_ptr<LightProperty>& light);
	void AddSpot(std::unique_ptr<LightProperty>& light);

	UINT NumDirectional() const;
	UINT NumPoint() const;
	UINT NumSpot() const;

	void DelDirectional(int index);
	void DelPoint(int index);
	void DelSpot(int index);

	void CopyDirectional(Light* lights);
	void CopyPoint(Light* lights);
	void CopySpot(Light* lights);

private:
	std::vector<std::unique_ptr<LightProperty>> mLights;
	std::vector<Light*> mDirectionalLight;
	std::vector<Light*> mPointLight;
	std::vector<Light*> mSpotLight;
};