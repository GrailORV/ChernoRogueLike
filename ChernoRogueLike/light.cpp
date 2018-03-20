//=============================================================================
//
// ライトの処理 [light.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "WinApp.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 静的変数
//*****************************************************************************


//=============================================================================
// CLightコンストラクタ
//=============================================================================
CLight::CLight() :m_dwRef(0)
{
}

//=============================================================================
// CLightデストラクタ
//=============================================================================
CLight::~CLight()
{
	Release();
}

//=============================================================================
// 参照コピー
//=============================================================================
HRESULT CLight::QueryInterface(REFIID riid, void FAR* FAR* ppvObject)
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
ULONG CLight::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// 終了処理
//=============================================================================
ULONG CLight::Release(void)
{
	if (--m_dwRef == 0)
	{
		delete this;
		return 0;
	}
	return m_dwRef;
}

//=============================================================================
// CLight初期化処理
//=============================================================================
HRESULT CLight::Init(void)
{
	// ライトの設定
	SetLight();

	return S_OK;
}

//=============================================================================
// CCamera更新処理
//=============================================================================
void CLight::Update(void)
{
}

//=============================================================================
// ライトの設定
//=============================================================================
void CLight::SetLight(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	{// キーライト
		// D3DLIGHT9構造体を0でクリアする
		ZeroMemory(&m_aLight, sizeof(D3DLIGHT9));

		// ライトのタイプの設定
		m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

		// 拡散光
		m_aLight[0].Diffuse.r = 1.0000000f;
		m_aLight[0].Diffuse.g = 0.9607844f;
		m_aLight[0].Diffuse.b = 0.8078432f;

		// 反射光
		m_aLight[0].Specular.r = 1.0000000f;
		m_aLight[0].Specular.g = 0.9607844f;
		m_aLight[0].Specular.b = 0.8078432f;

		// ライトの方向の設定
		m_aLight[0].Direction = Vector3(-0.5265408f, -0.5735765f, -0.6275069f);

		// ライトをレンダリングパイプラインに設定
		pDevice->SetLight(0, &m_aLight[0]);

		// ライトの設定
		pDevice->LightEnable(0, TRUE);
	}

	{// フィルライト
		// D3DLIGHT9構造体を0でクリアする
		ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));

		// ライトのタイプの設定
		m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

		// 拡散光
		m_aLight[1].Diffuse.r = 0.3231373f;
		m_aLight[1].Diffuse.g = 0.3607844f;
		m_aLight[1].Diffuse.b = 0.3937255f;

		// 反射光
		m_aLight[1].Specular.r = 0.3231373f;
		m_aLight[1].Specular.g = 0.3607844f;
		m_aLight[1].Specular.b = 0.3937255f;

		// ライトの方向の設定
		m_aLight[1].Direction = Vector3(0.4545195f, -0.7660444f, 0.4545195f);

		// ライトをレンダリングパイプラインに設定
		pDevice->SetLight(1, &m_aLight[1]);

		// ライトの設定
		pDevice->LightEnable(1, TRUE);
	}

	{// バックライト
		// D3DLIGHT9構造体を0でクリアする
		ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));

		// ライトのタイプの設定
		m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

		// 拡散光
		m_aLight[2].Diffuse.r = 0.9647059f;
		m_aLight[2].Diffuse.g = 0.7607844f;
		m_aLight[2].Diffuse.b = 0.4078432f;

		// 反射光
		m_aLight[2].Specular.r = 0.0f;
		m_aLight[2].Specular.g = 0.0f;
		m_aLight[2].Specular.b = 0.0f;

		// ライトの方向の設定
		m_aLight[2].Direction = Vector3(0.7198464f, 0.3420201f, 0.6040227f);

		// ライトをレンダリングパイプラインに設定
		pDevice->SetLight(2, &m_aLight[2]);

		// ライトの設定
		pDevice->LightEnable(2, TRUE);
	}

	{// 環境光
		XColor ambientColor;
		ambientColor.r = 0.05333332f;
		ambientColor.g = 0.09882354f;
		ambientColor.b = 0.01819608f;
		pDevice->SetRenderState(D3DRS_AMBIENT, ambientColor);
	}

	// ライティングモード
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

