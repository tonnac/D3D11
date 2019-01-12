#include "LightStorage.h"

using namespace DirectX;
using namespace std::chrono;

void LightStorage::UpdateLight(const Timer & gt)
{
	for (auto& light : mLights)
	{
		if (light->DirRot.isRotate)
		{
			float deltaTime;
			if (light->DirRot.isClockwise)
				deltaTime = gt.DeltaTime();
			else
				deltaTime = -gt.DeltaTime();

			float Theta = deltaTime * light->DirRot.RotSpeed;

			XMVECTOR D = XMLoadFloat3(&light->light.Direction);
			XMVECTOR Axis = XMLoadFloat3(&light->DirRot.Axis);

			XMMATRIX R = XMMatrixRotationAxis(Axis, Theta);

			D = XMVector3Normalize(XMVector3Transform(D, R));

			XMStoreFloat3(&light->light.Direction, D);
		}

		if (light->PosRot.isRotate)
		{
			float deltaTime;
			if (light->PosRot.isClockwise)
				deltaTime = gt.DeltaTime();
			else
				deltaTime = -gt.DeltaTime();

			float Theta = deltaTime * light->PosRot.RotSpeed;

			XMVECTOR P = XMLoadFloat3(&light->light.Position);
			XMVECTOR Axis = XMLoadFloat3(&light->PosRot.Axis);

			XMMATRIX R = XMMatrixRotationAxis(Axis, Theta);

			P = XMVector3Transform(P, R);

			XMStoreFloat3(&light->light.Position, P);
		}
	}
}

void LightStorage::AddLight(std::unique_ptr<LightProperty>& light)
{
	switch (light->Type)
	{
	case LightType::Directional:
		AddDirectional(light);
		break;
	case LightType::Point:
		AddPoint(light);
		break;
	case LightType::Spot:
		AddSpot(light);
		break;
	default:
		break;
	}
}

void LightStorage::AddDirectional(std::unique_ptr<LightProperty>& light)
{
	mDirectionalLight.push_back(&light->light);
	mLights.push_back(std::move(light));
}

void LightStorage::AddPoint(std::unique_ptr<LightProperty>& light)
{
	mPointLight.push_back(&light->light);
	mLights.push_back(std::move(light));
}

void LightStorage::AddSpot(std::unique_ptr<LightProperty>& light)
{
	mSpotLight.push_back(&light->light);
	mLights.push_back(std::move(light));
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
	UINT i = 0;

	for (auto& p = mDirectionalLight.begin(); p != mDirectionalLight.end(); ++p)
	{
		const Light& light = *(*p);
		CopyMemory(&lights[i++], &light, sizeof(Light));
	}
}

void LightStorage::CopyPoint(Light* lights)
{
	UINT i = (UINT)mDirectionalLight.size();

	for (auto& p = mPointLight.begin(); p != mPointLight.end(); ++p)
	{
		const Light& light = *(*p);
		CopyMemory(&lights[i++], &light, sizeof(Light));
	}
}

void LightStorage::CopySpot(Light* lights)
{
	UINT i = (UINT)(mDirectionalLight.size() + mPointLight.size());

	for (auto& p = mSpotLight.begin(); p != mSpotLight.end(); ++p)
	{
		const Light& light = *(*p);
		CopyMemory(&lights[i++], &light, sizeof(Light));
	}
}

LightVec * LightStorage::GetLightVec()
{
	return &mLights;
}

LightProperty * LightStorage::GetProperty(int nIndex)
{
	return mLights[nIndex].get();
}
