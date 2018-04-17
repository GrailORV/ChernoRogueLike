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
typedef enum
{
	OBJ_NONE = 0,
	OBJ_WALL,	
	OBJ_PLAYER,
}OBJ_TYPE;

//*****************************************************************************
// マップクラス
//*****************************************************************************
class CMap : public CScene
{
public:
	CMap();
	~CMap();

	static CMap *Create(int MapSizeX, int MapSizeZ);

	HRESULT Init(int MapSizeX, int MapSizeZ);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MapOpen(void);

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
	int m_nMapSize[MAP_SIZE_X][MAP_SIZE_Z];
};