//=============================================================================
//
// �`��ΏۃI�u�W�F�N�g�̏��� [scene.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene.h"

using namespace std;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************
// ���X�g�p
vector<CScene*> CScene::m_apScene[NUM_PRIORITY];

//=============================================================================
// ���X�g���I�u�W�F�X�V
//=============================================================================
void CScene::UpdateAll(void)
{
	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			m_apScene[nCntPri][nCntScene]->m_nID = nCntScene;
			if (!m_apScene[nCntPri][nCntScene]->m_bEnableUpdate)
			{
				continue;
			}
			m_apScene[nCntPri][nCntScene]->Update();
		}
	}
}

//=============================================================================
// ���X�g���I�u�W�F�`��
//=============================================================================
void CScene::DrawAll(void)
{
	//CCamera *pCamera = CManager::GetCamera();

	//// �J�����̐ݒ�
	//pCamera->SetCamera();

	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			if (!m_apScene[nCntPri][nCntScene]->m_bEnableDraw)
			{
				continue;
			}
			m_apScene[nCntPri][nCntScene]->Draw();
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g��j��
//=============================================================================
void CScene::Release(void)
{
	m_apScene[m_nPriority].erase(m_apScene[m_nPriority].begin() + m_nID);
}

//=============================================================================
// �I�u�W�F�N�g�S�̂�j��
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			m_apScene[nCntPri][nCntScene]->Uninit();
		}
	}
}

//=============================================================================
// CScene�R���X�g���N�^
//=============================================================================
CScene::CScene(UINT nPriority, OBJTYPE objType) :
	m_nPriority(nPriority),
	m_objType(objType),
	m_bEnableUpdate(true),
	m_bEnableDraw(true)
{
	m_apScene[nPriority].push_back(this);
}

//=============================================================================
// CScene�f�X�g���N�^
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// �S�ẴI�u�W�F�N�g�̍X�V������ON/OFF
//=============================================================================
void CScene::SetEnableUpdateAll(bool bEnableUpdate)
{
	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			// �I�u�W�F�N�g�̏I������
			m_apScene[nCntPri][nCntScene]->m_bEnableUpdate = bEnableUpdate;
		}
	}
}

//=============================================================================
// �S�ẴI�u�W�F�N�g�̕`�揈����ON/OFF
//=============================================================================
void CScene::SetEnableDrawAll(bool bEnableDraw)
{
	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			// �I�u�W�F�N�g�̏I������
			m_apScene[nCntPri][nCntScene]->m_bEnableDraw = bEnableDraw;
		}
	}
}



