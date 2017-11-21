//=============================================================================
//
// カメラの処理 [camera.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "WinApp.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 静的変数
//*****************************************************************************


//=============================================================================
// CCameraコンストラクタ
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
// CCameraデストラクタ
//=============================================================================
CCamera::~CCamera()
{
	Release();
}

//=============================================================================
// 参照コピー
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
// 参照カウンタインクリメント
//=============================================================================
ULONG CCamera::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// 終了処理
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
// CCamera初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	SetPosition(m_posP, m_posR);

	return S_OK;
}

//=============================================================================
// CCamera更新処理
//=============================================================================
void CCamera::Update(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));

	CInputKeyboard *pInputKeyboard;
	CInputMouse *pInputMouse;
	float fValueX, fValueY, fValueZ;

	// キーボード取得
	pInputKeyboard = pManager->GetInputKeyboard();

	// マウス取得
	pInputMouse = pManager->GetInputMouse();
	fValueX = (float)pInputMouse->GetAxisX();
	fValueY = (float)pInputMouse->GetAxisY();
	fValueZ = (float)pInputMouse->GetAxisZ();

	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		if (pInputKeyboard->GetKeyPress(DIK_W))
		{// 左前移動
			m_posP.x -= cosf(m_rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			m_posP.z += sinf(m_rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else if (pInputKeyboard->GetKeyPress(DIK_S))
		{// 左後移動
			m_posP.x -= cosf(m_rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			m_posP.z += sinf(m_rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// 左移動
			m_posP.x -= cosf(m_rot.y) * VALUE_MOVE;
			m_posP.z += sinf(m_rot.y) * VALUE_MOVE;
		}

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	else if (pInputKeyboard->GetKeyPress(DIK_D))
	{// 右移動
		if (pInputKeyboard->GetKeyPress(DIK_W))
		{// 右前移動
			m_posP.x += cosf(m_rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			m_posP.z -= sinf(m_rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else if (pInputKeyboard->GetKeyPress(DIK_S))
		{// 右後移動
			m_posP.x += cosf(m_rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			m_posP.z -= sinf(m_rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// 右移動
			m_posP.x += cosf(m_rot.y) * VALUE_MOVE;
			m_posP.z -= sinf(m_rot.y) * VALUE_MOVE;
		}

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	else if (pInputKeyboard->GetKeyPress(DIK_W))
	{// 前移動
		m_posP.x += sinf(m_rot.y) * VALUE_MOVE;
		m_posP.z += cosf(m_rot.y) * VALUE_MOVE;

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	else if (pInputKeyboard->GetKeyPress(DIK_S))
	{// 後移動
		m_posP.x -= sinf(m_rot.y) * VALUE_MOVE;
		m_posP.z -= cosf(m_rot.y) * VALUE_MOVE;

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}

	if (pInputKeyboard->GetKeyPress(DIK_T))
	{// 注視点移動「上」
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
	{// 注視点移動「下」
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
	{// 注視点移動「左」
		m_rot.y -= VALUE_ROTATE;
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI*2.0f;
		}

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	if (pInputKeyboard->GetKeyPress(DIK_E))
	{// 注視点移動「右」
		m_rot.y += VALUE_ROTATE;
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI*2.0f;
		}

		m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
	}

	if (pInputKeyboard->GetKeyPress(DIK_Y))
	{// 視点移動「上」
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
	{// 視点移動「下」
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
	{// 視点移動「左」
		m_rot.y += VALUE_ROTATE;
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI*2.0f;
		}

		m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
	}
	if (pInputKeyboard->GetKeyPress(DIK_C))
	{// 視点移動「右」
		m_rot.y -= VALUE_ROTATE;
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI*2.0f;
		}

		m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
		m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
	}

	if (fValueZ)
	{// 注視点から視点の距離変更
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
		{// 移動
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
		{// 回転
			if (fValueX < 0)
			{// 視点移動「左」
				m_rot.y += D3DX_PI * fValueX * VALUE_ROTATE_MOUSE;
				if (m_rot.y < -D3DX_PI)
				{
					m_rot.y += D3DX_PI*2.0f;
				}

				m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
				m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
			}
			else if (fValueX > 0)
			{// 視点移動「右」
				m_rot.y += D3DX_PI * fValueX * VALUE_ROTATE_MOUSE;
				if (m_rot.y > D3DX_PI)
				{
					m_rot.y -= D3DX_PI*2.0f;
				}

				m_posP.x = m_posR.x - sinf(m_rot.y) * m_fLengthIntervalXZ;
				m_posP.z = m_posR.z - cosf(m_rot.y) * m_fLengthIntervalXZ;
			}

			if (fValueY < 0)
			{// 視点移動「上」
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
			{// 視点移動「下」
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
		{// 注視点移動「左」
			m_rot.y += D3DX_PI * fValueX * VALUE_ROTATE_MOUSE;
			if (m_rot.y < -D3DX_PI)
			{
				m_rot.y += D3DX_PI*2.0f;
			}

			m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
			m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
		}
		else if (fValueX > 0)
		{// 注視点移動「右」
			m_rot.y += D3DX_PI * fValueX * VALUE_ROTATE_MOUSE;
			if (m_rot.y > D3DX_PI)
			{
				m_rot.y -= D3DX_PI*2.0f;
			}

			m_posR.x = m_posP.x + sinf(m_rot.y) * m_fLengthIntervalXZ;
			m_posR.z = m_posP.z + cosf(m_rot.y) * m_fLengthIntervalXZ;
		}

		if (fValueY < 0)
		{// 注視点移動「上」
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
		{// 注視点移動「下」
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
// カメラ位置設定
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
// 描画処理
//=============================================================================
void CCamera::SetCamera(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// ビュー行列の初期化
	D3DXMatrixIdentity(&m_mtxView);

	// カメラの位置と方向を設定
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posP,			// カメラの視点
		&m_posR,			// カメラの注視点
		&m_vecU);			// カメラの上の方向

	// ビュー行列の設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// ビューポートの取得
	D3DVIEWPORT9 vp;
	if (FAILED(pDevice->GetViewport(&vp)))
	{
		return;
	}

	// 射影行列の初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// 画角と描画範囲を設定
	float fAspect = (float)vp.Width / (float)vp.Height;
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, VIEW_ANGLE, fAspect, VIEW_NEAR_Z, VIEW_FAR_Z);

	// 射影行列の設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

