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

class CPlayer;
class CMap;
class CScene2D;

//*********************************************************
// マネージャークラス
//*********************************************************
class CManager :public IUnknown
{
public:
	CManager(UINT width, UINT height);
	~CManager();

	// モードの種類
	typedef enum
	{
		MODE_NONE = 0,
		MODE_TITLE,		// タイトル画面
		MODE_GAME,		// ゲーム画面
		MODE_RESULT,	// リザルト画面
		MODE_MAX
	} MODE;

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
	CModelManager* GetMeshManager(void) { return m_pModelManager.Get(); }
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard.Get(); }
	CInputMouse *GetInputMouse(void) { return m_pInputMouse.Get(); }
	CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad.Get();; }
	CSound *GetSound(void) { return m_pSound.Get(); }
	CCamera *GetCamera(void) { return m_pCamera.Get(); }
	CLight *GetLight(void) { return m_pLight.Get(); }

	void SetMode(MODE mode);
	MODE GetMode(void) { return m_mode; }

private:
	DWORD m_dwRef;

	ComPtr<CRenderer> m_pRenderer;
	ComPtr<CTextureManager> m_pTextureManager;
	ComPtr<CModelManager> m_pModelManager;
	ComPtr<CInputKeyboard> m_pInputKeyboard;	// キーボードへのポインタ
	ComPtr<CInputMouse> m_pInputMouse;			// マウスへのポインタ
	ComPtr<CInputJoypad> m_pInputJoypad;		// ジョイパッドへのポインタ
	ComPtr<CSound> m_pSound;
	ComPtr<CCamera> m_pCamera;
	ComPtr<CLight> m_pLight;

	CScene2D* m_pScene;
	CPlayer* m_pPlayer;
	CMap* m_pMap;

	UINT m_width;
	UINT m_height;

	MODE m_mode;

#ifdef _DEBUG
	ComPtr<CDebugProc> m_pDebugProc;			// デバッグ処理へのポインタ
#endif

};
