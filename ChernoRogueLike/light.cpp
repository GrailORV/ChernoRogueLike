//=============================================================================
//
// ライトの処理 [light.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
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
CLight::CLight()
{
}

//=============================================================================
// CLightデストラクタ
//=============================================================================
CLight::~CLight()
{
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
// CLight終了処理
//=============================================================================
void CLight::Uninit(void)
{
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
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&m_aLight, sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	m_aLight[0].Diffuse.r = 1.0f;
	m_aLight[0].Diffuse.g = 1.0f;
	m_aLight[0].Diffuse.b = 1.0f;

	// 環境光
	m_aLight[0].Ambient.r = 0.50f;
	m_aLight[0].Ambient.g = 0.50f;
	m_aLight[0].Ambient.b = 0.50f;

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[0].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(0, &m_aLight[0]);

	// ライトの設定
	pDevice->LightEnable(0, TRUE);


	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	m_aLight[1].Diffuse.r = 0.25f;
	m_aLight[1].Diffuse.g = 0.25f;
	m_aLight[1].Diffuse.b = 0.25f;

	// 環境光
	m_aLight[1].Ambient.r = 0.20f;
	m_aLight[1].Ambient.g = 0.20f;
	m_aLight[1].Ambient.b = 0.20f;

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[1].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(1, &m_aLight[1]);

	// ライトの設定
	pDevice->LightEnable(1, TRUE);

	// ライティングモード
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
}
