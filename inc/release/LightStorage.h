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

using LightVec = std::vector<std::shared_ptr<LightProperty>>;

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
	void AddLight(std::shared_ptr<LightProperty>& light);

	UINT NumDirectional() const;
	UINT NumPoint() const;
	UINT NumSpot() const;

	void CopyDirectional(Light* lights);
	void CopyPoint(Light* lights);
	void CopySpot(Light* lights);

	void DelLight(int nIndex);

	LightVec* GetLightVec();
	LightProperty* GetProperty(int nIndex);

private:
	void AddDirectional(std::shared_ptr<LightProperty>& light);
	void AddPoint(std::shared_ptr<LightProperty>& light);
	void AddSpot(std::shared_ptr<LightProperty>& light);

	void UpdateLight();

private:
	std::vector<std::shared_ptr<LightProperty>> mLights;
	std::vector<std::weak_ptr<LightProperty>> mDirectionalLight;
	std::vector<std::weak_ptr<LightProperty>> mPointLight;
	std::vector<std::weak_ptr<LightProperty>> mSpotLight;
};