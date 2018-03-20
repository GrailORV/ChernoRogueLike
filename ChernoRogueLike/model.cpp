//=============================================================================
//
// ���f���I�u�W�F�N�g�̏��� [model.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "WinApp.h"


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
// CModel����
//=============================================================================
CModel *CModel::Create(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale)
{
	CModel *pScene3D;

	pScene3D = new CModel;
	pScene3D->Init(nType, modelID, pos, rot, scale);

	return pScene3D;
}

//=============================================================================
// CModel�R���X�g���N�^
//=============================================================================
CModel::CModel(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = Vector3(0.0f, 0.0f, 0.0f);
	m_rot = Vector3(0.0f, 0.0f, 0.0f);

	m_nType = 0;
}

//=============================================================================
// CModel�f�X�g���N�^
//=============================================================================
CModel::~CModel()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale)
{
	HRESULT hr{};

	// �^�C�v��ݒ�
	m_nType = nType;

	// �ʒu��ݒ�
	m_pos = pos;

	// ������ݒ�
	m_rot = rot;

	// �X�P�[����ݒ�
	m_scale = scale;

	// �s�{�b�g�ʒu�̐ݒ�
	m_pivot = vector3NS::ZERO;

	// ���f���擾
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	pManager->GetModelManager()->BindModelFromString(modelID, &m_model);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{
	// �I�u�W�F�N�g��j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();
	CCamera *pCamera = pManager->GetCamera();

	// �J�����̐ݒ�
	pCamera->SetCamera();

	Matrix mtxScale, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �s�{�b�g�ʒu����
	D3DXMatrixTranslation(&mtxTranslate, -m_pivot.x, -m_pivot.y, -m_pivot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���݂̃}�e���A���擾
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);
	
	for (UINT i = 0; i < m_model->numMesh; i++)
	{
		// �I�t�Z�b�g���f
		Matrix world;
		D3DXMatrixMultiply(&world, &m_model->mesh[i]->offsetTransform, &m_mtxWorld);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &world);

		for (UINT j = 0; j < m_model->mesh[i]->numMat; j++)
		{
			pDevice->SetMaterial(&m_model->mesh[i]->material[j]);
			pDevice->SetTexture(0, m_model->texList[m_model->mesh[i]->texID[j]].Get());
			m_model->mesh[i]->mesh->DrawSubset(0);
		}
	}

	// �}�e���A�����ɖ߂�
	pDevice->SetMaterial(&matDef);
}

