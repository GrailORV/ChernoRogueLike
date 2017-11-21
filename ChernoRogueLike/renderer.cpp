//=============================================================================
//
// �����_�����O���� [renderer.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "WinApp.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "camera.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//=============================================================================
// CRenderer�R���X�g���N�^
//=============================================================================
CRenderer::CRenderer() :m_dwRef(0)
{
}

//=============================================================================
// CRenderer�f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{
	Release();
}

//=============================================================================
// �Q�ƃR�s�[
//=============================================================================
HRESULT CRenderer::QueryInterface(REFIID riid, void FAR* FAR* ppvObject)
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
ULONG CRenderer::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// �I������
//=============================================================================
ULONG CRenderer::Release(void)
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
HRESULT CRenderer::Init(HWND hwnd, BOOL bWindow)
{
	// �}�l�[�W���[�擾
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;										// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = pManager->GetWindowWidth();				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = pManager->GetWindowHeight();			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;										// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;							// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;						// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			// �C���^�[�o��
	d3dpp.hDeviceWindow = hwnd;

	if (bWindow)
	{
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
	}
	else
	{
		d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;			// ���t���b�V�����[�g
	}


	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hwnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hwnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��(�����l��D3DBLEND_ONE)
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��(�����l��D3DBLEND_ZERO)

																			// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

																			// �e�N�X�`���X�e�[�W�X�e�[�g�p�����[�^�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		// �J���[�u�����f�B���O����(�����l��D3DTOP_MODULATE)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �ŏ��̃J���[����(�����l��D3DTA_TEXTURE)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);		// �Q�Ԗڂ̃J���[����(�����l��D3DTA_CURRENT)

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);	// �A���t�@�u�����f�B���O����(�����l��D3DTOP_SELECTARG1)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// �ŏ��̃A���t�@����(�����l��D3DTA_TEXTURE�A�e�N�X�`�����Ȃ��ꍇ��D3DTA_DIFFUSE)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// �Q�Ԗڂ̃A���t�@����(�����l��D3DTA_CURRENT)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);	// 2�����x�N�g���Ŏw�肳�ꂽ�e�N�X�`�����W�̕ϊ��s��L����

	CScene2D::MakeVertexBuffer();

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	// �I�u�W�F�N�g�̍X�V
	CScene::UpdateAll();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), colorNS::_BACKGROUND, 1.0f, 0)))
	{
		MessageBox(NULL, "Failed target clear", "Error", MB_OK);
	}
	HRESULT hr = m_pD3DDevice->BeginScene();

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(hr))
	{
		// �I�u�W�F�N�g�̕`��
		CScene::DrawAll();

#ifdef _DEBUG
		// �f�o�b�O����
		CDebugProc::Draw();
#endif

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}
	else if (hr == D3DERR_INVALIDCALL)
	{
		MessageBox(NULL, "Error begin scene", "Error", MB_OK);
		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�z
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
