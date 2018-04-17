//=============================================================================
//
// プレーンの処理 [plane.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "plane.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef enum
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	NONE
}PLAYER_MOVE;

//*********************************************************
// プレイヤークラス
//*********************************************************
class CPlayer : public CPlane
{
public:
	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLANE);
	~CPlayer();

	static CPlayer *Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);

	HRESULT Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(int nType) { m_nType = nType; }

	int GetTurn(void) { return m_iTurn; }

private:
	static const int FRAME_MAX;
	static const UINT MOVE_FRAME;

	void InputMove(bool& inputEnable);

	// マップのサイズ
	static const int MAP_SIZE_X = 50;
	static const int MAP_SIZE_Z = 50;

	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_prePos;
	D3DXVECTOR3 m_rotDest;

	Vector3 m_moveBuff;
	bool m_bMove;
	bool m_inputEnable;
	bool m_inputSecondEnable;

	int m_posX;
	int m_posZ;

	int m_frameCount;
	UINT m_moveFrameCnt;
	int m_iCount;
	int m_iTurn;

	int m_nType;							// 種類

	// マップサイズ（２次元配列）
	int m_MapSize[MAP_SIZE_X][MAP_SIZE_Z];
};
