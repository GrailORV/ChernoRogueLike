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

private:
	CRenderer* m_pRenderer;

	UINT m_width;
	UINT m_height;

};
