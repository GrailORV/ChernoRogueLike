//=============================================================================
//
// ���C�g�̏��� [light.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

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

	{// �L�[���C�g
		// D3DLIGHT9�\���̂�0�ŃN���A����
		ZeroMemory(&m_aLight, sizeof(D3DLIGHT9));

		// ���C�g�̃^�C�v�̐ݒ�
		m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

		// �g�U��
		m_aLight[0].Diffuse.r = 1.0000000f;
		m_aLight[0].Diffuse.g = 0.9607844f;
		m_aLight[0].Diffuse.b = 0.8078432f;

		// ���ˌ�
		m_aLight[0].Specular.r = 1.0000000f;
		m_aLight[0].Specular.g = 0.9607844f;
		m_aLight[0].Specular.b = 0.8078432f;

		// ���C�g�̕����̐ݒ�
		m_aLight[0].Direction = Vector3(-0.5265408f, -0.5735765f, -0.6275069f);

		// ���C�g�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetLight(0, &m_aLight[0]);

		// ���C�g�̐ݒ�
		pDevice->LightEnable(0, TRUE);
	}

	{// �t�B�����C�g
		// D3DLIGHT9�\���̂�0�ŃN���A����
		ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));

		// ���C�g�̃^�C�v�̐ݒ�
		m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

		// �g�U��
		m_aLight[1].Diffuse.r = 0.3231373f;
		m_aLight[1].Diffuse.g = 0.3607844f;
		m_aLight[1].Diffuse.b = 0.3937255f;

		// ���ˌ�
		m_aLight[1].Specular.r = 0.3231373f;
		m_aLight[1].Specular.g = 0.3607844f;
		m_aLight[1].Specular.b = 0.3937255f;

		// ���C�g�̕����̐ݒ�
		m_aLight[1].Direction = Vector3(0.4545195f, -0.7660444f, 0.4545195f);

		// ���C�g�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetLight(1, &m_aLight[1]);

		// ���C�g�̐ݒ�
		pDevice->LightEnable(1, TRUE);
	}

	{// �o�b�N���C�g
		// D3DLIGHT9�\���̂�0�ŃN���A����
		ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));

		// ���C�g�̃^�C�v�̐ݒ�
		m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

		// �g�U��
		m_aLight[2].Diffuse.r = 0.9647059f;
		m_aLight[2].Diffuse.g = 0.7607844f;
		m_aLight[2].Diffuse.b = 0.4078432f;

		// ���ˌ�
		m_aLight[2].Specular.r = 0.0f;
		m_aLight[2].Specular.g = 0.0f;
		m_aLight[2].Specular.b = 0.0f;

		// ���C�g�̕����̐ݒ�
		m_aLight[2].Direction = Vector3(0.7198464f, 0.3420201f, 0.6040227f);

		// ���C�g�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetLight(2, &m_aLight[2]);

		// ���C�g�̐ݒ�
		pDevice->LightEnable(2, TRUE);
	}

	{// ����
		XColor ambientColor;
		ambientColor.r = 0.05333332f;
		ambientColor.g = 0.09882354f;
		ambientColor.b = 0.01819608f;
		pDevice->SetRenderState(D3DRS_AMBIENT, ambientColor);
	}

	// ���C�e�B���O���[�h
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

