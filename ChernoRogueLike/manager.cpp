//=============================================================================
//
// マネージャーの処理 [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// CManagerコンストラクタ
//=============================================================================
CManager::CManager(UINT width, UINT height) :
	m_width(width),
	m_height(height)
{
}

//=============================================================================
// CManagerデストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hwnd, BOOL bWindow)
{
	HRESULT hr{};

	// レンダリング処理の初期化処理
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hwnd, bWindow);

	// キーボードの初期化処理
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hwnd);

	// マウスの初期化処理
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hwnd);

	// ジョイパッドの初期化処理
	m_pInputJoypad = new CInputJoypad;
	m_pInputJoypad->Init(hInstance, hwnd);

	// ライトの初期化処理
	m_pLight = new CLight;
	m_pLight->Init();

#ifdef _DEBUG
	// デバッグフォントの初期化
	m_pDebugProc = new CDebugProc;
	m_pDebugProc->Init();
#endif

	CScene2D* pScene = CScene2D::Create(0, D3DXVECTOR3(352.0f, 0.0f, 0.0f), vector3NS::ZERO, 576.0f, 720.0f,colorNS::_WHITE);
	pScene->LoadTexture("data/TEXTURE/tex_haruka_princess.jpg");

	CScene2D::Create(0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), vector3NS::ZERO, 100.0f, 100.0f, colorNS::_WHITE);
	CScene2D::Create(0, D3DXVECTOR3(100.0f, 100.0f, 0.0f), vector3NS::ZERO, 100.0f, 100.0f, colorNS::_BLACK);
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	CScene::ReleaseAll();
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