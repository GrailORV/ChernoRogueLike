//=============================================================================
//
// 2D�I�u�W�F�N�g�̏��� [scene2D.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "WinApp.h"
#include "textureManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************
ComPtr<IDirect3DVertexBuffer9> CScene2D::m_pVtxBuff;

//=============================================================================
// 2D���_�o�b�t�@����
//=============================================================================
HRESULT CScene2D::MakeVertexBuffer(void)
{
	HRESULT hr{};

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	hr = pDevice->CreateVertexBuffer(sizeof(Vertex2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVFVertex2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		m_pVtxBuff.GetAddressOf(),				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);
	if (FAILED(hr))						// NULL�ɐݒ�
	{
		return hr;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		Vertex2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		for (UINT i = 0; i < NUM_VERTEX; i++)
		{
			pVtx->position.x = static_cast<float>(i % 2);
			pVtx->position.y = static_cast<float>(i / 2);
			pVtx->position.z = 0.0f;

			pVtx->color = colorNS::_WHITE;

			pVtx->uv.x = static_cast<float>(i % 2);
			pVtx->uv.y = static_cast<float>(i / 2);

			pVtx++;
		}

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// CScene2D����
//=============================================================================
CScene2D *CScene2D::Create(int nType, Vector3 pos, Vector3 rot, float width, float height, XColor color)
{
	CScene2D *pScene2D;

	pScene2D = new CScene2D;
	pScene2D->Init(nType, pos, rot, width, height, color);

	return pScene2D;
}

//=============================================================================
// CScene2D�R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPri, OBJTYPE objType) : CScene(nPri, objType),
m_pos{},
m_pivot{},
m_rot{},
m_scale{},
m_fWidth{},
m_fHeight{},
m_uv{},
m_color{},
m_nType{},
m_texMod{}
{
}

//=============================================================================
// CScene2D�f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(int nType, Vector3 pos, Vector3 rot, float width, float height, XColor color)
{
	// �^�C�v��ݒ�
	m_nType = nType;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	m_pos = pos;

	// �|���S���̉�]�p��ݒ�
	m_rot = rot;

	// �|���S���̃X�P�[���l�̕ύX
	m_scale = vector3NS::ONE;

	// �|���S���̃s�{�b�g�̐ݒ�
	m_pivot = vector3NS::ZERO;

	// �|���S���̃T�C�Y�̐ݒ�
	m_fWidth = width;
	m_fHeight = height;

	// �e�N�X�`�����W�̐ݒ�
	m_uv = Vector4(1.0f, 1.0f, 0.0f, 0.0f);

	// �F�̐ݒ�
	m_color = color;

	// �F�̏�Z�Ώۂ̐ݒ�
	m_texMod = D3DTA_CURRENT;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene2D::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene2D::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff.Get(), 0, sizeof(Vertex2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVFVertex2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture.Get());

	// ���[���h�ϊ��s��̐ݒ�
	Matrix world, scale, rot, translation, projection, view;
	D3DXMatrixScaling(&world, m_fWidth, m_fHeight, 1.0f);
	D3DXMatrixTranslation(&translation, -m_pivot.x, -m_pivot.y, -m_pivot.z);
	D3DXMatrixRotationZ(&rot, m_rot.z);
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	world *= translation;
	world = world * scale * rot;
	D3DXMatrixTranslation(&translation, m_pos.x, m_pos.y, m_pos.z);
	world *= translation;
	pDevice->SetTransform(D3DTS_WORLD, &world);

	projection = Matrix(
		2.0f / pManager->GetWindowWidth(), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / pManager->GetWindowHeight(), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	);
	pDevice->SetTransform(D3DTS_PROJECTION, &projection);

	D3DXMatrixIdentity(&view);
	pDevice->SetTransform(D3DTS_VIEW, &view);

	// �e�N�X�`�����W�Ԋҍs��̐ݒ�
	Matrix texture(
		m_uv.x, 0.0f, 0.0f, 0.0f,
		0.0f, m_uv.y, 0.0f, 0.0f,
		m_uv.z, m_uv.w, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	pDevice->SetTransform(D3DTS_TEXTURE0, &texture);

	// ���C�e�B���O�I�t
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �F�ݒ�
	pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, m_color);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, m_texMod);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// ���C�e�B���O�I��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �F�ݒ茳�ɖ߂�
	pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, colorNS::_WHITE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

	Matrix uvDefault;
	D3DXMatrixIdentity(&uvDefault);
	pDevice->SetTransform(D3DTS_TEXTURE0, &uvDefault);
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CScene2D::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}

//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CScene2D::BindTexture(const char* texID)
{
	if (!texID)
	{
		m_pTexture.Reset();
		m_texMod = D3DTA_CURRENT;
		return;
	}

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CTextureManager* pTextureManager = pManager->GetTextureManager();
	
	pTextureManager->BindtextureFromString(texID, m_pTexture.GetAddressOf());
	m_texMod = D3DTA_TEXTURE;

}
