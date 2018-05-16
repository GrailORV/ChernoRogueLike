////=============================================================================
////
//// デバッグログ処理 [debugLog.h]
//// Author : SORA ENOMOTO
////
////=============================================================================
//#pragma once
//
//#include "main.h"
//
////*****************************************************************************
//// デバッグログ処理クラス
////*****************************************************************************
//class CDebugLog :public IUnknown
//{
//public:
//	CDebugLog();
//	~CDebugLog();
//
//	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
//	STDMETHODIMP_(ULONG) AddRef(void);
//	STDMETHODIMP_(ULONG) Release(void);
//
//	HRESULT Init(void);
//	void Update(void);
//
//	static void Print(char *fmt, ...);
//	static void Draw(void);
//
//
//private:
//	DWORD m_dwRef;
//
//};