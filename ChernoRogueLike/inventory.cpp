//=============================================================================
//
// インベントリの処理 [inventory.cpp]
// Author :UENO
//
//=============================================================================
#include "stdafx.h"

#include "inventory.h"
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "textureManager.h"
#include "WinApp.h"
#include "input.h"
#include "debugproc.h"
#include "item.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_INVENTORY	(10)


//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// 静的変数
//*****************************************************************************
const char *CInventory::m_apTextureName[] =
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
CInventory *CInventory::Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CInventory *pInventory;

	pInventory = new CInventory;
	pInventory->Init(nType, column, row, width, height, pos, rot, color);

	return pInventory;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CInventory::CInventory(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pInventoryWindow = NULL;
	m_pItem = NULL;

	m_pos = vector3NS::ZERO;
	m_rot = vector3NS::ZERO;

	m_nType = 0;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		m_inventory[i].m_nHaves = 0;
		m_inventory[i].m_nType = 0;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CInventory::~CInventory()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInventory::Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	m_nColumnCount = 0;
	m_nRowCount = 0;
	m_nInventoryIndex = 0;
	m_fWidth = width / (float)(MAX_INVENTORY);
	m_fHeight = height / (float)(MAX_INVENTORY);
	m_column = column;
	m_row = MAX_INVENTORY / m_column + MAX_INVENTORY % m_column;
	m_inventoryState = STATE_SELECT;
	
	//インベントリの読み込み
	LoadInventory();
	WriteInventory();
	{// インベントリウィンドウの生成
		m_pInventoryWindow = CScene2D::Create(nType, pos, rot, width, height, color);
		m_pInventoryWindow->BindTexture("ore");
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInventory::Uninit(void)
{
	{// インベントリウィンドウの終了
		if (m_pInventoryWindow != NULL)
		{
			m_pInventoryWindow->Uninit();
			m_pInventoryWindow = NULL;
		}
	}

	if (m_pItem != NULL)
	{
		delete m_pItem;
		m_pItem = NULL;
	}

	// オブジェクトを破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInventory::Update(void)
{
	CManager* pManager;
	CInputKeyboard *pInputKeyboard;

	//マネージャー取得
	pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	//キーボード取得
	pInputKeyboard = pManager->GetInputKeyboard();

	//インベントリの操作
	KeyControl();

	//インデックス番号（デバッグ）
	CDebugProc::Print("%d番目_ID：%d_所持数：%d\n", m_nInventoryIndex,m_inventory[m_nInventoryIndex].m_nType, m_inventory[m_nInventoryIndex].m_nHaves);

	// インベントリの状態を調べる


	//カーソルの位置更新

	if (m_pItem)
	{
		m_pItem->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CInventory::Draw()
{
}

//=============================================================================
// インベントリの状態を調べる
//=============================================================================
void CInventory::CheckState()
{
	switch (m_inventoryState)
	{
	case STATE_START:
		break;
	case STATE_SELECT:
		break;
	case STATE_USING:
		break;
	case STATE_MAX:
		break;
	}
}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CInventory::BindTexture(const char* texID)
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
// インベントリ画面のキー操作
//=============================================================================
void CInventory::KeyControl(void)
{
	CManager* pManager;
	CInputKeyboard *pInputKeyboard;
	//マネージャー取得
	pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	//キーボード取得
	pInputKeyboard = pManager->GetInputKeyboard();


	//インベントリの操作
	if (pInputKeyboard->GetKeyTrigger(DIK_O))
	{//カーソル移動（上）
		m_nInventoryIndex--;
	}
	if (pInputKeyboard->GetKeyTrigger(DIK_P))
	{//カーソル移動（下）
		m_nInventoryIndex++;
	}

	//インベントリの下限と上限の設定
	if (m_nInventoryIndex < 0  || m_inventory[0].m_nHaves == 0)
	{
		m_nInventoryIndex = 0;
	}
	else if (m_inventory[m_nInventoryIndex].m_nHaves == 0)
	{
		m_nInventoryIndex--;
	}
	else if (m_nInventoryIndex >= MAX_INVENTORY)
	{
		m_nInventoryIndex = MAX_INVENTORY - 1;
	}

	

}

//=============================================================================
// インベントリの読み込み
//=============================================================================
void CInventory::LoadInventory(void)
{
	int InventoryIndex = 0;

	//ファイル構造体
	FILE *fp;

	//ファイル名
	char * fname = "data/CSV/Inventory.csv";

	//ファイルオープン
	fp = fopen(fname, "r");

	//ファイルが読み込めない場合
	if (!fp)
	{
		MessageBox(NULL, "アイテムデータを読み込めませんでした。\n", NULL, MB_OK);
	}

	for (int nCntInventory_Y = 0; nCntInventory_Y < MAX_ITEM; nCntInventory_Y++)
	{
		//種類の読み込み
		fscanf_s(fp, "%d,", &m_inventory[InventoryIndex].m_nType);

		//アイテムの所持状態を読み込み
		fscanf_s(fp, "%d,", &m_inventory[InventoryIndex].m_nHaves);

		//もしアイテムの所持数が0の場合（持ってないと同義）
		if (m_inventory[InventoryIndex].m_nHaves == 0)
		{
			//情報を詰める
			InventoryIndex--;
		}

		InventoryIndex++;
	}
	
	//ファイルクローズ
	fclose(fp);

}

//=============================================================================
// インベントリの書き込み
//=============================================================================
void CInventory::WriteInventory(void)
{
	int InventoryIndex = 0;

	//ファイル構造体
	FILE *fp;

	//ファイル名
	char *fname = "data/CSV/Inventory.csv";
	char s = 'a';
	//ファイルオープン
	fp = fopen(fname, "w");

	//ファイルが読み込めない場合
	if (!fp)
	{
		MessageBox(NULL, "アイテムデータを読み込めませんでした。\n", NULL, MB_OK);
	}

	//ヘッダーの書き込み
	fprintf(fp, "種類,所持数\n");
	
	for (int nCntInventory_Y = 0; nCntInventory_Y < MAX_ITEM; nCntInventory_Y++)
	{
		//種類の書き込み
		fprintf(fp, "%d,",m_inventory[InventoryIndex].m_nType);
		
		//アイテムの所持状態を書き込み
		fprintf(fp, "%d\n",m_inventory[InventoryIndex].m_nHaves);
	}

	//ファイルクローズ
	fclose(fp);
}