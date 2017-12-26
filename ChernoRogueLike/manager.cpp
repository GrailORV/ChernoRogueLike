//=============================================================================
//
// マネージャーの処理 [manager.cpp]
// Author : 
//
//=============================================================================
#include "stdafx.h"
#include "resource.h"

#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "plane.h"
#include "player.h"
#include "model.h"

//=============================================================================
// CManagerコンストラクタ
//=============================================================================
CManager::CManager(UINT width, UINT height) :
	m_width(width),
	m_height(height),
	m_dwRef(0)
{
}

//=============================================================================
// CManagerデストラクタ
//=============================================================================
CManager::~CManager()
{
	Release();
}

//=============================================================================
// 参照コピー
//=============================================================================
HRESULT CManager::QueryInterface(REFIID riid, void FAR* FAR* ppvObject)
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
ULONG CManager::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// 終了処理
//=============================================================================
ULONG CManager::Release(void)
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
HRESULT CManager::Init(HINSTANCE hInstance, HWND hwnd, BOOL bWindow)
{
	HRESULT hr{};

	// レンダリング処理の初期化処理
	m_pRenderer = new CRenderer;
	hr = m_pRenderer->Init(bWindow);
	if (FAILED(hr))
	{
		return hr;
	}

	// テクスチャマネージャの初期化
	m_pTextureManager = new CTextureManager;
	hr = m_pTextureManager->Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// メッシュマネージャの初期化
	m_pModelManager = new CModelManager;
	hr = m_pModelManager->Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// キーボードの初期化処理
	m_pInputKeyboard = new CInputKeyboard;
	hr = m_pInputKeyboard->Init(hInstance, hwnd);
	if (FAILED(hr))
	{
		return hr;
	}

	// マウスの初期化処理
	m_pInputMouse = new CInputMouse;
	hr = m_pInputMouse->Init(hInstance, hwnd);
	if (FAILED(hr))
	{
		return hr;
	}

	// ジョイパッドの初期化処理
	m_pInputJoypad = new CInputJoypad;
	hr = m_pInputJoypad->Init(hInstance, hwnd);
	if (FAILED(hr))
	{
		return hr;
	}

	// サウンドの初期化
	m_pSound = new CSound;
	hr = m_pSound->Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// カメラの初期化
	m_pCamera = new CCamera;
	hr = m_pCamera->Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// ライトの初期化処理
	m_pLight = new CLight;
	hr = m_pLight->Init();
	if (FAILED(hr))
	{
		return hr;
	}

#ifdef _DEBUG
	// デバッグフォントの初期化
	m_pDebugProc = new CDebugProc;
	hr = m_pDebugProc->Init();
	if (FAILED(hr))
	{
		return hr;
	}
#endif

	m_pTextureManager->LoadSceneTex(m_mode);
	m_pModelManager->LoadSceneMesh(m_mode);

	m_pScene = CScene2D::Create(0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), vector3NS::ZERO, 160.0f, 200.0f, colorNS::_WHITE);
	m_pScene->BindTexture("tex_haruka_princess");

	CPlane* pPlane = CPlane::Create(0, 4, 4, 640.0f, 800.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), vector3NS::ZERO);
	pPlane->BindTexture("tex_haruka_princess");

	CPlayer* pPlayer = CPlayer::Create(0, 4, 4, 50.f, 50.f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), vector3NS::ZERO);
	pPlayer->BindTexture("ore");

	CModel* pModel = CModel::Create(0, "torus", D3DXVECTOR3(30.0f, 50.0f, 70.0f), D3DXVECTOR3(D3DX_PI / 4.0f, D3DX_PI / 4.0f, D3DX_PI / 6.0f));

	m_pSound->Play(CSound::BGM_LABEL_NO_CURRY);

	return hr;
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	// キーボードの更新処理
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}

	// マウスの更新処理
	if (m_pInputMouse)
	{
		m_pInputMouse->Update();
	}

	// ジョイパッドの更新処理
	if (m_pInputJoypad)
	{
		m_pInputJoypad->Update();
	}

	// サウンドの更新処理
	if (m_pSound)
	{
		m_pSound->Update();
	}

	// カメラの更新処理
	if (m_pCamera)
	{
		m_pCamera->Update();
	}

	// ライトの更新処理
	if (m_pLight)
	{
		m_pLight->Update();
	}

	// オブジェクトの更新処理
	if (m_pRenderer)
	{
		m_pRenderer->Update();
	}

	CDebugProc::Print("F1を押すと消えるよ（はーと\n");

	if (m_pInputKeyboard->GetKeyTrigger(DIK_RETURN))
	{
		if (m_pScene)
		{
			m_pScene->Uninit();
			m_pScene = NULL;
		}
		else
		{
			m_pScene = CScene2D::Create(0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), vector3NS::ZERO, 160.0f, 200.0f, colorNS::_WHITE);
			m_pScene->BindTexture("tex_haruka_princess");
		}
	}

#ifdef _DEBUG
	// デバッグフォントの更新
	m_pDebugProc->Update();
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// オブジェクトの更新処理
	if (m_pRenderer)
	{
		m_pRenderer->Draw();
	}
}