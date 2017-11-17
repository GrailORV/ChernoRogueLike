//=============================================================================
//
// デバッグ処理 [debugproc.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
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
char CDebugProc::m_aStr[LENGTH_STRING_BUFF] = { "\0" };
bool CDebugProc::m_bDisp = true;
const float CDebugProc::m_fWidth = 640.0f;
const float CDebugProc::m_fHeight = 640.0f;

//=============================================================================
// CDebugProcコンストラクタ
//=============================================================================
CDebugProc::CDebugProc()
{
	// バッファクリア
	memset(m_aStr, 0, sizeof m_aStr);
}

//=============================================================================
// CDebugProcデストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
void CDebugProc::Init()
{
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
	D3DXCreateFontIndirect(pDevice, &fontDesc, m_pFont.GetAddressOf());

	// 背景板ポリの生成
	MakeVertexBuffer();

}

//=============================================================================
// 開放処理
//=============================================================================
void CDebugProc::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CDebugProc::Update(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

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
			D3DXMATRIX projection
			{
				2.0f / pManager->GetWindowWidth(),0.0f,0.0f,0.0f,
				0.0f,-2.0f / pManager->GetWindowHeight(),0.0f,0.0f,
				0.0f,0.0f,1.0f,0.0f,
				-1.0f,1.0f,0.0f,1.0f
			};

			pDevice->SetTransform(D3DTS_WORLD, &projection);
			pDevice->SetStreamSource(0, m_pVtxBuff.Get(), 0, sizeof(VertexDebug));
			pDevice->SetFVF(FVF_DEBUG_PROC);
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
void CDebugProc::MakeVertexBuffer(void)
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
		return;
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

			pVtx->color = D3DCOLOR_ARGB(192, 0, 0, 0);

			pVtx++;
		}

		m_pVtxBuff->Unlock();
	}

}