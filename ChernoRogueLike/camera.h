//=============================================================================
//
// �J�����̏��� [camera.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"

//*********************************************************
// �J�����N���X
//*********************************************************
interface CCamera :public IUnknown
{
protected:
	const float VIEW_ANGLE = D3DXToRadian(60.0f);	// ����p
	const float VIEW_NEAR_Z = 10.0f;					// �r���[���ʂ�NearZ�l
	const float	VIEW_FAR_Z = 10000.0f;				// �r���[���ʂ�FarZ�l

	const float VALUE_MOVE = 2.0f;					// �J�����̈ړ���
	const float VALUE_ROTATE = D3DX_PI * 0.01f;		// �J�����̉�]��
	const float VALUE_MOVE_DRAG = 0.30f;					// �h���b�O���̈ړ���
	const float VALUE_ROTATE_MOUSE = 0.003f;				// �}�E�X�ł̉�]��

	const float CAM_LENGTH_RATE = 5.0f;					// �z�C�[�����͌W��
	const float CAM_LENGTH_LIMIT_MIN = 100.0f;				// ���_�ƒ����_�̋����ŏ��l
	const float CAM_LENGTH_LIMIT_MAX = 1000.0f;				// ���_�ƒ����_�̋����ő�l

public:
	CCamera();
	~CCamera();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(void);
	void Update(void);

	void SetPosition(D3DXVECTOR3 posP, D3DXVECTOR3 posR);

	D3DXVECTOR3 GetPosP(void) { return m_posP; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetVecU(void) { return m_vecU; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	void SetCamera(void);

	D3DXMATRIX GetViewMatrix(void) { return m_mtxView; }
	D3DXMATRIX GetProjectionMatrix(void) { return m_mtxProjection; }

private:
	DWORD m_dwRef;

	D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X

	D3DXVECTOR3 m_posP;				// ���_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXVECTOR3 m_rot;				// ����
	float m_fLengthInterval;		// ���_���璍���_�܂ł̋���
	float m_fLengthIntervalXZ;		// ���_���璍���_�܂ł̋���(XZ����)

	D3DXQUATERNION m_quatRot;
	D3DXMATRIX m_mtxRot;
};

