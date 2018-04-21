//=============================================================================
//
// �X�t�B�A�̏��� [sphere.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include "sphere.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
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
// CSphere����
//=============================================================================
CSphere *CSphere::Create(int nType, UINT tessellation, Vector3 size, Vector3 pos, Vector3 rot, XColor color)
{
	CSphere *pScene3D;

	pScene3D = new CSphere;
	pScene3D->Init(nType, tessellation, size, pos, rot, color);

	return pScene3D;
}

//=============================================================================
// CSphere�R���X�g���N�^
//=============================================================================
CSphere::CSphere(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = vector3NS::ZERO;
	m_rot = vector3NS::ZERO;
	m_pivot = vector3NS::ZERO;
	m_size = vector3NS::ZERO;

	m_nType = 0;
}

//=============================================================================
// CSphere�f�X�g���N�^
//=============================================================================
CSphere::~CSphere()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSphere::Init(int nType, UINT tessellation, Vector3 size, Vector3 pos, Vector3 rot, XColor color)
{
	HRESULT hr{};

	// �^�C�v��ݒ�
	m_nType = nType;

	// �ʒu��ݒ�
	m_pos = pos;

	// ������ݒ�
	m_rot = rot;

	// �Ίp���̒����E�p�x��ݒ�
	m_size = size;

	// �s�{�b�g�ʒu�̐ݒ�
	m_pivot = vector3NS::ZERO;

	// �F�̐ݒ�
	m_color = color;

	// ���_�m��
	MakeVertexBuffer(tessellation);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSphere::Uninit(void)
{
	// �I�u�W�F�N�g��j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSphere::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSphere::Draw(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();
	CCamera *pCamera = pManager->GetCamera();

	// �J�����̐ݒ�
	pCamera->SetCamera();

	Matrix mtxScale, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �T�C�Y���f
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �s�{�b�g�ʒu����
	D3DXMatrixTranslation(&mtxTranslate, -m_pivot.x, -m_pivot.y, -m_pivot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff.Get(), 0, sizeof(Vertex3D));

	// �C���f�b�N�X�o�b�t�@��ݒ�
	pDevice->SetIndices(m_pIdxBuff.Get());

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVFVertexGeo);

	// �F�ݒ�
	D3DMATERIAL9 matCurr, matDef;
	pDevice->GetMaterial(&matDef);
	matCurr = matDef;
	matCurr.Diffuse = m_color;
	pDevice->SetMaterial(&matCurr);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture.Get());

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_numIdx, 0, m_numFace);

	// �}�e���A�����ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐���
//=============================================================================
HRESULT CSphere::MakeVertexBuffer(UINT tessellation)
{
	HRESULT hr{};

	m_numFace = 20 * UINT(powf(4.0f, float(tessellation)));
	m_numVtx = 10 * int(powf(float(tessellation + 1), 2.0f)) + 2;
	m_numIdx = m_numFace * 3;

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	hr = pDevice->CreateVertexBuffer(
		sizeof(Vertex3D)*m_numVtx,
		D3DUSAGE_WRITEONLY,
		FVFVertexGeo,
		D3DPOOL_MANAGED,
		m_pVtxBuff.GetAddressOf(),
		NULL
	);
	if (FAILED(hr))
	{
		return hr;
	}

	float a = 1.0f / sqrtf(5.0f);
	float b = (1.0f - a) / 2.0f;
	float c = (1.0f + a) / 2.0f;
	float d = sqrtf(b);
	float e = sqrtf(c);
	Vertex3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx->position = pVtx->normal = Vector3(0.0f, 1.0f, 0.0f);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(0.0f, a, 2.0f * a);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(e, a, b);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(d, a, -c);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(-d, a, -c);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(-e, a, b);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(d, -a, c);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(e, -a, -b);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(0.0f, -a, -2.0f * a);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(-e, -a, -b);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(-d, -a, c);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(0.0f, -1.0f, 0.0f);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	m_pVtxBuff->Unlock();

	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD)*m_numIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		m_pIdxBuff.GetAddressOf(),
		NULL
	);
	if (FAILED(hr))
	{
		return hr;
	}

	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	{// �㔼��
		for (UINT nCntFace = 0; nCntFace < m_numFace / 2; nCntFace++)
		{
			if (nCntFace / 5 < 1)
			{
				*pIdx = 0;
				pIdx++;
				*pIdx = nCntFace + 1;
				pIdx++;
				*pIdx = (nCntFace + 1) % 5 + 1;
				pIdx++;
			}
			else
			{
				*pIdx = nCntFace % 5 + 1;
				pIdx++;
				*pIdx = nCntFace % 5 + 6;
				pIdx++;
				*pIdx = (nCntFace + 1) % 5 + 1;
				pIdx++;
			}
		}
	}

	{// ������
		for (UINT nCntFace = 0; nCntFace < m_numFace / 2; nCntFace++)
		{
			if (nCntFace / 5 < 1)
			{
				*pIdx = 11;
				pIdx++;
				*pIdx = 11 - int(nCntFace + 1);
				pIdx++;
				*pIdx = 11 - int((nCntFace + 1) % 5 + 1);
				pIdx++;
			}
			else
			{
				*pIdx = 11 - int(nCntFace % 5 + 1);
				pIdx++;
				*pIdx = 11 - int(nCntFace % 5 + 6);
				pIdx++;
				*pIdx = 11 - int((nCntFace + 1) % 5 + 1);
				pIdx++;
			}
		}
	}

	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CSphere::BindTexture(const char* texID)
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

