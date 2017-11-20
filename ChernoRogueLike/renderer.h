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
interface CRenderer :public IUnknown
{
public:
	CRenderer();
	~CRenderer();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(HWND hwnd, BOOL bWindow);
	void Update(void);
	void Draw(void);

	IDirect3DDevice9* GetDevice(void) { return m_pD3DDevice.Get(); }

private:
	DWORD m_dwRef;

	ComPtr<IDirect3D9> m_pD3D;					// Direct3D オブジェクト
	ComPtr<IDirect3DDevice9> m_pD3DDevice;		// Deviceオブジェクト(描画に必要)

};

