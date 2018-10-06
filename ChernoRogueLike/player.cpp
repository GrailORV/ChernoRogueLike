//=============================================================================
//
// �v���C���[�̏��� [player.cpp]
// Author : ����@���
//
//=============================================================================
#include "stdafx.h"

#include "player.h"
#include "WinApp.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
<<<<<<< HEAD
#include "Menu.h"
=======
#include "enemy.h"
>>>>>>> c18c7c42d4c821fa7b84c67b0387663369610a35

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE		50.0f

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{

};

//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************
const int CPlayer::FRAME_MAX = 5;		// �L�[���͂̎��s�܂ł̑ҋ@���� : 0.6�b
const UINT CPlayer::MOVE_FRAME = 20;

//=============================================================================
// CPlane����
//=============================================================================
CPlayer *CPlayer::Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;
	pPlayer->Init(nType, column, row, width, height, pos, rot, color);

	return pPlayer;
}

//=============================================================================
// CPlane�R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objtype) :
	CPlane(nPriority, objtype),
	m_rotDest(vector3NS::ZERO),
	m_prePos(vector3NS::ZERO),
	m_moveFrameCnt(0),
	m_currentMapLocation(0, 0),
	m_moveBuff(0, 0),
	m_front(0, 0)
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
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
<<<<<<< HEAD
	HRESULT hr{};
	m_pMenu = CMenu::Create(1, 2, 4, 400.0f, 200.0f, D3DXVECTOR3(650.0f, 0, 0), D3DXVECTOR3(0, 0, 0), color);
=======
	m_currentMapLocation = CMap::GetRespawnPoint();
	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_PLAYER);

	CPlane::Init(nType, column, row, width, height, pos, rot, color);
>>>>>>> c18c7c42d4c821fa7b84c67b0387663369610a35

	m_pos = CMap::MapPositionLink(m_currentMapLocation);

	m_Status.ATK = 1;

	m_ATK = m_Status.ATK;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// �I�u�W�F�N�g��j��
	CPlane::Uninit();

}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	Behavior();
	Move();
	Item();

	CDebugProc::Print("�^�[���� : %d\n", m_iTurn);
	CDebugProc::Print("RotAngle : %f\n", D3DXToDegree(m_rot.y));
	CDebugProc::Print("%f,%f,%f\n", m_pos.x, m_pos.y, m_pos.z);

}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	CPlane::Draw();
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer::Move(void)
{
	if (m_inputEnable)
	{
		InputMove(m_inputEnable);
	}
	else if (!m_bMove)
	{
		if (m_inputSecondEnable)
		{
			InputMove(m_inputSecondEnable);
		}
		m_frameCount++;
		if (m_frameCount > FRAME_MAX)
		{
			m_frameCount = 0;
			MoveMap(m_moveBuff);
		}
	}

	MovePosition(MOVE_FRAME);
}

//=============================================================================
// ���ړ�����
//=============================================================================
void CPlayer::MovePosition(UINT moveFrame)
{
	if (m_bMove)
	{
		m_moveFrameCnt++;
		D3DXVec3Lerp(&m_pos, &m_prePos, &(m_prePos + m_move), float(m_moveFrameCnt) / float(moveFrame));
		if (m_moveFrameCnt > moveFrame)
		{
			m_pos = m_prePos + m_move;
			m_moveFrameCnt = 0;
			m_move = vector3NS::ZERO;
			m_bMove = false;
			m_inputEnable = true;
			m_inputSecondEnable = true;
		}
	}
}

//=============================================================================
// �}�b�v�ړ�����
//=============================================================================
void CPlayer::MoveMap(INT8_2 moveBuff)
{
	m_bMove = true;
	m_prePos = m_pos;
	m_moveBuff = INT8_2(0, 0);

	float angleBuff = (D3DX_PI / 2.0f - atan2f(float(moveBuff.z), float(moveBuff.x)));
	float rotAngle = (signbit(angleBuff) * 2.0f - 1.0f) * D3DX_PI + angleBuff;
	m_rot.y = rotAngle;
	m_front = INT8_2(moveBuff.x, -moveBuff.z);

	if (CMap::GetMapStateFromLocation(m_currentMapLocation.mapX + moveBuff.x, m_currentMapLocation.mapZ - moveBuff.z) == CMap::MAP_STATE_WALL ||
		CMap::GetMapStateFromLocation(m_currentMapLocation.mapX + moveBuff.x, m_currentMapLocation.mapZ - moveBuff.z) == CMap::MAP_STATE_ENEMY ||
		m_bTurningPlayer)
	{
		return;
	}

	m_iTurn++;
	m_move += Vector3(moveBuff.x, 0.0f, moveBuff.z) * MOVE;
	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_FLOOR);
	m_currentMapLocation.mapX += moveBuff.x;
	m_currentMapLocation.mapZ -= moveBuff.z;
	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_PLAYER);
}

//=============================================================================
// �ړ��̓��͏���
//=============================================================================
void CPlayer::InputMove(bool& inputEnable)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

	// ������Ɉړ�
	if (pInputKeyboard->GetKeyPress(DIK_UP) && m_moveBuff.z <= 0)
	{
		m_moveBuff.z++;
		inputEnable = false;
		return;
	}

	// �������Ɉړ�
	if (pInputKeyboard->GetKeyPress(DIK_DOWN) && m_moveBuff.z >= 0)
	{
		m_moveBuff.z--;
		inputEnable = false;
		return;
	}
	// �������Ɉړ�
	if (pInputKeyboard->GetKeyPress(DIK_LEFT) && m_moveBuff.x >= 0)
	{
		m_moveBuff.x--;
		inputEnable = false;
		return;
	}
	// �E�����Ɉړ�
	if (pInputKeyboard->GetKeyPress(DIK_RIGHT) && m_moveBuff.x <= 0)
	{
		m_moveBuff.x++;
		inputEnable = false;
		return;
	}
	
}

//=============================================================================
// �s���̓��͏���
//=============================================================================
void CPlayer::Behavior()
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

	// �U��
	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))
	{
		// �U������
		Attack();
	}

	// P�L�[�Ń��j���[�E�B���h�E�\��
	else if (pInputKeyboard->GetKeyTrigger(DIK_P))
	{
		// �E�B���h�E�\��

	}

	// �����]���L�[�i���������Ă���ԁj
	if (pInputKeyboard->GetKeyPress(DIK_O))
	{
		m_bTurningPlayer = true;
	}
	else
	{
		m_bTurningPlayer = false;
	}
}

//=============================================================================
// �U������
//=============================================================================
void CPlayer::Attack()
{
	CEnemy *pEnemy = CManager::GetEnemy();
	// �ڂ̑O�ɓG�����邩����
	if (CMap::GetMapStateFromLocation(m_currentMapLocation.mapX + m_front.x, m_currentMapLocation.mapZ + m_front.z) == CMap::MAP_STATE_ENEMY)
	{
		// �U���͕��̃_���[�W�^����
		pEnemy->AddLife(-m_ATK);
		m_iTurn++;
	}

	// �G�����Ȃ��������U��
	else
		m_iTurn++;
}

//=============================================================================
// �A�C�e���擾����
//=============================================================================
void CPlayer::Item()
{
	// �����ɃA�C�e�������邩����
	if (CMap::GetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ) == CMap::MAP_STATE_ITEM)
	{
		/*�A�C�e�����擾���܂��B
		�@�C���x���g���ɒǉ����ꂽ�肳��Ȃ�������E�E�E
		 �@���͂Ƃ肠�����}�b�v��������Ƃ��܂�*/
		CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_FLOOR);
	}
}