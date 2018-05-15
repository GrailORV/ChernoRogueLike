//=============================================================================
//
// �v���[���̏��� [plane.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "plane.h"
#include "map.h"

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct
{
	int8_t Life;
	uint8_t ATK;
}PLAYER_STATUS;

struct INT8_2
{
	int8_t x, z;
	INT8_2(int8_t inX, int8_t inZ)
	{
		x = inX; z = inZ;
	}
};

//*********************************************************
// �v���C���[�N���X
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

	void MovePosition(UINT moveFrame);
	void MoveMap(INT8_2 moveBuff);

private:
	static const int FRAME_MAX;
	static const UINT MOVE_FRAME;

	MapLocation m_currentMapLocation;
	PLAYER_STATUS m_Status;

	void Move(void);
	void InputMove(bool& inputEnable);
	void Attack();
	void Item();
	void Behavior();

	Vector3 m_move;
	Vector3 m_prePos;
	Vector3 m_rotDest;

	uint8_t m_ATK;

	INT8_2 m_front;				// �����Ă����
	INT8_2 m_moveBuff;

	UINT m_moveFrameCnt;

	bool m_bMove;
	bool m_inputEnable;
	bool m_inputSecondEnable;
	bool m_bTurningPlayer;					// �����]���t���O

	int m_frameCount;
	int m_iCount;
	int m_iTurn;
	int m_nType;							// ���

};
