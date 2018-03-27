//=============================================================================
//
// マップ処理 [map.h]
// Author : 小川 朔哉
//
//=============================================================================
#pragma once
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体
//*****************************************************************************


//*****************************************************************************
// マップクラス
//*****************************************************************************
class CMap : public CScene2D
{
public:
	CMap();
	~CMap();

	static CMap *Create(int nType, D3DXVECTOR3 pos);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }

	typedef struct
	{

	};

private:
	//D3DXVECTOR3 m_pos;
	//D3DXVECTOR3 m_rot;
};