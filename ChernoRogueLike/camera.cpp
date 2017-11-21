//=============================================================================
//
// �J�����̏��� [camera.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "WinApp.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************


//=============================================================================
// CCamera�R���X�g���N�^
//=============================================================================
CCamera::CCamera() :m_dwRef(0)
{
	float fLengthXZ;

	m_posP = D3DXVECTOR3(0.0f, 150.0f, -300.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fLengthInterval = sqrtf((m_posP.x - m_posR.x) * (m_posP.x - m_posR.x)
		+ (m_posP.y - m_posR.y) * (m_posP.y - m_posR.y)
		+ (m_posP.z - m_posR.z) * (m_posP.z - m_posR.z));
	fLengthXZ = sqrtf((m_posP.x - m_posR.x) * (m_posP.x - m_posR.x)
		+ (m_posP.z - m_posR.z) * (m_posP.z - m_posR.z));
	m_rot.y = atan2f((m_posR.x - m_posP.x), (m_posR.z - m_posP.z));
	m_rot.x = atan2f((m_posR.y - m_posP.y), fLengthXZ);

	m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;

	D3DXMatrixIdentity(&m_mtxRot);
	D3DXQuaternionIdentity(&m_quatRot);
}

//=============================================================================
// CCamera�f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
	Release();
}

//=============================================================================
// �Q�ƃR�s�[
//=============================================================================
HRESULT CCamera::QueryInterface(REFIID riid, void FAR* FAR* ppvObject)
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
ULONG CCamera::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// �I������
//=============================================================================
ULONG CCamera::Release(void)
{
	if (--m_dwRef == 0)
	{
		delete this;
		return 0;
	}
	return m_dwRef;
}

//=============================================================================
// CCamera����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	SetPosition(m_posP, m_posR);

	return S_OK;
}

//=============================================================================
// CCamera�X�V����
//=============================================================================
void CCamera::Update(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));

	CInputKeyboard *pInputKeyboard;
	CInputMouse *pInputMouse;
	float fValueX, fValueY, fValueZ;

	// �L�[�{�[�h�擾
	pInputKeyboard = pManager->GetInputKeyboard();

	// �}�E�X�擾
	pInputMouse = pManager->GetInputMouse();
	fValueX = (float)pInputMouse->GetAxisX();
	fValueY = (float)pInputMouse->GetAxisY();
	fValueZ = (float)pInputMouse->GetAxisZ();

	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		if (pInputKeyboard->GetKeyPress(DIK_W))
		{// ���O�ړ�
			m_posP.x -= cosf(m_rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			m_posP.z += sinf(m_rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else if (pInputKeyboard->GetKeyPress(DIK_S))
		{// ����ړ�
			m_posP.x -= cosf(m_rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			m_posP.z += sinf(m_rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// ���ړ�
			m_posP.x -= cosf(m_rot.y) * VALUE_MOVE;
			m_posP.z += sinf(m_rot.y) * VALUE_MOVE;
		}

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	else if (pInputKeyboard->GetKeyPress(DIK_D))
	{// �E�ړ�
		if (pInputKeyboard->GetKeyPress(DIK_W))
		{// �E�O�ړ�
			m_posP.x += cosf(m_rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			m_posP.z -= sinf(m_rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else if (pInputKeyboard->GetKeyPress(DIK_S))
		{// �E��ړ�
			m_posP.x += cosf(m_rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			m_posP.z -= sinf(m_rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// �E�ړ�
			m_posP.x += cosf(m_rot.y) * VALUE_MOVE;
			m_posP.z -= sinf(m_rot.y) * VALUE_MOVE;
		}

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	else if (pInputKeyboard->GetKeyPress(DIK_W))
	{// �O�ړ�
		m_posP.x += sinf(m_rot.y) * VALUE_MOVE;
		m_posP.z += cosf(m_rot.y) * VALUE_MOVE;

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	else if (pInputKeyboard->GetKeyPress(DIK_S))
	{// ��ړ�
		m_posP.x -= sinf(m_rot.y) * VALUE_MOVE;
		m_posP.z -= cosf(m_rot.y) * VALUE_MOVE;

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}

	if (pInputKeyboard->GetKeyPress(DIK_T))
	{// �����_�ړ��u��v
		m_rot.x += VALUE_ROTATE;
		if (m_rot.x > (D3DX_PI / 2.0f - D3DX_PI*0.1f))
		{
			m_rot.x = (D3DX_PI / 2.0f - D3DX_PI*0.1f);
		}

		m_posR.y = m_posP.y + sinf(m_rot.x) * m_fLengthInterval;

		m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;
		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	if (pInputKeyboard->GetKeyPress(DIK_B))
	{// �����_�ړ��u���v
		m_rot.x -= VALUE_ROTATE;
		if (m_rot.x < (-D3DX_PI / 2.0f + D3DX_PI*0.1f))
		{
			m_rot.x = (-D3DX_PI / 2.0f + D3DX_PI*0.1f);
		}

		m_posR.y = m_posP.y + sinf(m_rot.x) * m_fLengthInterval;

		m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;
		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	if (pInputKeyboard->GetKeyPress(DIK_Q))
	{// �����_�ړ��u���v
		m_rot.y -= VALUE_ROTATE;
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI*2.0f;
		}

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	if (pInputKeyboard->GetKeyPress(DIK_E))
	{// �����_�ړ��u�E�v
		m_rot.y += VALUE_ROTATE;
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI*2.0f;
		}

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}

	if (pInputKeyboard->GetKeyPress(DIK_Y))
	{// ���_�ړ��u��v
		m_rot.x -= VALUE_ROTATE;
		if (m_rot.x < (-D3DX_PI / 2.0f + D3DX_PI*0.02f))
		{
			m_rot.x = (-D3DX_PI / 2.0f + D3DX_PI*0.02f);
		}

		m_posP.y = m_posR.y - sinf(m_rot.x) * m_fLengthInterval;

		m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;
		m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	if (pInputKeyboard->GetKeyPress(DIK_N))
	{// ���_�ړ��u���v
		m_rot.x += VALUE_ROTATE;
		if (m_rot.x > (D3DX_PI / 2.0f - D3DX_PI*0.02f))
		{
			m_rot.x = (D3DX_PI / 2.0f - D3DX_PI*0.02f);
		}

		m_posP.y = m_posR.y - sinf(m_rot.x) * m_fLengthInterval;

		m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;
		m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	if (pInputKeyboard->GetKeyPress(DIK_Z))
	{// ���_�ړ��u���v
		m_rot.y += VALUE_ROTATE;
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI*2.0f;
		}

		m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	if (pInputKeyboard->GetKeyPress(DIK_C))
	{// ���_�ړ��u�E�v
		m_rot.y -= VALUE_ROTATE;
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI*2.0f;
		}

		m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
	}

	if (fValueZ)
	{// �����_���王�_�̋����ύX
		m_fLengthInterval += fValueZ / CAM_LENGTH_RATE;
		if (m_fLengthInterval < CAM_LENGTH_LIMIT_MIN)
		{
			m_fLengthInterval = CAM_LENGTH_LIMIT_MIN;
		}
		else if (m_fLengthInterval > CAM_LENGTH_LIMIT_MAX)
		{
			m_fLengthInterval = CAM_LENGTH_LIMIT_MAX;
		}
		m_posP.y = m_posR.y - sinf(m_rot.x) * m_fLengthInterval;

		m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;
		m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
	}

	if (pInputMouse->GetLeftPress())
	{
		if (pInputMouse->GetRightPress())
		{// �ړ�
			if (fValueX != 0 || fValueY != 0)
			{
				float angle = atan2f(fValueX, fValueY);
				float length = sqrtf(fValueX*fValueX + fValueY*fValueY) * VALUE_MOVE_DRAG;

				angle -= m_rot.y;
				if (angle < -D3DX_PI) angle += D3DX_PI * 2.0f;

				m_posR.x -= sinf(angle) * length;
				m_posR.z += cosf(angle) * length;

				m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
				m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
			}
		}
		else
		{// ��]
			if (fValueX < 0)
			{// ���_�ړ��u���v
				m_rot.y += D3DX_PI * fValueX * VALUE_ROTATE_MOUSE;
				if (m_rot.y < -D3DX_PI)
				{
					m_rot.y += D3DX_PI*2.0f;
				}

				m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
				m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
			}
			else if (fValueX > 0)
			{// ���_�ړ��u�E�v
				m_rot.y += D3DX_PI * fValueX * VALUE_ROTATE_MOUSE;
				if (m_rot.y > D3DX_PI)
				{
					m_rot.y -= D3DX_PI*2.0f;
				}

				m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
				m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
			}

			if (fValueY < 0)
			{// ���_�ړ��u��v
				m_rot.x -= D3DX_PI * fValueY * VALUE_ROTATE_MOUSE;
				if (m_rot.x > (D3DX_PI / 2.0f - D3DX_PI*0.02f))
				{
					m_rot.x = (D3DX_PI / 2.0f - D3DX_PI*0.02f);
				}

				m_posP.y = m_posR.y - sinf(m_rot.x) * m_fLengthInterval;

				m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;
				m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
				m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
			}
			else if (fValueY > 0)
			{// ���_�ړ��u���v
				m_rot.x -= D3DX_PI * fValueY * VALUE_ROTATE_MOUSE;
				if (m_rot.x < (-D3DX_PI / 2.0f + D3DX_PI*0.02f))
				{
					m_rot.x = (-D3DX_PI / 2.0f + D3DX_PI*0.02f);
				}

				m_posP.y = m_posR.y - sinf(m_rot.x) * m_fLengthInterval;

				m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;
				m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
				m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
			}
		}
	}
	else if (pInputMouse->GetRightPress())
	{
		if (fValueX < 0)
		{// �����_�ړ��u���v
			m_rot.y += D3DX_PI * fValueX * VALUE_ROTATE_MOUSE;
			if (m_rot.y < -D3DX_PI)
			{
				m_rot.y += D3DX_PI*2.0f;
			}

			m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
			m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
		}
		else if (fValueX > 0)
		{// �����_�ړ��u�E�v
			m_rot.y += D3DX_PI * fValueX * VALUE_ROTATE_MOUSE;
			if (m_rot.y > D3DX_PI)
			{
				m_rot.y -= D3DX_PI*2.0f;
			}

			m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
			m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
		}

		if (fValueY < 0)
		{// �����_�ړ��u��v
			m_rot.x -= D3DX_PI * fValueY * VALUE_ROTATE_MOUSE;
			if (m_rot.x > (D3DX_PI / 2.0f - D3DX_PI*0.1f))
			{
				m_rot.x = (D3DX_PI / 2.0f - D3DX_PI*0.1f);
			}

			m_posR.y = m_posP.y + sinf(m_rot.x) * m_fLengthInterval;

			m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;
			m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
			m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
		}
		else if (fValueY > 0)
		{// �����_�ړ��u���v
			m_rot.x -= D3DX_PI * fValueY * VALUE_ROTATE_MOUSE;
			if (m_rot.x < (-D3DX_PI / 2.0f + D3DX_PI*0.1f))
			{
				m_rot.x = (-D3DX_PI / 2.0f + D3DX_PI*0.1f);
			}

			m_posR.y = m_posP.y + sinf(m_rot.x) * m_fLengthInterval;

			m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;
			m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
			m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
		}
	}
}

//=============================================================================
// �J�����ʒu�ݒ�
//=============================================================================
void CCamera::SetPosition(D3DXVECTOR3 posP, D3DXVECTOR3 posR)
{
	float fLengthXZ;

	m_fLengthInterval = sqrtf((posR.x - posP.x) * (posR.x - posP.x)
		+ (posR.y - posP.y) * (posR.y - posP.y)
		+ (posR.z - posP.z) * (posR.z - posP.z));
	fLengthXZ = sqrtf((posR.x - posP.x) * (posR.x - posP.x)
		+ (posR.z - posP.z) * (posR.z - posP.z));

	m_posP = posP;
	m_posR = posR;
	m_rot.y = atan2f((posR.x - posP.x), (posR.z - posP.z));
	m_rot.x = atan2f((posR.y - posP.y), fLengthXZ);

	m_fLengthIntervalXZ = cosf(m_rot.x) * m_fLengthInterval;

	if (fLengthXZ == 0.0f)
	{
		m_vecU = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}

	D3DXQuaternionRotationYawPitchRoll(&m_quatRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixRotationQuaternion(&m_mtxRot, &m_quatRot);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCamera::SetCamera(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// �r���[�s��̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �J�����̈ʒu�ƕ�����ݒ�
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posP,			// �J�����̎��_
		&m_posR,			// �J�����̒����_
		&m_vecU);			// �J�����̏�̕���

	// �r���[�s��̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �r���[�|�[�g�̎擾
	D3DVIEWPORT9 vp;
	if (FAILED(pDevice->GetViewport(&vp)))
	{
		return;
	}

	// �ˉe�s��̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// ��p�ƕ`��͈͂�ݒ�
	float fAspect = (float)vp.Width / (float)vp.Height;
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, VIEW_ANGLE, fAspect, VIEW_NEAR_Z, VIEW_FAR_Z);

	// �ˉe�s��̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

