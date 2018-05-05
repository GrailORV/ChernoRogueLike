//=============================================================================
//
// �A�C�e���̏��� [item.h]
// Author : UENO
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ITEM	10	//�A�C�e���̍ő吔
#define MAX_HEADS	2	//�A�C�e���̍ő區�ڐ�

//*********************************************************
// �A�C�e���N���X
//*********************************************************
class CItem
{
public:
	CItem();
	~CItem();

	//�A�C�e���̎��
	typedef enum 
	{
		ITEM_NONE = 0, 
		ITEM_HERB,
		ITEM_WEAPON,
		ITEM_ARMOR
	}ITEM_TYPE;

	//�A�C�e���̏��(�\����)
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
