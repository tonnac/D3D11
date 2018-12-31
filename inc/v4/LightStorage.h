#pragma once

#include "FrameResource.h"


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
	void AddDirectional(const Light& light);
	void AddPoint(const Light& light);
	void AddSpot(const Light& light);

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
	std::vector<Light> mDirectionalLight;
	std::vector<Light> mPointLight;
	std::vector<Light> mSpotLight;
};