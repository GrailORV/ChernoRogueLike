//=============================================================================
//
// �v���[���̏��� [plane.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "plane.h"


//*********************************************************
// 3D�I�u�W�F�N�g�N���X
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

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }

	void SetUV(float width, float height, float x, float y) { m_uv = D3DXVECTOR4(width, height, x, y); }
	D3DXVECTOR4 GetUV(void) { return m_uv; }

	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor(void) { return m_color; }

	void SetType(int nType) { m_nType = nType; }

	int GetTurn(void) { return m_iTurn; }

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;
	static const int FLAME_MAX;

	// �}�b�v�̃T�C�Y
	static const int MAP_SIZE_X = 50;
	static const int MAP_SIZE_Z = 50;

	UINT m_numFace, m_numIndex, m_numVertex;
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rotDest;
	float m_width;
	float m_height;
	D3DXVECTOR4 m_uv;
	D3DXCOLOR m_color;

	Vector3 m_moveBuff;
	bool m_bMove;
	bool m_inputEnable;
	bool m_inputSecondEnable;

	int m_frameCount;
	int m_iCount;
	int m_iTurn;

	int m_nType;							// ���

	// �}�b�v�T�C�Y�i�Q�����z��j
	int m_MapSize[MAP_SIZE_X][MAP_SIZE_Z];
};
