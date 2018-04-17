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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE		50.0f

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


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
	CPlayer *pPlane;

	pPlane = new CPlayer;
	pPlane->Init(nType, column, row, width, height, pos, rot, color);

	return pPlane;
}

//=============================================================================
// CPlane�R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objtype) :
	CPlane(nPriority, objtype),
	m_rotDest(vector3NS::ZERO),
	m_prePos(vector3NS::ZERO),
	m_moveFrameCnt(0)
{
	m_iCount = 0;
	m_iTurn = 0;
	m_frameCount = 0;
	m_posX = 0;
	m_posZ = 0;
	m_pos = vector3NS::ZERO;
	m_move = vector3NS::ZERO;
	m_moveBuff = vector3NS::ZERO;
	m_bMove = false;
	m_inputEnable = false;
	m_inputSecondEnable = false;
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
	CPlane::Init(nType, column, row, width, height, pos, rot, color);

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
			if (D3DXVec3Length(&m_moveBuff) <= 0.0f)
			{
				m_inputEnable = true;
				m_inputSecondEnable = true;
			}
			else
			{
				// �����ɉ�]���������

				m_bMove = true;
				m_iTurn++;
				m_move += m_moveBuff;
				m_prePos = m_pos;
				m_moveBuff = Vector3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	if (m_bMove)
	{
		m_moveFrameCnt++;
		D3DXVec3Lerp(&m_pos, &m_prePos, &(m_prePos + m_move), float(m_moveFrameCnt) / float(MOVE_FRAME));
		if (m_moveFrameCnt > MOVE_FRAME)
		{
			m_moveFrameCnt = 0;
			m_move = vector3NS::ZERO;
			m_bMove = false;
			m_inputEnable = true;
			m_inputSecondEnable = true;
		}
	}

	/*m_pos += m_move;
	m_move *= 0.8f;
	if (D3DXVec3Length(&m_move) < 0.01f)
	{
		m_move = Vector3(0.0f, 0.0f, 0.0f);
	}*/


	CDebugProc::Print("�^�[���� : %d\n", m_iTurn);
	CDebugProc::Print("�t���[���� : %d\n", m_iCount);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	CPlane::Draw();
}

//=============================================================================
// ���͏���
//=============================================================================
void CPlayer::InputMove(bool& inputEnable)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

	if (pInputKeyboard->GetKeyPress(DIK_UP) && m_moveBuff.z <= 0.0f)
	{
		m_moveBuff.z += MOVE;
		inputEnable = false;
		return;
	}
	if (pInputKeyboard->GetKeyPress(DIK_DOWN) && m_moveBuff.z >= 0.0f)
	{
		m_moveBuff.z += -MOVE;
		inputEnable = false;
		return;
	}
	if (pInputKeyboard->GetKeyPress(DIK_LEFT) && m_moveBuff.x >= 0.0f)
	{
		m_moveBuff.x += -MOVE;
		inputEnable = false;
		return;
	}
	if (pInputKeyboard->GetKeyPress(DIK_RIGHT) && m_moveBuff.x <= 0.0f)
	{
		m_moveBuff.x += MOVE;
		inputEnable = false;
		return;
	}
}
