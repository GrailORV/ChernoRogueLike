//=============================================================================
//
// �C���x���g���̏��� [inventory.cpp]
// Author :UENO
//
//=============================================================================
#include "stdafx.h"

#include "inventory.h"
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "textureManager.h"
#include "WinApp.h"
#include "input.h"
#include "debugproc.h"
#include "item.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_INVENTORY	(10)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************
const char *CInventory::m_apTextureName[] =
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
CInventory *CInventory::Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CInventory *pInventory;

	pInventory = new CInventory;
	pInventory->Init(nType, column, row, width, height, pos, rot, color);

	return pInventory;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInventory::CInventory(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pInventoryWindow = NULL;
	m_pItem = NULL;

	m_pos = vector3NS::ZERO;
	m_rot = vector3NS::ZERO;

	m_nType = 0;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		m_inventory[i].m_nHaves = 0;
		m_inventory[i].m_nType = 0;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInventory::~CInventory()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInventory::Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	m_nColumnCount = 0;
	m_nRowCount = 0;
	m_nInventoryIndex = 0;
	m_fWidth = width / (float)(MAX_INVENTORY);
	m_fHeight = height / (float)(MAX_INVENTORY);
	m_column = column;
	m_row = MAX_INVENTORY / m_column + MAX_INVENTORY % m_column;
	m_inventoryState = STATE_SELECT;
	
	//�C���x���g���̓ǂݍ���
	LoadInventory();
	WriteInventory();
	{// �C���x���g���E�B���h�E�̐���
		m_pInventoryWindow = CScene2D::Create(nType, pos, rot, width, height, color);
		m_pInventoryWindow->BindTexture("ore");
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInventory::Uninit(void)
{
	{// �C���x���g���E�B���h�E�̏I��
		if (m_pInventoryWindow != NULL)
		{
			m_pInventoryWindow->Uninit();
			m_pInventoryWindow = NULL;
		}
	}

	if (m_pItem != NULL)
	{
		delete m_pItem;
		m_pItem = NULL;
	}

	// �I�u�W�F�N�g��j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CInventory::Update(void)
{
	CManager* pManager;
	CInputKeyboard *pInputKeyboard;

	//�}�l�[�W���[�擾
	pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	//�L�[�{�[�h�擾
	pInputKeyboard = pManager->GetInputKeyboard();

	//�C���x���g���̑���
	KeyControl();

	//�C���f�b�N�X�ԍ��i�f�o�b�O�j
	CDebugProc::Print("%d�Ԗ�_ID�F%d_�������F%d\n", m_nInventoryIndex,m_inventory[m_nInventoryIndex].m_nType, m_inventory[m_nInventoryIndex].m_nHaves);

	// �C���x���g���̏�Ԃ𒲂ׂ�


	//�J�[�\���̈ʒu�X�V

	if (m_pItem)
	{
		m_pItem->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CInventory::Draw()
{
}

//=============================================================================
// �C���x���g���̏�Ԃ𒲂ׂ�
//=============================================================================
void CInventory::CheckState()
{
	switch (m_inventoryState)
	{
	case STATE_START:
		break;
	case STATE_SELECT:
		break;
	case STATE_USING:
		break;
	case STATE_MAX:
		break;
	}
}

//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CInventory::BindTexture(const char* texID)
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
// �C���x���g����ʂ̃L�[����
//=============================================================================
void CInventory::KeyControl(void)
{
	CManager* pManager;
	CInputKeyboard *pInputKeyboard;
	//�}�l�[�W���[�擾
	pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	//�L�[�{�[�h�擾
	pInputKeyboard = pManager->GetInputKeyboard();


	//�C���x���g���̑���
	if (pInputKeyboard->GetKeyTrigger(DIK_O))
	{//�J�[�\���ړ��i��j
		m_nInventoryIndex--;
	}
	if (pInputKeyboard->GetKeyTrigger(DIK_P))
	{//�J�[�\���ړ��i���j
		m_nInventoryIndex++;
	}

	//�C���x���g���̉����Ə���̐ݒ�
	if (m_nInventoryIndex < 0  || m_inventory[0].m_nHaves == 0)
	{
		m_nInventoryIndex = 0;
	}
	else if (m_inventory[m_nInventoryIndex].m_nHaves == 0)
	{
		m_nInventoryIndex--;
	}
	else if (m_nInventoryIndex >= MAX_INVENTORY)
	{
		m_nInventoryIndex = MAX_INVENTORY - 1;
	}

	

}

//=============================================================================
// �C���x���g���̓ǂݍ���
//=============================================================================
void CInventory::LoadInventory(void)
{
	int InventoryIndex = 0;

	//�t�@�C���\����
	FILE *fp;

	//�t�@�C����
	char * fname = "data/CSV/Inventory.csv";

	//�t�@�C���I�[�v��
	fp = fopen(fname, "r");

	//�t�@�C�����ǂݍ��߂Ȃ��ꍇ
	if (!fp)
	{
		MessageBox(NULL, "�A�C�e���f�[�^��ǂݍ��߂܂���ł����B\n", NULL, MB_OK);
	}

	for (int nCntInventory_Y = 0; nCntInventory_Y < MAX_ITEM; nCntInventory_Y++)
	{
		//��ނ̓ǂݍ���
		fscanf_s(fp, "%d,", &m_inventory[InventoryIndex].m_nType);

		//�A�C�e���̏�����Ԃ�ǂݍ���
		fscanf_s(fp, "%d,", &m_inventory[InventoryIndex].m_nHaves);

		//�����A�C�e���̏�������0�̏ꍇ�i�����ĂȂ��Ɠ��`�j
		if (m_inventory[InventoryIndex].m_nHaves == 0)
		{
			//�����l�߂�
			InventoryIndex--;
		}

		InventoryIndex++;
	}
	
	//�t�@�C���N���[�Y
	fclose(fp);

}

//=============================================================================
// �C���x���g���̏�������
//=============================================================================
void CInventory::WriteInventory(void)
{
	int InventoryIndex = 0;

	//�t�@�C���\����
	FILE *fp;

	//�t�@�C����
	char *fname = "data/CSV/Inventory.csv";
	char s = 'a';
	//�t�@�C���I�[�v��
	fp = fopen(fname, "w");

	//�t�@�C�����ǂݍ��߂Ȃ��ꍇ
	if (!fp)
	{
		MessageBox(NULL, "�A�C�e���f�[�^��ǂݍ��߂܂���ł����B\n", NULL, MB_OK);
	}

	//�w�b�_�[�̏�������
	fprintf(fp, "���,������\n");
	
	for (int nCntInventory_Y = 0; nCntInventory_Y < MAX_ITEM; nCntInventory_Y++)
	{
		//��ނ̏�������
		fprintf(fp, "%d,",m_inventory[InventoryIndex].m_nType);
		
		//�A�C�e���̏�����Ԃ���������
		fprintf(fp, "%d\n",m_inventory[InventoryIndex].m_nHaves);
	}

	//�t�@�C���N���[�Y
	fclose(fp);
}