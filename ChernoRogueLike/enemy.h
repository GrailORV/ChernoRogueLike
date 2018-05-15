//=============================================================================
//
// プレーンの処理 [plane.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "plane.h"
#include "map.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	int8_t Life;
	uint8_t ATK;
	int nType;
}ENEMY_STATUS;

//*********************************************************
// 3Dオブジェクトクラス
//*********************************************************
class CEnemy : public CPlane
{
public:


	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLANE);
	~CEnemy();

	static CEnemy *Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);

	HRESULT Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(int nType) { m_nType = nType; }

	uint8_t GetLife(void) { return m_EnemyStatus.Life; }
	void AddLife(uint8_t Num) { m_EnemyStatus.Life += Num; }

private:

	MapLocation m_currentMapLocation;

	void LifeCheck(void);

	Vector3 m_move;
	Vector3 m_prePos;
	Vector3 m_rotDest;

	UINT m_moveFrameCnt;

	bool m_bMove;
	bool m_inputEnable;
	bool m_inputSecondEnable;
	bool m_bTurningPlayer;					// 方向転換フラグ
	bool m_bUse;

	int m_frameCount;
	int m_iCount;
	int m_iTurn;
	int m_nType;							// 種類

	ENEMY_STATUS m_EnemyStatus;

};
