//=============================================================================
//
// テクスチャ管理処理 [textureManager.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "stdafx.h"
#include <map>

//*********************************************************
// テクスチャ管理クラス
//*********************************************************
interface CTextureManager :public IUnknown
{
public:
	CTextureManager();
	~CTextureManager();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(void);

	void LoadSceneTex(int mode);
	void BindtextureFromString(std::string texID, IDirect3DTexture9** ppTex);

	void OnLostDevice(void);

private:
	DWORD m_dwRef;

	std::map<std::string, ComPtr<IDirect3DTexture9>> m_texData;

};