//=============================================================================
//
// マネージャーの処理 [manager.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

//*********************************************************
// ヘッダファイル
//*********************************************************
#include "stdafx.h"
#include "debugproc.h"
#include "renderer.h"
#include "light.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "textureManager.h"
#include "modelManager.h"
#include "gameModeManager.h"

class CScene2D;

//*********************************************************
// マネージャークラス
//*********************************************************
class CManager :public IUnknown
{
public:
	CManager(UINT width, UINT height);
	~CManager();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(HINSTANCE hInstance, HWND hwnd, BOOL bWIndow);
	void Update(void);
	void Draw(void);

	void SetWindowWidth(UINT width) { m_width = width; }
	UINT GetWindowWidth(void) { return m_width; }
	void SetWindowHeight(UINT height) { m_height = height; }
	UINT GetWindowHeight(void) { return m_height; }

	CRenderer* GetRenderer(void) { return m_pRenderer.Get(); }
	CTextureManager* GetTextureManager(void) { return m_pTextureManager.Get(); }
	CModelManager* GetModelManager(void) { return m_pModelManager.Get(); }
	CGameModeManager* GetModeManager(void) { return m_pGameModeManager.Get(); }
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard.Get(); }
	CInputMouse *GetInputMouse(void) { return m_pInputMouse.Get(); }
	CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad.Get();; }
	CSound *GetSound(void) { return m_pSound.Get(); }
	CCamera *GetCamera(void) { return m_pCamera.Get(); }
	CLight *GetLight(void) { return m_pLight.Get(); }

private:
	DWORD m_dwRef;

	ComPtr<CRenderer> m_pRenderer;
	ComPtr<CTextureManager> m_pTextureManager;
	ComPtr<CModelManager> m_pModelManager;
	ComPtr<CGameModeManager> m_pGameModeManager;
	ComPtr<CInputKeyboard> m_pInputKeyboard;	// キーボードへのポインタ
	ComPtr<CInputMouse> m_pInputMouse;			// マウスへのポインタ
	ComPtr<CInputJoypad> m_pInputJoypad;		// ジョイパッドへのポインタ
	ComPtr<CSound> m_pSound;
	ComPtr<CCamera> m_pCamera;
	ComPtr<CLight> m_pLight;

	UINT m_width;
	UINT m_height;

#ifdef _DEBUG
	ComPtr<CDebugProc> m_pDebugProc;			// デバッグ処理へのポインタ
#endif

};
