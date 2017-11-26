//=============================================================================
//
// カメラの処理 [camera.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"

//*********************************************************
// カメラクラス
//*********************************************************
interface CCamera :public IUnknown
{
protected:
	const float VIEW_ANGLE = D3DXToRadian(60.0f);	// 視野角
	const float VIEW_NEAR_Z = 10.0f;					// ビュー平面のNearZ値
	const float	VIEW_FAR_Z = 10000.0f;				// ビュー平面のFarZ値

	const float VALUE_MOVE = 2.0f;					// カメラの移動量
	const float VALUE_ROTATE = D3DX_PI * 0.01f;		// カメラの回転量
	const float VALUE_MOVE_DRAG = 0.30f;					// ドラッグ時の移動量
	const float VALUE_ROTATE_MOUSE = 0.003f;				// マウスでの回転量

	const float CAM_LENGTH_RATE = 5.0f;					// ホイール入力係数
	const float CAM_LENGTH_LIMIT_MIN = 100.0f;				// 視点と注視点の距離最小値
	const float CAM_LENGTH_LIMIT_MAX = 1000.0f;				// 視点と注視点の距離最大値

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

	D3DXMATRIX m_mtxView;			// ビューマトリックス
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス

	D3DXVECTOR3 m_posP;				// 視点
	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXVECTOR3 m_rot;				// 向き
	float m_fLengthInterval;		// 視点から注視点までの距離
	float m_fLengthIntervalXZ;		// 視点から注視点までの距離(XZ平面)

	D3DXQUATERNION m_quatRot;
	D3DXMATRIX m_mtxRot;
};

