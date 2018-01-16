//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "stdafx.h"

//*********************************************************
// �`��N���X
//*********************************************************
class CRenderer :public IUnknown
{
public:
	CRenderer();
	~CRenderer();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(BOOL bWindow);
	void Update(void);
	void Draw(void);

	IDirect3DDevice9* GetDevice(void) { return m_pD3DDevice.Get(); }

	void SetDefaultMaterial(void);

private:
	void OnLostDevice(void);

	DWORD m_dwRef;
	BOOL m_bWindow;

	ComPtr<IDirect3D9> m_pD3D;					// Direct3D �I�u�W�F�N�g
	ComPtr<IDirect3DDevice9> m_pD3DDevice;		// Device�I�u�W�F�N�g(�`��ɕK�v)

};

