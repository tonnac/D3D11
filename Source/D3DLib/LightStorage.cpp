#include "LightStorage.h"

void LightStorage::AddDirectional(const Light & light)
{
	mDirectionalLight.push_back(light);
}

void LightStorage::AddPoint(const Light & light)
{
	mPointLight.push_back(light);
}

void LightStorage::AddSpot(const Light & light)
{
	mSpotLight.push_back(light);
}

UINT LightStorage::NumDirectional() const
{
	return (UINT)mDirectionalLight.size();
}

UINT LightStorage::NumPoint() const
{
	return (UINT)mPointLight.size();
}

UINT LightStorage::NumSpot() const
{
	return (UINT)mSpotLight.size();
}

void LightStorage::DelDirectional(int index)
{
	mDirectionalLight.erase(std::next(mDirectionalLight.begin(), index));
}

void LightStorage::DelPoint(int index)
{
	mPointLight.erase(std::next(mPointLight.begin(), index));
}

void LightStorage::DelSpot(int index)
{
	mSpotLight.erase(std::next(mSpotLight.begin(), index));
}

void LightStorage::CopyDirectional(Light* lights)
{
	std::copy(mDirectionalLight.begin(), mDirectionalLight.end(), &lights[0]);
}

void LightStorage::CopyPoint(Light* lights)
{
	std::copy(mPointLight.begin(), mPointLight.end(), &lights[(UINT)mDirectionalLight.size()]);
}

void LightStorage::CopySpot(Light* lights)
{
	std::copy(mSpotLight.begin(), mSpotLight.end(), &lights[(UINT)(mDirectionalLight.size() + mPointLight.size())]);
}
