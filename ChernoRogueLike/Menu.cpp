//=============================================================================
//
// ���j���[�̏��� [Menu.cpp]
// Author :UENO
//
//=============================================================================
#include "stdafx.h"

#include "Menu.h"
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "textureManager.h"
#include "WinApp.h"
#include "input.h"
#include "inventory.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE		10.f

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************
const char *CMenu::m_apTextureName[] =
{
	//���j���[�I������
	"ore",
	"ore",
	"ore",
	"ore",
	"ore",
	"ore",
	"ore"
};



//=============================================================================
// ����
//=============================================================================
CMenu *CMenu::Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CMenu *pMenu;

	pMenu = new CMenu;
	pMenu->Init(nType,  column, row, width, height, pos, rot, color);

	return pMenu;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMenu::CMenu(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority,objType)
{
	//���j���[�g
	m_pMenuWindow = NULL;
	//���j���[�I������
	for (int nCountSelect = 0; nCountSelect < SELECT_MAX; nCountSelect++)
	{
		m_apMenuSelect[nCountSelect] = NULL;
	}
	//���j���[�J�[�\��
	m_pMenuCursor = NULL;

	//�C���x���g��
	m_pInventory = NULL;

	//�X�e�[�^�X
	m_menuState = STATE_START;

	m_pos = vector3NS::ZERO;
	m_rot = vector3NS::ZERO;

	m_nType = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMenu::~CMenu()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMenu::Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{	
	//��Ԃ̏�����
	m_menuState = STATE_SELECT;

	//�J�[�\���̏����ʒu
	m_nMenuIndex = SELECT_ITEM;

	//�I�����ڂ̏c���Ɖ���
	m_fWidth = width / (float)(SELECT_MAX);
	m_fHeight = height / (float)(SELECT_MAX);
	
	//�s��̐ݒ�
	m_column = column;
	m_row = SELECT_MAX / m_column;
	if (SELECT_MAX % m_column != 0)
	{
		m_row = m_row + 1;
	}

	{// ���j���[�E�B���h�E�̐���
		m_pMenuWindow = CScene2D::Create(nType, pos, rot, width, height, color);
		m_pMenuWindow->BindTexture("ore");
	}

	{// �I�����ڂ̐���
		for (int nCountSelect = 0; nCountSelect < SELECT_MAX; nCountSelect++)
		{
			m_apMenuSelect[nCountSelect] = CScene2D::Create(nType,
				pos + D3DXVECTOR3((width / (float)m_column) * (nCountSelect % m_column) + (width / (m_column * 2.0f) - (m_fWidth / 2.0f)),
				(height / (float)m_row) * (nCountSelect / m_column) + (height / (m_row * 2)) - (m_fHeight / 2.0f),
					0),
				rot,
				m_fWidth,
				m_fHeight,
				color);
			
			//�e�N�X�`���̐ݒ�
			m_apMenuSelect[nCountSelect]->BindTexture(m_apTextureName[nCountSelect]);
		}
	}

	{//�I���A�C�R���̐���
		m_pMenuCursor = CScene2D::Create(1, m_apMenuSelect[m_nMenuIndex]->GetPosition(), D3DXVECTOR3(0,0,0), m_fWidth, m_fHeight, color);
		m_pMenuCursor->BindTexture("tex_haruka_princess");
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMenu::Uninit(void)
{
	{// ���j���[�E�B���h�E�̏I��
		if (m_pMenuWindow != NULL)
		{
			m_pMenuWindow->Uninit();
			m_pMenuWindow = NULL;
		}
	}

	{//�I�����ڂ̏I��
		for (int nCountSelect = 0; nCountSelect < SELECT_MAX; nCountSelect++)
		{
			if (m_apMenuSelect[nCountSelect] != NULL)
			{
				m_apMenuSelect[nCountSelect]->Uninit();
				m_apMenuSelect[nCountSelect] = NULL;
			}
		}
	}

	{//�I���A�C�R���̏I��
		if (m_pMenuCursor != NULL)
		{
			m_pMenuCursor->Uninit();
			m_pMenuCursor = NULL;
		}
	}
	// �I�u�W�F�N�g��j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMenu::Update(void)
{
	//�L�[����
	KeyControl();

	//�C���f�b�N�X�ԍ��i�f�o�b�O�j
	CDebugProc::Print("���j���[�C���f�b�N�X : %d\n", m_nMenuIndex);

	//�J�[�\���̈ʒu�X�V
	m_pMenuCursor->SetPosition(m_apMenuSelect[m_nMenuIndex]->GetPosition());

}

//=============================================================================
// �`�揈��
//=============================================================================
void CMenu::Draw()
{
}

//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CMenu::BindTexture(const char* texID)
{
	if (!texID)
	{
		m_pTexture.Reset();
		return;
	}

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	pTextureManager->BindtextureFromString(texID, m_pTexture.GetAddressOf());
}

//=============================================================================
// ���j���[��ʂ̃L�[����
//=============================================================================
void CMenu::KeyControl(void)
{
	CManager* pManager;
	CInputKeyboard *pInputKeyboard;
	//�}�l�[�W���[�擾
	pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	//�L�[�{�[�h�擾
	pInputKeyboard = pManager->GetInputKeyboard();


	//���j���[�̑���
	switch (m_menuState)
	{
	case STATE_SELECT:
		if (pInputKeyboard->GetKeyTrigger(DIK_O))
		{//�J�[�\���ړ��i���j
			m_nMenuIndex--;

			if ((m_nMenuIndex + 1) % m_column == 0)
			{
				m_nMenuIndex += m_column;
			}
			if (m_nMenuIndex >= SELECT_MAX)
			{
				m_nMenuIndex = SELECT_MAX - 1;
			}
		}
		if (pInputKeyboard->GetKeyTrigger(DIK_P))
		{//�J�[�\���ړ��i�E�j
			m_nMenuIndex++;

			if (m_nMenuIndex % m_column == 0)
			{
				m_nMenuIndex -= m_column;
			}
			if (m_nMenuIndex >= SELECT_MAX)
			{
				m_nMenuIndex = SELECT_MAX - 1;
			}
		}
		if (pInputKeyboard->GetKeyTrigger(DIK_I))
		{//�J�[�\���ړ��i���j
			m_nMenuIndex += m_column;

			if (m_nMenuIndex >= SELECT_MAX)
			{
				m_nMenuIndex = m_nMenuIndex % m_column;
			}
		}
		if (pInputKeyboard->GetKeyTrigger(DIK_U))
		{//�J�[�\���ړ��i��j
			int nSelectMax = m_column * m_row;

			m_nMenuIndex -= m_column;

			if (m_nMenuIndex < 0)
			{
				m_nMenuIndex = nSelectMax + m_nMenuIndex;

				if (m_nMenuIndex >= SELECT_MAX)
				{
					m_nMenuIndex -= m_column;
				}
			}
		}
		if (pInputKeyboard->GetKeyTrigger(DIK_L))
		{//����{�^��
			if (!m_pInventory)
			{
				switch (m_nMenuIndex)
				{
				case SELECT_ITEM:
					//�C���x���g����\��
					m_pInventory = CInventory::Create(1, 1, 1, 300.f, 500.0f, D3DXVECTOR3(100.0f, 0, 0), vector3NS::ZERO, colorNS::_WHITE);
					
					//��Ԃ̕ύX(�g�p����)
					m_menuState = STATE_USING;
					break;
				}
			}
		}
		break;
	case STATE_USING:
		break;
	default:
		break;
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_K))
	{//�L�����Z���{�^���i�f�o�b�O�j
		if (m_pInventory)
		{//�C���x���g�����폜
			m_pInventory->Uninit();
			m_pInventory = NULL;

			//��Ԃ̕ύX�i�I�𒆂�)
			m_menuState = STATE_SELECT;
		}
	}
}