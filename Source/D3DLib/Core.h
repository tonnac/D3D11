#pragma once

#include "wClass.h"
#include "Timer.h"
#include "Shape.h"
#include "Camera.h"
#include "ShaderStorage.h"
#include "LightStorage.h"


class Core : public wClass
{
protected:
	Core(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);
	Core(HINSTANCE hInstance, HWND hWnd, RECT rt);
public:
	bool			Initialize();
	bool			ToolInitialize();
	bool			ToolRun();
	virtual void	OnResize()override;
protected:
	virtual bool	GameInit()override;
	virtual bool	GameRun()override;
	LRESULT			MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)override;

	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();

	void			CalculateFrame();
	void			FramePassCB();

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


	DirectX::XMFLOAT4 mBackColor = DirectX::XMFLOAT4(0.647058845f, 0.164705887f, 0.164705887f, 1.0f);
	bool misSkybox = true;
};