//=============================================================================
//
// デバッグ処理 [debugproc.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include <stdio.h>
#include <stdarg.h>
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "WinApp.h"
#include "input.h"

//*****************************************************************************
// 静的変数
//*****************************************************************************
ComPtr<ID3DXFont> CDebugProc::m_pFont;
ComPtr<IDirect3DVertexBuffer9> CDebugProc::m_pVtxBuff;
ComPtr<IDirect3DTexture9> CDebugProc::m_pTexture;
char CDebugProc::m_aStr[LENGTH_STRING_BUFF] = { "\0" };
bool CDebugProc::m_bDisp = true;
float CDebugProc::m_fWidth = 320.0f;
float CDebugProc::m_fHeight = 540.0f;
const uint32_t CDebugProc::m_cTexSize = 512;

//=============================================================================
// CDebugProcコンストラクタ
//=============================================================================
CDebugProc::CDebugProc() :m_dwRef(0)
{
	// バッファクリア
	memset(m_aStr, 0, sizeof m_aStr);
}

//=============================================================================
// CDebugProcデストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{
	Release();
}

//=============================================================================
// 参照コピー
//=============================================================================
HRESULT CDebugProc::QueryInterface(REFIID riid, void FAR* FAR* ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown))
	{
		*ppvObject = this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

//=============================================================================
// 参照カウンタインクリメント
//=============================================================================
ULONG CDebugProc::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// 終了処理
//=============================================================================
ULONG CDebugProc::Release(void)
{
	if (--m_dwRef == 0)
	{
		delete this;
		return 0;
	}
	return m_dwRef;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDebugProc::Init(void)
{
	HRESULT hr{};

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// 情報表示用フォントを設定
	D3DXFONT_DESC fontDesc;
	fontDesc.Height = 24;
	fontDesc.Width = 12;
	fontDesc.Weight = FW_BOLD;
	fontDesc.MipLevels = 0;
	fontDesc.Italic = FALSE;
	fontDesc.CharSet = SHIFTJIS_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH;
	strcpy_s(fontDesc.FaceName, "Terminal");

	// フォントの生成
	hr = D3DXCreateFontIndirect(pDevice, &fontDesc, m_pFont.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	// 背景板ポリの生成
	hr = MakeVertexBuffer();
	if (FAILED(hr))
	{
		return hr;
	}

	RenderTexture();

	return S_OK;

}

//=============================================================================
// 更新処理
//=============================================================================
void CDebugProc::Update(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

	if (!pInputKeyboard)
	{
		return;
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_F1))
	{
		m_bDisp = !m_bDisp;
	}
}

//=============================================================================
// 書き込み処理
//=============================================================================
void CDebugProc::Print(char *fmt, ...)
{
#ifdef _DEBUG
	if (!m_bDisp)
	{
		return;
	}

	va_list list;			// 可変引数を処理する為に使用する変数
	char *pCur;
	char aBuf[256] = { "\0" };
	char aWk[32];

	// 可変引数にアクセスする前の初期処理
	va_start(list, fmt);

	pCur = fmt;
	for (; *pCur; ++pCur)
	{
		if (*pCur != '%')
		{
			sprintf_s(aWk, "%c", *pCur);
		}
		else
		{
			pCur++;

			switch (*pCur)
			{
			case 'd':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf_s(aWk, "%d", va_arg(list, int));
				break;

			case 'f':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf_s(aWk, "%f", va_arg(list, double));		// double型で指定
				break;

			case 's':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf_s(aWk, "%s", va_arg(list, char*));
				break;

			case 'c':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf_s(aWk, "%c", va_arg(list, char));
				break;

			default:
				sprintf_s(aWk, "%c", *pCur);
				break;
			}
		}

		strcat_s(aBuf, aWk);
	}

	// 可変引数にアクセスした後の終了処理
	va_end(list);

	if ((strlen(m_aStr) + strlen(aBuf)) < sizeof m_aStr - 1)
	{
		strcat_s(m_aStr, aBuf);
	}
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CDebugProc::Draw(void)
{
#ifdef _DEBUG
	if (m_bDisp)
	{
		CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
		IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

		{// フォントの背景描画
			pDevice->SetStreamSource(0, m_pVtxBuff.Get(), 0, sizeof(VertexDebug));
			pDevice->SetFVF(FVF_DEBUG_PROC);
			pDevice->SetTexture(0, m_pTexture.Get());
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}

		{// フォントの描画
			RECT rect = { 0, 0, static_cast<LONG>(pManager->GetWindowWidth()), static_cast<LONG>(pManager->GetWindowHeight()) };

			m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0x00));

			// バッファクリア
			memset(m_aStr, 0, sizeof m_aStr);
		}
	}
#endif
}

//=============================================================================
// 頂点バッファの生成
//=============================================================================
HRESULT CDebugProc::MakeVertexBuffer(void)
{

	HRESULT hr{};
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	hr = pDevice->CreateVertexBuffer(
		sizeof(VertexDebug) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_DEBUG_PROC,
		D3DPOOL_MANAGED,
		m_pVtxBuff.GetAddressOf(),
		NULL
	);
	if (FAILED(hr))
	{
		return hr;
	}

	{// 頂点バッファの中身を埋める
		VertexDebug* pVtx;

		// 頂点データのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点データの設定
		for (int i = 0; i < 4; i++)
		{
			pVtx->position.x = (i % 2) * m_fWidth;
			pVtx->position.y = (i / 2) * m_fHeight;
			pVtx->position.z = 0.0f;

			pVtx->rhw = 1.0f;

			pVtx->color = D3DCOLOR_ARGB(255, 255, 255, 255);

			pVtx->texcoord.x = float(i % 2);
			pVtx->texcoord.y = float(i / 2);

			pVtx++;
		}

		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// キーボード入力処理
//=============================================================================
void CDebugProc::IsInputKeyboard()
{

}

//=============================================================================
// マウス入力処理
//=============================================================================
void CDebugProc::IsInputMouse()
{

}

//=============================================================================
// テクスチャ生成処理
//=============================================================================
void CDebugProc::RenderTexture()
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	VertexDebug vtx[4];
	for (int i = 0; i < 4; i++)
	{
		int a = i & 0x01;
		int b = i >> 1;
		int c = !(a ^ b);

		vtx[i].position.x = a * m_cTexSize;
		vtx[i].position.y = b * m_cTexSize;
		vtx[i].position.z = 0.0f;

		vtx[i].rhw = 1.0f;

		vtx[i].color = D3DCOLOR_ARGB(255, c * 255, a * 255, b * 255);

		vtx[i].texcoord.x = float(i % 2);
		vtx[i].texcoord.y = float(i / 2);
	}

	ComPtr<IDirect3DSurface9> texSurface;
	ComPtr<IDirect3DSurface9> depthSurface;
	ComPtr<IDirect3DSurface9> backBufferSurface;
	ComPtr<IDirect3DSurface9> backDepthSurface;

	pDevice->GetRenderTarget(0, backBufferSurface.ReleaseAndGetAddressOf());
	pDevice->GetDepthStencilSurface(backDepthSurface.ReleaseAndGetAddressOf());

	D3DXCreateTexture(
		pDevice,
		m_cTexSize,
		m_cTexSize,
		0,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		m_pTexture.ReleaseAndGetAddressOf());

	pDevice->CreateDepthStencilSurface(
		m_cTexSize,
		m_cTexSize,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		depthSurface.ReleaseAndGetAddressOf(),
		NULL
	);
	pDevice->SetDepthStencilSurface(depthSurface.Get());

	m_pTexture->GetSurfaceLevel(0, texSurface.ReleaseAndGetAddressOf());

	pDevice->SetRenderTarget(0, texSurface.Get());

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	pDevice->BeginScene();

	pDevice->SetFVF(FVF_DEBUG_PROC);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(VertexDebug));

	pDevice->EndScene();

	pDevice->SetRenderTarget(0, backBufferSurface.Get());
	pDevice->SetDepthStencilSurface(backDepthSurface.Get());

}
