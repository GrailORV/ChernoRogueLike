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
		CScene::ReleaseAll();
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

	// サウンドの初期化
	m_pSound = new CSound;
	m_pSound->Init();

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