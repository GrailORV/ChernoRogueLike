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

class CMenu;
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

<<<<<<< HEAD
private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;

	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X

	UINT m_column, m_row;
	UINT m_numFace, m_numIndex, m_numVertex;
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_pivot;
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rotDest;
	float m_width;
	float m_height;
	D3DXVECTOR4 m_uv;
	D3DXCOLOR m_color;
	CMenu* m_pMenu;
=======
	UINT m_moveFrameCnt;

	bool m_bMove;
	bool m_inputEnable;
	bool m_inputSecondEnable;
	bool m_bTurningPlayer;					// �����]���t���O

	int m_frameCount;
>>>>>>> c18c7c42d4c821fa7b84c67b0387663369610a35
	int m_iCount;
	int m_iTurn;
	int m_nType;							// ���
};
