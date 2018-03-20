//=============================================================================
//
// カメラの処理 [camera.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "main.h"

//*********************************************************
// カメラクラス
//*********************************************************
class CCamera :public IUnknown
{
protected:
	const float VIEW_ANGLE = D3DXToRadian(60.0f);	// 視野角
	const float VIEW_NEAR_Z = 10.0f;					// ビュー平面のNearZ値
	const float	VIEW_FAR_Z = 10000.0f;				// ビュー平面のFarZ値

	const float VALUE_MOVE = 10.0f;					// カメラの移動量
	const float VALUE_ROTATE = D3DX_PI * 0.01f;		// カメラの回転量
	const float VALUE_MOVE_DRAG = 0.30f;					// ドラッグ時の移動量
	const float VALUE_ROTATE_MOUSE = 0.003f;				// マウスでの回転量

	const float CAM_LENGTH_RATE = 5.0f;					// ホイール入力係数
	const float CAM_LENGTH_LIMIT_MIN = 100.0f;				// 視点と注視点の距離最小値
	const float CAM_LENGTH_LIMIT_MAX = 10000.0f;				// 視点と注視点の距離最大値

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

	Matrix m_mtxView;			// ビューマトリックス
	Matrix m_mtxProjection;		// プロジェクションマトリックス

	Vector3 m_posP;				// 視点
	Vector3 m_posR;				// 注視点
	Vector3 m_vecU;				// 上方向ベクトル
	Vector3 m_rot;				// 向き
	float m_fLengthInterval;		// 視点から注視点までの距離
	float m_fLengthIntervalXZ;		// 視点から注視点までの距離(XZ平面)

	Quaternion m_quatRot;
	Matrix m_mtxRot;
};

