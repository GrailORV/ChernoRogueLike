//=============================================================================
//
// アイテムの処理 [item.h]
// Author : UENO
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ITEM	10	//アイテムの最大数
#define MAX_HEADS	2	//アイテムの最大項目数

//*********************************************************
// アイテムクラス
//*********************************************************
class CItem
{
public:
	CItem();
	~CItem();

	//アイテムの種類
	typedef enum 
	{
		ITEM_NONE = 0, 
		ITEM_HERB,
		ITEM_WEAPON,
		ITEM_ARMOR
	}ITEM_TYPE;

	//アイテムの情報(構造体)
	typedef struct
	{
		int m_nType;
		int m_nHaves;
	}Item;

	HRESULT Init(void);
	void Uninit();
	void Update();
	void SetItemNumber(int num) { m_nItemNumber = num; }

private:
	Item m_item[MAX_ITEM];
	int m_nItemNumber;
};
