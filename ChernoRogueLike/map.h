//=============================================================================
//
// マップ処理 [map.h]
// Author : 小川 朔哉
//
//=============================================================================
#pragma once
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAP_SIZE_X	3
#define MAP_SIZE_Z	3

//*****************************************************************************
// 構造体
//*****************************************************************************


//*****************************************************************************
// マップクラス
//*****************************************************************************
class CMap : public CScene
{
public:
	CMap();
	~CMap();

	static CMap *Create(int nType, D3DXVECTOR3 pos);

	HRESULT Init(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetMapPos(void) { return m_nMapSize[m_nMapX][m_nMapZ]; }

	Vector3 GetPosition(void) { return m_pos; }
	void SetPosition(Vector3 pos) { m_pos = pos; }

	Vector3 GetRotation(void) { return m_rot; }
	void SetRotation(Vector3 rot) { m_rot = rot; }

protected:
	Vector3 m_pos;
	Vector3 m_rot;

	int m_nMapX;
	int m_nMapZ;
	static int m_nMapSize[MAP_SIZE_X][MAP_SIZE_Z];
};