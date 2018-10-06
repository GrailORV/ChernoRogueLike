//=============================================================================
//
// �v���C���[�̏��� [player.cpp]
// Author : ����@���
//
//=============================================================================
#include "stdafx.h"

#include "enemy.h"
#include "WinApp.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{

};

//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************


//=============================================================================
// CPlane����
//=============================================================================
CEnemy *CEnemy::Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CEnemy *pEnemy;

	pEnemy = new CEnemy;
	pEnemy->Init(nType, column, row, width, height, pos, rot, color);

	return pEnemy;
}

//=============================================================================
// CPlane�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority, CScene::OBJTYPE objtype) :
	CPlane(nPriority, objtype),
	m_rotDest(vector3NS::ZERO),
	m_prePos(vector3NS::ZERO),
	m_currentMapLocation(3, 3),
	m_moveFrameCnt(0)
{
	m_iCount = 0;
	m_iTurn = 0;
	m_frameCount = 0;
	m_pos = vector3NS::ZERO;
	m_move = vector3NS::ZERO;
	m_bMove = false;
	m_inputEnable = false;
	m_inputSecondEnable = false;
	m_bTurningPlayer = false;
}

//=============================================================================
// CPlane�f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CPlane::Init(nType, column, row, width, height, pos, rot, color);

	m_EnemyStatus.Life = 5;
	m_EnemyStatus.ATK = 1;
	m_EnemyStatus.nType = 0;

	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_ENEMY);
	m_pos = CMap::MapPositionLink(m_currentMapLocation);
	
	m_bUse = true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// �I�u�W�F�N�g��j��
	CPlane::Uninit();

}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	CDebugProc::Print("ENEMY LIFE : %d\n", m_EnemyStatus.Life);

	if (!m_bUse)
	{
		return;
	}

	m_pos = CMap::MapPositionLink(m_currentMapLocation);

	LifeCheck();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	if (m_bUse)
	{
		CPlane::Draw();
	}
}

//=============================================================================
// �c�胉�C�t���`�F�b�N
//=============================================================================
void CEnemy::LifeCheck(void)
{
	if (m_EnemyStatus.Life <= 0)
	{
		CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_FLOOR);
		m_bUse = false;
	}
}