//=============================================================================
//
// ライトの処理 [light.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "stdafx.h"

//*********************************************************
// ライトクラス
//*********************************************************
interface CLight:public IUnknown
{
public:
	static const UINT NUM_LIGHT = 3;

public:
	CLight();
	~CLight();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(void);
	void Update(void);

	void SetLight(void);


private:
	unsigned long m_dwRef;

	D3DLIGHT9 m_aLight[NUM_LIGHT];		// ライト情報

};
