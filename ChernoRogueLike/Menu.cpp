//=============================================================================
//
// メニューの処理 [Menu.cpp]
// Author :UENO
//
//=============================================================================
#include "stdafx.h"

#include "Menu.h"
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "textureManager.h"
#include "WinApp.h"
#include "input.h"
#include "inventory.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE		10.f

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// 静的変数
//*****************************************************************************
const char *CMenu::m_apTextureName[] =
{
	//メニュー選択項目
	"ore",
	"ore",
	"ore",
	"ore",
	"ore",
	"ore",
	"ore"
};



//=============================================================================
// 生成
//=============================================================================
CMenu *CMenu::Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CMenu *pMenu;

	pMenu = new CMenu;
	pMenu->Init(nType,  column, row, width, height, pos, rot, color);

	return pMenu;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMenu::CMenu(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority,objType)
{
	//メニュー枠
	m_pMenuWindow = NULL;
	//メニュー選択項目
	for (int nCountSelect = 0; nCountSelect < SELECT_MAX; nCountSelect++)
	{
		m_apMenuSelect[nCountSelect] = NULL;
	}
	//メニューカーソル
	m_pMenuCursor = NULL;

	//インベントリ
	m_pInventory = NULL;

	//ステータス
	m_menuState = STATE_START;

	m_pos = vector3NS::ZERO;
	m_rot = vector3NS::ZERO;

	m_nType = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMenu::~CMenu()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMenu::Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{	
	//状態の初期化
	m_menuState = STATE_SELECT;

	//カーソルの初期位置
	m_nMenuIndex = SELECT_ITEM;

	//選択項目の縦幅と横幅
	m_fWidth = width / (float)(SELECT_MAX);
	m_fHeight = height / (float)(SELECT_MAX);
	
	//行列の設定
	m_column = column;
	m_row = SELECT_MAX / m_column;
	if (SELECT_MAX % m_column != 0)
	{
		m_row = m_row + 1;
	}

	{// メニューウィンドウの生成
		m_pMenuWindow = CScene2D::Create(nType, pos, rot, width, height, color);
		m_pMenuWindow->BindTexture("ore");
	}

	{// 選択項目の生成
		for (int nCountSelect = 0; nCountSelect < SELECT_MAX; nCountSelect++)
		{
			m_apMenuSelect[nCountSelect] = CScene2D::Create(nType,
				pos + D3DXVECTOR3((width / (float)m_column) * (nCountSelect % m_column) + (width / (m_column * 2.0f) - (m_fWidth / 2.0f)),
				(height / (float)m_row) * (nCountSelect / m_column) + (height / (m_row * 2)) - (m_fHeight / 2.0f),
					0),
				rot,
				m_fWidth,
				m_fHeight,
				color);
			
			//テクスチャの設定
			m_apMenuSelect[nCountSelect]->BindTexture(m_apTextureName[nCountSelect]);
		}
	}

	{//選択アイコンの生成
		m_pMenuCursor = CScene2D::Create(1, m_apMenuSelect[m_nMenuIndex]->GetPosition(), D3DXVECTOR3(0,0,0), m_fWidth, m_fHeight, color);
		m_pMenuCursor->BindTexture("tex_haruka_princess");
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMenu::Uninit(void)
{
	{// メニューウィンドウの終了
		if (m_pMenuWindow != NULL)
		{
			m_pMenuWindow->Uninit();
			m_pMenuWindow = NULL;
		}
	}

	{//選択項目の終了
		for (int nCountSelect = 0; nCountSelect < SELECT_MAX; nCountSelect++)
		{
			if (m_apMenuSelect[nCountSelect] != NULL)
			{
				m_apMenuSelect[nCountSelect]->Uninit();
				m_apMenuSelect[nCountSelect] = NULL;
			}
		}
	}

	{//選択アイコンの終了
		if (m_pMenuCursor != NULL)
		{
			m_pMenuCursor->Uninit();
			m_pMenuCursor = NULL;
		}
	}
	// オブジェクトを破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMenu::Update(void)
{
	//キー操作
	KeyControl();

	//インデックス番号（デバッグ）
	CDebugProc::Print("メニューインデックス : %d\n", m_nMenuIndex);

	//カーソルの位置更新
	m_pMenuCursor->SetPosition(m_apMenuSelect[m_nMenuIndex]->GetPosition());

}

//=============================================================================
// 描画処理
//=============================================================================
void CMenu::Draw()
{
}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CMenu::BindTexture(const char* texID)
{
	if (!texID)
	{
		m_pTexture.Reset();
		return;
	}

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	pTextureManager->BindtextureFromString(texID, m_pTexture.GetAddressOf());
}

//=============================================================================
// メニュー画面のキー操作
//=============================================================================
void CMenu::KeyControl(void)
{
	CManager* pManager;
	CInputKeyboard *pInputKeyboard;
	//マネージャー取得
	pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	//キーボード取得
	pInputKeyboard = pManager->GetInputKeyboard();


	//メニューの操作
	switch (m_menuState)
	{
	case STATE_SELECT:
		if (pInputKeyboard->GetKeyTrigger(DIK_O))
		{//カーソル移動（左）
			m_nMenuIndex--;

			if ((m_nMenuIndex + 1) % m_column == 0)
			{
				m_nMenuIndex += m_column;
			}
			if (m_nMenuIndex >= SELECT_MAX)
			{
				m_nMenuIndex = SELECT_MAX - 1;
			}
		}
		if (pInputKeyboard->GetKeyTrigger(DIK_P))
		{//カーソル移動（右）
			m_nMenuIndex++;

			if (m_nMenuIndex % m_column == 0)
			{
				m_nMenuIndex -= m_column;
			}
			if (m_nMenuIndex >= SELECT_MAX)
			{
				m_nMenuIndex = SELECT_MAX - 1;
			}
		}
		if (pInputKeyboard->GetKeyTrigger(DIK_I))
		{//カーソル移動（下）
			m_nMenuIndex += m_column;

			if (m_nMenuIndex >= SELECT_MAX)
			{
				m_nMenuIndex = m_nMenuIndex % m_column;
			}
		}
		if (pInputKeyboard->GetKeyTrigger(DIK_U))
		{//カーソル移動（上）
			int nSelectMax = m_column * m_row;

			m_nMenuIndex -= m_column;

			if (m_nMenuIndex < 0)
			{
				m_nMenuIndex = nSelectMax + m_nMenuIndex;

				if (m_nMenuIndex >= SELECT_MAX)
				{
					m_nMenuIndex -= m_column;
				}
			}
		}
		if (pInputKeyboard->GetKeyTrigger(DIK_L))
		{//決定ボタン
			if (!m_pInventory)
			{
				switch (m_nMenuIndex)
				{
				case SELECT_ITEM:
					//インベントリを表示
					m_pInventory = CInventory::Create(1, 1, 1, 300.f, 500.0f, D3DXVECTOR3(100.0f, 0, 0), vector3NS::ZERO, colorNS::_WHITE);
					
					//状態の変更(使用中に)
					m_menuState = STATE_USING;
					break;
				}
			}
		}
		break;
	case STATE_USING:
		break;
	default:
		break;
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_K))
	{//キャンセルボタン（デバッグ）
		if (m_pInventory)
		{//インベントリを削除
			m_pInventory->Uninit();
			m_pInventory = NULL;

			//状態の変更（選択中に)
			m_menuState = STATE_SELECT;
		}
	}
}