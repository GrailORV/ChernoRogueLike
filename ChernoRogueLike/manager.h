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

class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoypad;

#ifdef _DEBUG
class CDebugProc;
#endif

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

	UINT GetWindowWidth(void) { return m_width; }
	UINT GetWindowHeight(void) { return m_height; }

	CRenderer* GetRenderer(void) { return m_pRenderer; }
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }

private:
	CRenderer* m_pRenderer;
	CInputKeyboard *m_pInputKeyboard;	// キーボードへのポインタ
	CInputMouse *m_pInputMouse;			// マウスへのポインタ
	CInputJoypad *m_pInputJoypad;		// ジョイパッドへのポインタ

	UINT m_width;
	UINT m_height;

#ifdef _DEBUG
	CDebugProc *m_pDebugProc;			// デバッグ処理へのポインタ
#endif

};
