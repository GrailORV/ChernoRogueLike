//=============================================================================
//
// �}�l�[�W���[�̏��� [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "light.h"

//=============================================================================
// CManager�R���X�g���N�^
//=============================================================================
CManager::CManager(UINT width, UINT height) :
	m_width(width),
	m_height(height),
	m_pRenderer{ nullptr },
	m_pInputKeyboard{ nullptr },
	m_pInputMouse{ nullptr },
	m_pInputJoypad{ nullptr },
	m_pLight{ nullptr }
{
}

//=============================================================================
// CManager�f�X�g���N�^
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hwnd, BOOL bWindow)
{
	HRESULT hr{};

	// �����_�����O�����̏���������
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hwnd, bWindow);

	// �L�[�{�[�h�̏���������
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hwnd);

	// �}�E�X�̏���������
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hwnd);

	// �W���C�p�b�h�̏���������
	m_pInputJoypad = new CInputJoypad;
	m_pInputJoypad->Init(hInstance, hwnd);

	// ���C�g�̏���������
	m_pLight = new CLight;
	m_pLight->Init();

#ifdef _DEBUG
	// �f�o�b�O�t�H���g�̏�����
	m_pDebugProc = new CDebugProc;
	m_pDebugProc->Init();
#endif

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �����_�����O�̏I������
	SafeDelete(m_pRenderer, &CRenderer::Uninit);

	// �L�[�{�[�h�̏I������
	SafeDelete(m_pInputKeyboard, &CInputKeyboard::Uninit);

	// �}�E�X�̏I������
	SafeDelete(m_pInputMouse, &CInputMouse::Uninit);

	// �W���C�p�b�h�̏I������
	SafeDelete(m_pInputJoypad, &CInputJoypad::Uninit);

	// ���C�g�̏I������
	SafeDelete(m_pLight, &CLight::Uninit);

#ifdef _DEBUG
	// �f�o�b�O�t�H���g�̔j��
	SafeDelete(m_pDebugProc, &CDebugProc::Uninit);
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
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

	CDebugProc::Print("F1�������Ə������i�́[��\n");
	

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