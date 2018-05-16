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
class CDebugProc :public IUnknown
{
private:
	static const u_int LENGTH_STRING_BUFF = 1024;
	static const DWORD FVF_DEBUG_PROC = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	struct VertexDebug
	{
		Vector3 position;
		float rhw;
		Color color;
		Vector2 texcoord;
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
	static float m_fWidth;
	static float m_fHeight;

	static ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;

	static ComPtr<IDirect3DTexture9> m_pTexture;

	static bool m_bDisp;					// デバッグ表示ON/OFF

	static HRESULT MakeVertexBuffer(void);

private:
	static const uint32_t m_cTexSize;

	void IsInputKeyboard();
	void IsInputMouse();
	void RenderTexture();

};

