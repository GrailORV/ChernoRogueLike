//=============================================================================
//
// 3D�I�u�W�F�N�g�̏��� [scene3D.cpp]
// Author : 
//
//=============================================================================
#include "stdafx.h"

#include "plane.h"
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
// CPlane����
//=============================================================================
CPlane *CPlane::Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CPlane *pScene3D;

	pScene3D = new CPlane;
	pScene3D->Init(nType, column, row, width, height, pos, rot, color);

	return pScene3D;
}

//=============================================================================
// CPlane�R���X�g���N�^
//=============================================================================
CPlane::CPlane(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nType = 0;
}

//=============================================================================
// CPlane�f�X�g���N�^
//=============================================================================
CPlane::~CPlane()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlane::Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	HRESULT hr{};

	// �^�C�v��ݒ�
	m_nType = nType;

	// �������̐ݒ�
	m_column = column;
	m_row = row;
	m_numFace = column * row * 2 + (column - 1) * 4;
	m_numIndex = m_numFace + 2;
	m_numVertex = (column + 1) * (row + 1);

	// �ʒu��ݒ�
	m_pos = pos;

	// ������ݒ�
	m_rot = rot;

	// �Ίp���̒����E�p�x��ݒ�
	m_width = width;
	m_height = height;

	// �s�{�b�g�ʒu�̐ݒ�
	m_pivot = vector3NS::ZERO;

	// �e�N�X�`�����W�̐ݒ�
	m_uv = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.0f);

	// �F�̐ݒ�
	m_color = color;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	hr = MakeVertexBuffer();
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlane::Uninit(void)
{
	// �I�u�W�F�N�g��j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlane::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlane::Draw(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();
	CCamera *pCamera = pManager->GetCamera();

	// �J�����̐ݒ�
	pCamera->SetCamera();

	D3DXMATRIX mtxScale, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

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
	pDevice->SetFVF(FVFVertex3D);

	// �e�N�X�`�����W�Ԋҍs��̐ݒ�
	D3DXMATRIX texture(
		m_uv.x, 0.0f, 0.0f, 0.0f,
		0.0f, m_uv.y, 0.0f, 0.0f,
		m_uv.z, m_uv.w, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	pDevice->SetTransform(D3DTS_TEXTURE0, &texture);

	// �F�ݒ�
	D3DMATERIAL9 matCurr, matDef;
	pDevice->GetMaterial(&matDef);
	matCurr = matDef;
	matCurr.Diffuse = m_color;
	pDevice->SetMaterial(&matCurr);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture.Get());

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_numIndex, 0, m_numFace);

	// �}�e���A�����ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐���
//=============================================================================
HRESULT CPlane::MakeVertexBuffer(void)
{
	HRESULT hr{};

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	hr = pDevice->CreateVertexBuffer(
		sizeof(Vertex3D)*m_numVertex, 
		D3DUSAGE_WRITEONLY, 
		FVFVertex3D, 
		D3DPOOL_MANAGED, 
		m_pVtxBuff.GetAddressOf(), 
		NULL
	);
	if (FAILED(hr))
	{
		return hr;
	}

	Vertex3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (UINT row = 0; row <= m_row; row++)
	{
		for (UINT column = 0; column <= m_column; column++)
		{
			pVtx->position.x = (column / (float)m_column - 0.5f) * m_width;
			pVtx->position.y = 0.0f;
			pVtx->position.z = (0.5f - row / (float)m_row) * m_height;

			pVtx->normal = vector3NS::UP;

			pVtx->color = 0xffffffff;

			pVtx->uv.x = column / (float)m_column;
			pVtx->uv.y = row / (float)m_row;

			pVtx++;
		}
	}
	m_pVtxBuff->Unlock();

	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD)*m_numIndex,
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
	int nDivide = (m_column + 2) * 2;
	int nVtxNum;
	for (UINT nCntVtx = 0; nCntVtx < m_numIndex; nCntVtx++)
	{
		int nVtxBuff = nCntVtx%nDivide;
		// �k�ޕ�
		if (nVtxBuff == nDivide - 2)
		{
			// �O�̒��_�ԍ����p
		}
		else if (nVtxBuff == nDivide - 1)
		{
			nVtxNum = ((nVtxBuff % 2) + (nCntVtx / nDivide))*(m_column + 1) + (nVtxBuff / 2);
		}
		// �ʏ�̒��_
		else
		{
			nVtxNum = (((nVtxBuff + 1) % 2) + (nCntVtx / nDivide))*(m_column + 1) + (nVtxBuff / 2);
		}
		*pIdx = (WORD)nVtxNum;
		pIdx++;
	}

	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �摜�̃��[�h
//=============================================================================
void CPlane::LoadTexture(const char *pTextureName)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	HRESULT hr{};

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		pTextureName,		// �t�@�C���̖��O
		m_pTexture.GetAddressOf());		// �ǂݍ��ރ������[
	if (FAILED(hr))
	{
		return;
	}

}

//=============================================================================
// �摜�̃o�C���h
//=============================================================================
void CPlane::BindTexture(IDirect3DTexture9* pTex)
{
	pTex->QueryInterface(IID_IUnknown, (void**)m_pTexture.GetAddressOf());
}

