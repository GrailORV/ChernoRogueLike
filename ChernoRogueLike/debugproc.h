//=============================================================================
//
// デバッグ処理 [debugproc.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// デバッグ処理クラス
//*****************************************************************************
interface CDebugProc :public IUnknown
{
private:
	static const u_int LENGTH_STRING_BUFF = 1024;
	static const DWORD FVF_DEBUG_PROC = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	struct VertexDebug
	{
		D3DXVECTOR3 position;
		float rhw;
		D3DCOLOR color;
	};

public:
	CDebugProc();
	~CDebugProc();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(void);
	void Update(void);

	static void Print(char *fmt, ...);
	static void Draw(void);

	static bool IsEnableDisp(void) { return m_bDisp; }
	static void EnableDisp(bool bDisp) { m_bDisp = bDisp; }

private:
	DWORD m_dwRef;

	static ComPtr<ID3DXFont> m_pFont;				// フォントへのポインタ
	static char m_aStr[LENGTH_STRING_BUFF];	// デバッグ表示のバッファ
	static const float m_fWidth;
	static const float m_fHeight;


	static ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;

	static bool m_bDisp;					// デバッグ表示ON/OFF

	static HRESULT MakeVertexBuffer(void);
};

