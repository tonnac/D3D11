#pragma once

#include "FrameResource.h"
#include "Timer.h"

enum class LightType : unsigned char
{
	Directional,
	Point,
	Spot
};

struct RotInfo
{
	bool isClockwise = true;
	bool isRotate = false;
	DirectX::XMFLOAT3 Axis = DirectX::XMFLOAT3(0, 0, 0);
	float RotSpeed = 1.0f;
};


struct LightProperty
{
	LightType Type = LightType::Directional;
	bool isPrimary = false;

	RotInfo DirRot;
	RotInfo PosRot;

	Light light;
};

using LightVec = std::vector<std::unique_ptr<LightProperty>>;

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
	void AddLight(std::unique_ptr<LightProperty>& light);

	UINT NumDirectional() const;
	UINT NumPoint() const;
	UINT NumSpot() const;

	void DelDirectional(int index);
	void DelPoint(int index);
	void DelSpot(int index);

	void CopyDirectional(Light* lights);
	void CopyPoint(Light* lights);
	void CopySpot(Light* lights);

	LightVec* GetLightVec();

private:
	void AddDirectional(std::unique_ptr<LightProperty>& light);
	void AddPoint(std::unique_ptr<LightProperty>& light);
	void AddSpot(std::unique_ptr<LightProperty>& light);

private:
	std::vector<std::unique_ptr<LightProperty>> mLights;
	std::vector<Light*> mDirectionalLight;
	std::vector<Light*> mPointLight;
	std::vector<Light*> mSpotLight;
};