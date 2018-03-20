//=============================================================================
//
// �J�����̏��� [camera.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "main.h"

//*********************************************************
// �J�����N���X
//*********************************************************
class CCamera :public IUnknown
{
protected:
	const float VIEW_ANGLE = D3DXToRadian(60.0f);	// ����p
	const float VIEW_NEAR_Z = 10.0f;					// �r���[���ʂ�NearZ�l
	const float	VIEW_FAR_Z = 10000.0f;				// �r���[���ʂ�FarZ�l

	const float VALUE_MOVE = 10.0f;					// �J�����̈ړ���
	const float VALUE_ROTATE = D3DX_PI * 0.01f;		// �J�����̉�]��
	const float VALUE_MOVE_DRAG = 0.30f;					// �h���b�O���̈ړ���
	const float VALUE_ROTATE_MOUSE = 0.003f;				// �}�E�X�ł̉�]��

	const float CAM_LENGTH_RATE = 5.0f;					// �z�C�[�����͌W��
	const float CAM_LENGTH_LIMIT_MIN = 100.0f;				// ���_�ƒ����_�̋����ŏ��l
	const float CAM_LENGTH_LIMIT_MAX = 10000.0f;				// ���_�ƒ����_�̋����ő�l

public:
	CCamera();
	~CCamera();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(void);
	void Update(void);

	void SetPosition(Vector3 posP, Vector3 posR);

	Vector3 GetPosP(void) { return m_posP; }
	Vector3 GetPosR(void) { return m_posR; }
	Vector3 GetVecU(void) { return m_vecU; }
	Vector3 GetRot(void) { return m_rot; }

	void SetCamera(void);

	Matrix GetViewMatrix(void) { return m_mtxView; }
	Matrix GetProjectionMatrix(void) { return m_mtxProjection; }

private:
	DWORD m_dwRef;

	Matrix m_mtxView;			// �r���[�}�g���b�N�X
	Matrix m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X

	Vector3 m_posP;				// ���_
	Vector3 m_posR;				// �����_
	Vector3 m_vecU;				// ������x�N�g��
	Vector3 m_rot;				// ����
	float m_fLengthInterval;		// ���_���璍���_�܂ł̋���
	float m_fLengthIntervalXZ;		// ���_���璍���_�܂ł̋���(XZ����)

	Quaternion m_quatRot;
	Matrix m_mtxRot;
};

