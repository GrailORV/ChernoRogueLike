//=============================================================================
//
// ゲームモード管理処理 [modeManager.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "stdafx.h"
#include "debugMode.h"

class CMode;

//*********************************************************
// ゲームモード管理クラス
//*********************************************************
class CGameModeManager :public IUnknown
{
public:
	CGameModeManager();
	~CGameModeManager();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	template<class C>
	HRESULT Init(void)
	{
		HRESULT hr = E_FAIL;

		m_pMode = new C;
		hr = m_pMode->Init();

		if (FAILED(hr))
		{
			m_pMode = new CDebugMode();
			m_pMode->Init();
		}

		return hr;
	}

	template<class C>
	void Load(void)
	{
		HRESULT hr = E_FAIL;

		SafeUninit(m_pMode);

		m_pMode = new C;
		hr = m_pMode->Init();

		if (FAILED(hr))
		{
			m_pMode = new CDebugMode();
			m_pMode->Init();
		}
	}

	void OnLostDevice(void);

private:
	DWORD m_dwRef;

	CMode* m_pMode;

};