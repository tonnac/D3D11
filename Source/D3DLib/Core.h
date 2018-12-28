#pragma once

#include "wClass.h"
#include "Timer.h"
#include "Shape.h"
#include "Camera.h"
#include "ShaderStorage.h"
#include "FrameResource.h"


class Core : public wClass
{
protected:
	Core(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);
public:
	bool			Initialize();

protected:
	virtual bool	GameInit()override;
	virtual bool	GameRun()override;
	LRESULT			MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)override;

	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();

	void			CalculateFrame();
	void			FramePassCB();
	virtual void	OnResize()override;

private:
	bool			GameFrame();
	bool			PreRender();
	bool			GameRender();
	bool			PostRender();

	DirectX::XMFLOAT4 OnKeyboardInput();

protected:
	bool			m_bFrameinfo = true;
	std::tstring	m_FrameInfo;
	Timer			m_Timer;
	DirectionShape	m_Dir;

	Camera  m_DefaultCamera;
	Camera* m_pMainCamera = nullptr;

	PassConstants mMainPassCB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mPassCB = nullptr;

	ShaderStorage * mShaderStorage = ShaderStorage::Storage();
	std::unordered_map<DxType, std::unique_ptr<DxObj>> mDxObj;

	SkyBox mSkybox;

	MaterialStorage * MatStorage = MaterialStorage::GetStorage();
};