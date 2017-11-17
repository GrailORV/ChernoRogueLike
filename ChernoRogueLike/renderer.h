//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "stdafx.h"

//*********************************************************
// 描画クラス
//*********************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hwnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	IDirect3DDevice9* GetDevice(void) { return m_pD3DDevice.Get(); }

private:
	ComPtr<IDirect3D9> m_pD3D;					// Direct3D オブジェクト
	ComPtr<IDirect3DDevice9> m_pD3DDevice;		// Deviceオブジェクト(描画に必要)

};

