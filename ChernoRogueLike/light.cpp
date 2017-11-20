//=============================================================================
//
// ���C�g�̏��� [light.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "WinApp.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************


//=============================================================================
// CLight�R���X�g���N�^
//=============================================================================
CLight::CLight() :m_dwRef(0)
{
}

//=============================================================================
// CLight�f�X�g���N�^
//=============================================================================
CLight::~CLight()
{
	Release();
}

//=============================================================================
// �Q�ƃR�s�[
//=============================================================================
HRESULT CLight::QueryInterface(REFIID riid, void FAR* FAR* ppvObject)
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
ULONG CLight::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// �I������
//=============================================================================
ULONG CLight::Release(void)
{
	if (--m_dwRef == 0)
	{
		delete this;
		return 0;
	}
	return m_dwRef;
}

//=============================================================================
// CLight����������
//=============================================================================
HRESULT CLight::Init(void)
{
	// ���C�g�̐ݒ�
	SetLight();

	return S_OK;
}

//=============================================================================
// CCamera�X�V����
//=============================================================================
void CLight::Update(void)
{
}

//=============================================================================
// ���C�g�̐ݒ�
//=============================================================================
void CLight::SetLight(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&m_aLight, sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	m_aLight[0].Diffuse.r = 1.0f;
	m_aLight[0].Diffuse.g = 1.0f;
	m_aLight[0].Diffuse.b = 1.0f;

	// ����
	m_aLight[0].Ambient.r = 0.50f;
	m_aLight[0].Ambient.g = 0.50f;
	m_aLight[0].Ambient.b = 0.50f;

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[0].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, &m_aLight[0]);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(0, TRUE);


	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	m_aLight[1].Diffuse.r = 0.25f;
	m_aLight[1].Diffuse.g = 0.25f;
	m_aLight[1].Diffuse.b = 0.25f;

	// ����
	m_aLight[1].Ambient.r = 0.20f;
	m_aLight[1].Ambient.g = 0.20f;
	m_aLight[1].Ambient.b = 0.20f;

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[1].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(1, &m_aLight[1]);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(1, TRUE);

	// ���C�e�B���O���[�h
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x000000);
}

