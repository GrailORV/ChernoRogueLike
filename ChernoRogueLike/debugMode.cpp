//=============================================================================
//
// �f�o�b�O���[�h�̏��� [debugMode.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include "debugMode.h"
#include "manager.h"
#include "model.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "winApp.h"
#include "box.h"
#include "boxManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************


//=============================================================================
// CDebugMode�R���X�g���N�^
//=============================================================================
CDebugMode::CDebugMode() :CMode("DebugMode"),
m_pModel(nullptr)
{
}

//=============================================================================
// CDebugMode�f�X�g���N�^
//=============================================================================
CDebugMode::~CDebugMode()
{
}

//=============================================================================
// ������
//=============================================================================
HRESULT CDebugMode::Init(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CSound* pSound = pManager->GetSound();
	CCamera* pCamera = pManager->GetCamera();

	pCamera->SetPosition(Vector3(350.0f, 1700.0f, -1300.0f), Vector3(350.0f, 0.0f, 2000.0f));

	CBoxManager* boxManager = CBoxManager::Create(0);
	boxManager->BindTexture("net_of_box");

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CDebugMode::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	OnLostDevice();

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CDebugMode::Update(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

	if (pInputKeyboard->GetKeyPress(DIK_SPACE))
	{
		uint32_t boxNum = CBoxManager::GetBoxNum();
		if (boxNum < 10000)
		{

			CBox::Create(
				0,
				Vector3(80.0f, 80.0f, 80.0f),
				Vector3(80.0f*float(boxNum % 10), 80.0f*float(boxNum / 10 / 10), 80.0f*float(boxNum / 10 % 10)),
				vector3NS::ZERO,
				XColor((boxNum + 2) % 3 / 2 * 0.5f + 0.5f, (boxNum + 1) % 3 / 2 * 0.5f + 0.5f, boxNum % 3 / 2 * 0.5f + 0.5f, 1.0f));
		}
	}

	if (pInputKeyboard->GetKeyPress(DIK_DELETE))
	{
		if (CBoxManager::GetBoxList().size())
		{
			CBoxManager::GetBoxList()[CBoxManager::GetBoxList().size() - 1]->Uninit();
		}
	}

	CDebugProc::Print("�ł΂������[�ǂ���\n\n");
	CDebugProc::Print("�{�b�N�X���F%d\n", CBoxManager::GetBoxNum());
	CDebugProc::Print("���؁[���łӂ����\n");
	CDebugProc::Print("�ł�[�Ƃł������\n");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDebugMode::Draw(void)
{
}

//=============================================================================
// �I�u�W�F�N�g�j��
//=============================================================================
void CDebugMode::OnLostDevice(void)
{
	SafeUninit(m_pModel);
}

