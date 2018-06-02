//=============================================================================
//
// メニュー処理 [Menu.h]
// Author : Ueno
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"

class CScene2D;
class CInventory;

//*********************************************************
// メニュークラス
//*********************************************************
class CMenu : public CScene
{
public:
	CMenu(int nPriority = 3,OBJTYPE objType = OBJTYPE_NONE);
	~CMenu();

	//メニューの選択項目
	typedef enum 
	{
		SELECT_ITEM = 0,
		SELECT_STATUS,
		SELECT_SHOP,
		SELECT_CLOSE,
		SELECT_TEST,
		SELECT_TEST2,
		SELECT_MAX
	}MENU_SELECT;

	//メニュー画面の状態
	typedef enum
	{
		STATE_START = 0,
		STATE_SELECT,
		STATE_USING,
		STATEMAX
	}MENU_STATE;

	static CMenu *Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);

	HRESULT Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CreateMenuWindow(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	HRESULT MakeVertexBuffer(void) {};

	void BindTexture(const char* texID);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; };
	D3DXVECTOR3 GetPosition() { return m_pos; };

	void SetPivot(D3DXVECTOR3 pivot) { m_pivot = pivot; }
	D3DXVECTOR3 GetPivot(void) { return m_pivot; }

	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation() { return m_rot; };

	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor(void) { return m_color; }

	void SetType(int nType) { m_nType = nType; }

	void KeyControl(void);

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// テクスチャへのポインタ
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// 頂点バッファへのポインタ
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;

	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス

	UINT m_column, m_row;	 // 列数、行数
	UINT m_numFace, m_runIndex, m_numVertex;
	D3DXVECTOR3 m_pos;		 // 位置
	D3DXVECTOR3 m_pivot;	 // 基準点
	D3DXVECTOR3 m_rot;		 // 向き
	D3DXVECTOR3 m_move;		 // 
	D3DXVECTOR3 m_rotDest;   // 
	D3DXVECTOR4 m_uv;		 // 
	D3DXCOLOR m_color;		 // 色
	MENU_STATE m_menuState;	 // メニューの状態
	float m_fWidth;			 // 横の長さ
	float m_fHeight;		 // 縦の長さ
	int m_nType;			 // 種類
	int m_nMenuIndex;		 // メニューのカーソル番号

	CScene2D* m_pMenuWindow;				// メニュー枠
	CScene2D* m_apMenuSelect[SELECT_MAX];	// メニューの選択肢
	CScene2D* m_pMenuCursor;				// メニューの選択アイコン	
	CInventory* m_pInventory;				// インベントリ

	static const char *m_apTextureName[];	// テクスチャ
};