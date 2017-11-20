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
#include "input.h"

//*********************************************************
// マネージャークラス
//*********************************************************
class CManager
{
public:
	CManager(UINT width, UINT height);
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hwnd, BOOL bWIndow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetWindowWidth(UINT width) { m_width = width; }
	UINT GetWindowWidth(void) { return m_width; }
	void SetWindowHeight(UINT height) { m_height = height; }
	UINT GetWindowHeight(void) { return m_height; }

	CRenderer* GetRenderer(void) { return m_pRenderer.Get(); }
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard.Get(); }
	CInputMouse *GetInputMouse(void) { return m_pInputMouse.Get(); }
	CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad.Get();; }
	CLight *GetLight(void) { return m_pLight.Get(); }

private:
	ComPtr<CRenderer> m_pRenderer;
	ComPtr<CInputKeyboard> m_pInputKeyboard;	// キーボードへのポインタ
	ComPtr<CInputMouse> m_pInputMouse;			// マウスへのポインタ
	ComPtr<CInputJoypad> m_pInputJoypad;		// ジョイパッドへのポインタ
	ComPtr<CLight> m_pLight;

	UINT m_width;
	UINT m_height;

#ifdef _DEBUG
	ComPtr<CDebugProc> m_pDebugProc;			// デバッグ処理へのポインタ
#endif

};
