//=============================================================================
//
// インベントリの処理 [inventory.h]
// Author : Ueno
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"

class CScene2D;
class CItem;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_INVENTORY	10	//インベントリの最大数
#define MAX_HEADS	2	//アイテムの最大項目数

//*********************************************************
// メニュークラス
//*********************************************************
class CInventory : public CScene
{
public:
	CInventory(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	~CInventory();

	//インベントリの状態
	typedef enum
	{
		STATE_START = 0,
		STATE_SELECT,
		STATE_USING,
		STATE_MAX
	}INVENTORY_STATE;

	//インベントリの項目
	typedef enum
	{
		SELECT_USE = 0,
		SELECT_EQUIP,
		SELECT_DROP,
		SELECT_THROW,
		SELECT_BACK,
		SELECT_MAX
	}INVENTORY_SELECT;

	//インベントリの情報(構造体)
	typedef struct
	{
		int m_nType;
		int m_nHaves;
	}Inventory;

	static CInventory *Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);

	HRESULT Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT MakeVertexBuffer(void) {};

	//テクスチャの設定
	void BindTexture(const char* texID);

	//ポジションの設定・取得
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; };
	D3DXVECTOR3 GetPosition() { return m_pos; };

	//ピボットポイントの設定・取得
	void SetPivot(D3DXVECTOR3 pivot) { m_pivot = pivot; }
	D3DXVECTOR3 GetPivot(void) { return m_pivot; }

	//ロテーションの設定・取得
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation() { return m_rot; };

	//カラーの設定・取得
	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor(void) { return m_color; }

	void SetType(int nType) { m_nType = nType; }

	//キー操作
	void KeyControl(void);

	// インベントリクラスの状態を調べる
	void CheckState(void);

	//インベントリの読み込み
	void LoadInventory(void);

	//インベントリの更新
	void WriteInventory(void);

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// テクスチャへのポインタ
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// 頂点バッファへのポインタ
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;

	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス

	UINT m_column, m_row;
	UINT m_numFace, m_runIndex, m_numVertex;
	D3DXVECTOR3 m_pos;		 // 位置
	D3DXVECTOR3 m_pivot;	 // 基準点
	D3DXVECTOR3 m_rot;		 // 向き
	D3DXVECTOR3 m_move;		 // 
	D3DXVECTOR3 m_rotDest;   // 
	D3DXVECTOR4 m_uv;		 // 
	D3DXCOLOR m_color;		 // 色
	float m_fWidth;			 // 横の長さ
	float m_fHeight;		 // 縦の長さ
	int m_nType;			 // 種類
	int m_nColumnCount;		 // 列のカウント
	int m_nRowCount;		 // 行のカウント
	int m_nInventoryIndex;	 // インベントリのカーソル番号

	Inventory m_inventory[MAX_INVENTORY]; //インベントリの情報
	INVENTORY_SELECT m_inventorySelect;	  //インベントリの項目
	INVENTORY_STATE  m_inventoryState;	  //インベントリの状態

	CScene2D* m_pInventoryWindow;			// インベントリ枠
	CScene2D* m_pItemMenuWIndow;			// アイテムの枠
	CItem* m_pItem;
	static const char *m_apTextureName[];	// テクスチャ
};
