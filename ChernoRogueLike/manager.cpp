//=============================================================================
//
// �}�l�[�W���[�̏��� [manager.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"
#include "resource.h"

#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "plane.h"
#include "model.h"
#include "debugMode.h"
#include "StartGameMode.h"

//=============================================================================
// CManager�R���X�g���N�^
//=============================================================================
CManager::CManager(UINT width, UINT height) :
	m_width(width),
	m_height(height),
	m_dwRef(0)
{
}

//=============================================================================
// CManager�f�X�g���N�^
//=============================================================================
CManager::~CManager()
{
	Release();
}

//=============================================================================
// �Q�ƃR�s�[
//=============================================================================
HRESULT CManager::QueryInterface(REFIID riid, void FAR* FAR* ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown))
	{
		*ppvObject = this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

//=============================================================================
// �Q�ƃJ�E���^�C���N�������g
//=============================================================================
ULONG CManager::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// �I������
//=============================================================================
ULONG CManager::Release(void)
{
	if (--m_dwRef == 0)
	{
		delete this;
		return 0;
	}
	return m_dwRef;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hwnd, BOOL bWindow)
{
	HRESULT hr{};

	// �����_�����O�����̏���������
	m_pRenderer = new CRenderer;
	hr = m_pRenderer->Init(bWindow);
	if (FAILED(hr))
	{
		return hr;
	}

	// �e�N�X�`���}�l�[�W���̏�����
	m_pTextureManager = new CTextureManager;
	hr = m_pTextureManager->Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// ���b�V���}�l�[�W���̏�����
	m_pModelManager = new CModelManager;
	hr = m_pModelManager->Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// �L�[�{�[�h�̏���������
	m_pInputKeyboard = new CInputKeyboard;
	hr = m_pInputKeyboard->Init(hInstance, hwnd);
	if (FAILED(hr))
	{
		return hr;
	}

	// �}�E�X�̏���������
	m_pInputMouse = new CInputMouse;
	hr = m_pInputMouse->Init(hInstance, hwnd);
	if (FAILED(hr))
	{
		return hr;
	}

	// �W���C�p�b�h�̏���������
	m_pInputJoypad = new CInputJoypad;
	hr = m_pInputJoypad->Init(hInstance, hwnd);
	if (FAILED(hr))
	{
		return hr;
	}

	// �T�E���h�̏�����
	m_pSound = new CSound;
	hr = m_pSound->Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// �J�����̏�����
	m_pCamera = new CCamera;
	hr = m_pCamera->Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// ���C�g�̏���������
	m_pLight = new CLight;
	hr = m_pLight->Init();
	if (FAILED(hr))
	{
		return hr;
	}

#ifdef _DEBUG
	// �f�o�b�O�t�H���g�̏�����
	m_pDebugProc = new CDebugProc;
	hr = m_pDebugProc->Init();
	if (FAILED(hr))
	{
		return hr;
	}
#endif

	m_pTextureManager->LoadSceneTex(0);
	m_pModelManager->LoadSceneMesh(0);

	// �Q�[�����[�h�}�l�[�W���̏�����
	m_pGameModeManager = new CGameModeManager;
	hr = m_pGameModeManager->Init<StartGameMode>();
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	CDebugProc::Print("F1�������Ə������i�́[��\n");

	// �L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}

	// �}�E�X�̍X�V����
	if (m_pInputMouse)
	{
		m_pInputMouse->Update();
	}

	// �W���C�p�b�h�̍X�V����
	if (m_pInputJoypad)
	{
		m_pInputJoypad->Update();
	}

	// �T�E���h�̍X�V����
	if (m_pSound)
	{
		m_pSound->Update();
	}

	// �J�����̍X�V����
	if (m_pCamera)
	{
		m_pCamera->Update();
	}

	// ���C�g�̍X�V����
	if (m_pLight)
	{
		m_pLight->Update();
	}

	// �I�u�W�F�N�g�̍X�V����
	if (m_pRenderer)
	{
		m_pRenderer->Update();
	}

#ifdef _DEBUG
	// �f�o�b�O�t�H���g�̍X�V
	m_pDebugProc->Update();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	// �I�u�W�F�N�g�̍X�V����
	if (m_pRenderer)
	{
		m_pRenderer->Draw();
	}
}