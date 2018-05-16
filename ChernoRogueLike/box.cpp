//=============================================================================
//
// �{�b�N�X�̏��� [box.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include "box.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "camera.h"
#include "WinApp.h"
#include "boxManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************

//=============================================================================
// CBox����
//=============================================================================
CBox *CBox::Create(int nType, Vector3 size, Vector3 pos, Vector3 rot, XColor color)
{
	CBox *pBox;

	pBox = new CBox;
	pBox->Init(nType, size, pos, rot, color);

	return pBox;
}

//=============================================================================
// CBox�R���X�g���N�^
//=============================================================================
CBox::CBox(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nType = 0;
}

//=============================================================================
// CBox�f�X�g���N�^
//=============================================================================
CBox::~CBox()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBox::Init(int nType, Vector3 size, Vector3 pos, Vector3 rot, XColor color)
{
	HRESULT hr{};

	if (!CBoxManager::GetInitialized())
	{
		Release();
		return E_FAIL;
	}

	// �^�C�v��ݒ�
	m_nType = nType;

	// �{�b�N�X�f�[�^������
	m_pos = pos;
	m_rot = rot;
	m_size = size;
	m_color = color;

	// �{�b�N�X�}�l�[�W���ɓo�^
	CBoxManager::CreateBox(this);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBox::Uninit(void)
{
	CBoxManager::DestroyBox(m_boxID);

	// �I�u�W�F�N�g��j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBox::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBox::Draw(void)
{
}

//=============================================================================
// �ʒu�ݒ�
//=============================================================================
void CBox::SetPosition(Vector3 pos)
{
	m_pos = pos;

	Matrix mtxWorld, mtxScale, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �T�C�Y���f
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���ύX
	CBoxManager::SetWorldByID(m_boxID, mtxWorld);
}

//=============================================================================
// ��]�ݒ�
//=============================================================================
void CBox::SetRotation(Vector3 rot)
{
	m_rot = rot;

	Matrix mtxWorld, mtxScale, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �T�C�Y���f
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���ύX
	CBoxManager::SetWorldByID(m_boxID, mtxWorld);

}

//=============================================================================
// �T�C�Y�ݒ�
//=============================================================================
void CBox::SetSize(Vector3 size)
{
	m_size = size;

	Matrix mtxWorld, mtxScale, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �T�C�Y���f
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���ύX
	CBoxManager::SetWorldByID(m_boxID, mtxWorld);

}

//=============================================================================
// �F�ݒ�
//=============================================================================
void CBox::SetColor(XColor color)
{
	m_color = color;

	CBoxManager::SetColorByID(m_boxID, m_color);
}
