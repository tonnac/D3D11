
#include "Core.h"
#include "DirectWrite.h"
#include "DirectInput.h"
#include "SRVStorage.h"
#include <windowsx.h>


using namespace DirectX;
using Microsoft::WRL::ComPtr;
using namespace std;
using namespace Input;


Core::Core(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName) 
	: wClass(hInstance, Width, Height, WindowName)
{
}

Core::Core(HINSTANCE hInstance, HWND hWnd, RECT rt)
{
	g_hWnd = hWnd;
	g_hInstance = hInstance;
	g_ClientWidth = (rt.right - rt.left);
	g_ClientHeight = (rt.bottom - rt.top);
}

bool Core::Initialize()
{
	if (!InitMainWindow())
		return false;
	if (!InitDirect3D())
		return false;
	if (!S_Write.Initialize())
		return false;
	
	OnResize();
	return true;
}

bool Core::ToolInitialize()
{
	if (!InitDirect3D())
		return false;
	if (!S_Write.Initialize())
		return false;
	GameInit();
	OnResize();
	return true;
}

bool Core::ToolRun()
{
	GameFrame();
	GameRender();
	return false;
}

bool Core::GameInit()
{
	m_Timer.Reset();
	S_Input.Init();

	DxState::InitState(m_pd3dDevice.Get());
	mShaderStorage->Initialize(m_pd3dDevice.Get());
	SrvStorage::GetStorage()->SetDevice(m_pd3dDevice.Get());
	mDxObj = std::move(DxObjStorage::Dxobj()->GetDxobjList());
	 
	m_DefaultCamera.SetViewMatrix(XMFLOAT3(0,45,-120));
	m_DefaultCamera.SetProjMatrix(XM_PIDIV4, AspectRatio());
	m_pMainCamera = &m_DefaultCamera;

	d3dUtil::CreateConstantBuffer(m_pd3dDevice.Get(), 1, sizeof(PassConstants), mPassCB.GetAddressOf());

	m_Dir.Create(m_pd3dDevice.Get());
	mSkybox.Create(m_pd3dDevice.Get(), L"..\\..\\data\\cube\\snowcube1024.dds");
	Init();
	return true;
}

bool Core::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}

LRESULT Core::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_Paused = true;
			m_Timer.Stop();
		}
		else
		{
			m_Paused = false;
			m_Timer.Start();
		}
		return 0;

	case WM_SIZE:
		g_ClientWidth = LOWORD(lParam);
		g_ClientHeight = HIWORD(lParam);
		if (m_pd3dDevice != nullptr)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_Paused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_Paused = false;
				m_Minimized = false;
				m_Maximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_Minimized == true)
				{
					m_Paused = false;
					m_Minimized = false;
					OnResize();
				}
				else if (m_Maximized == true)
				{
					m_Paused = false;
					m_Maximized = false;
					OnResize();
				}
				else if (m_Resizing == true)
				{

				}
				else
				{
					OnResize();
				}
			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		m_Paused = true;
		m_Resizing = true;
		m_Timer.Stop();
		return 0;

	case WM_EXITSIZEMOVE:
		m_Paused = false;
		m_Resizing = false;
		m_Timer.Start();
		OnResize();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


bool Core::Init()
{
	return true;
}

bool Core::Frame()
{
	return true;
}

bool Core::Render()
{
	return true;
}

void Core::CalculateFrame()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	m_Timer.Tick();
	++frameCnt;

	if (m_Timer.TotalTime() - timeElapsed >= 1.0f)
	{
		float fps = Casting(float, frameCnt);
		float mspf = 1000.0f / fps;

		tstring fpsStr = to_tstring(fps);
		tstring mspfStr = to_tstring(mspf);

		m_FrameInfo = L"fps:" + fpsStr + L"    mspf:" + mspfStr;

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void Core::FramePassCB()
{
	XMMATRIX View = XMLoadFloat4x4(&m_pMainCamera->m_matView);
	XMMATRIX Proj = XMLoadFloat4x4(&m_pMainCamera->m_matProj);

	XMMATRIX InvView = XMMatrixInverse(&XMMatrixDeterminant(View), View);
	XMMATRIX InvProj = XMMatrixInverse(&XMMatrixDeterminant(Proj), Proj);

	XMMATRIX ViewProj = View * Proj;
	XMMATRIX InvViewProj = XMMatrixInverse(&XMMatrixDeterminant(ViewProj), ViewProj);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(View));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(Proj));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(InvView));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(InvProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(ViewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(InvViewProj));
	mMainPassCB.EyePosW = m_pMainCamera->m_vPos;

	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = m_Timer.DeltaTime();
	mMainPassCB.DeltaTime = m_Timer.TotalTime();

	LightStorage * light = LightStorage::getLight();

	light->CopyDirectional(mMainPassCB.Lights);
	light->CopyPoint(mMainPassCB.Lights);
	light->CopySpot(mMainPassCB.Lights);

	mMainPassCB.DirectionalNum = light->NumDirectional();
	mMainPassCB.PointNum = light->NumPoint();
	mMainPassCB.SpotNum = light->NumSpot();

	m_pImmediateContext->UpdateSubresource(mPassCB.Get(), 0, nullptr, &mMainPassCB, 0, 0);
}

void Core::OnResize()
{
	S_Write.Reset();
	Device::OnResize();
	S_Write.OnResize(m_pSwapchain.Get());
}

bool Core::GameFrame()
{
	CalculateFrame();
	S_Input.Frame();
	XMFLOAT4 retInput = OnKeyboardInput();
	m_pMainCamera->Update(retInput);
	m_pMainCamera->Frame();
	LightStorage::getLight()->UpdateLight(m_Timer);
	Frame();
	FramePassCB();
	MatStorage->UpdateMaterialCBs(m_pImmediateContext.Get());
	S_RItem.UpdateObjectCBs(m_pImmediateContext.Get());
	S_Input.PostFrame();
	return true;
}

bool Core::PreRender()
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), &mBackColor.x);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);

	auto SamplerStates = DxState::GetSamArray();

	m_pImmediateContext->VSSetConstantBuffers(0, 1, mPassCB.GetAddressOf());
	m_pImmediateContext->GSSetConstantBuffers(0, 1, mPassCB.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(0, 1, mPassCB.GetAddressOf());
	m_pImmediateContext->PSSetSamplers(0, Casting(UINT, SamplerStates.size()), SamplerStates.data());

	S_Write.Begin();
	if(m_bFrameinfo)
		S_Write.DrawText({ 0, 0, 800, 600 }, m_FrameInfo, Colors::Black);
	return true;
}

bool Core::GameRender()
{
	PreRender();
	{
		mDxObj[DxType::LINE]->SetResource(m_pImmediateContext.Get());
		m_Dir.Render(m_pImmediateContext.Get());
		Render();
		if (misSkybox)
		{
			mDxObj[DxType::SKY]->SetResource(m_pImmediateContext.Get());
			mSkybox.Render(m_pImmediateContext.Get());
		}
	}
	PostRender();
	return true;
}

bool Core::PostRender()
{
	S_Write.End();
	ThrowifFailed(m_pSwapchain->Present(0, 0));
	return true;
}

XMFLOAT4 Core::OnKeyboardInput()
{
	XMFLOAT4 YawPitchRoll(0, 0, 0, 0);
	auto mousePos = S_Input.getMousePos();
	auto deltaTime = m_Timer.DeltaTime();

	if (S_Input.getKeyState(DIK_LCONTROL) == KEYSTATE::KEY_HOLD && S_Input.getKeyState(DIK_F) == KEYSTATE::KEY_PUSH)
	{
		m_bFrameinfo = !m_bFrameinfo;
	}

	//if (S_Input.getKeyState(DIK_1) == KEYSTATE::KEY_PUSH)
	//{
	//	if (mDxObj[DxType::SKINNED]->m_RasterizerState == E_RSS::Wireframe)
	//	{
	//		mDxObj[DxType::SKINNED]->m_RasterizerState = E_RSS::Default;
	//	}
	//	else if (mDxObj[DxType::SKINNED]->m_RasterizerState == E_RSS::Default)
	//	{
	//		mDxObj[DxType::SKINNED]->m_RasterizerState = E_RSS::Wireframe;
	//	}	
	//}

	if (S_Input.getKeyState(DIK_2) == KEYSTATE::KEY_PUSH)
	{
		if (mDxObj[DxType::DEFAULT]->m_RasterizerState == E_RSS::Wireframe)
		{
			mDxObj[DxType::DEFAULT]->m_RasterizerState = E_RSS::Default;
		}
		else if (mDxObj[DxType::DEFAULT]->m_RasterizerState == E_RSS::Default)
		{
			mDxObj[DxType::DEFAULT]->m_RasterizerState = E_RSS::Wireframe;
		}
	}

	//if (S_Input.getKeyState(DIK_3) == KEYSTATE::KEY_PUSH)
	//{
	//	IncreaseEnum(m_BlendState);
	//}

	if (S_Input.getKeyState(DIK_A) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->MoveSide(-deltaTime * 150.0f);
	}

	if (S_Input.getKeyState(DIK_D) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->MoveSide(deltaTime * 150.0f);
	}

	if (S_Input.getKeyState(DIK_W) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->MoveLook(deltaTime * 150.0f);
	}

	if (S_Input.getKeyState(DIK_S) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->MoveLook(-deltaTime * 150.0f);
	}

	if (S_Input.getKeyState(DIK_LBUTTON) == KEYSTATE::KEY_HOLD)
	{
		YawPitchRoll.x += 0.2f * XMConvertToRadians(Casting(float, mousePos.lY));
		YawPitchRoll.y += 0.2f * XMConvertToRadians(Casting(float, mousePos.lX));
	}

	if (S_Input.getKeyState(DIK_SPACE) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->m_fSpeed += m_Timer.DeltaTime() * 5.0f;
	}

	float fValue = Casting(float, mousePos.lZ);
	YawPitchRoll.w = fValue * deltaTime;
	return YawPitchRoll;
}