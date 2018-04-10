//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : 小川　朔哉
//
//=============================================================================
#include "stdafx.h"

#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "camera.h"
#include "WinApp.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE		10.f

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// 静的変数
//*****************************************************************************
const int CPlayer::FLAME_MAX = 5;		// キー入力の実行までの待機時間 : 0.6秒


//=============================================================================
// CPlane生成
//=============================================================================
CPlayer *CPlayer::Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CPlayer *pPlane;

	pPlane = new CPlayer;
	pPlane->Init(nType, column, row, width, height, pos, rot, color);

	return pPlane;
}

//=============================================================================
// CPlaneコンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType)
{
	m_iCount = 0;
	m_iTurn = 0;
}

//=============================================================================
// CPlaneデストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CPlane::Init(nType, column, row, width, height, pos, rot, color);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// オブジェクトを破棄
	CPlane::Uninit();

}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CCamera *pCamera = pManager->GetCamera();
	CInputKeyboard *pInputKeyboard;
	// キーボード取得
	pInputKeyboard = pManager->GetInputKeyboard();

	if (m_inputEnable)
	{
		if (pInputKeyboard->GetKeyPress(DIK_UP))
		{
			m_moveBuff.z += MOVE;
			m_inputEnable = false;
		}
		if (pInputKeyboard->GetKeyPress(DIK_DOWN))
		{
			m_moveBuff.z += -MOVE;
			m_inputEnable = false;
		}
		if (pInputKeyboard->GetKeyPress(DIK_LEFT))
		{
			m_moveBuff.x += -MOVE;
			m_inputEnable = false;
		}
		if (pInputKeyboard->GetKeyPress(DIK_RIGHT))
		{
			m_moveBuff.x += MOVE;
			m_inputEnable = false;
		}
	}
	else if (!m_bMove)
	{
		if (m_inputSecondEnable)
		{
			if (pInputKeyboard->GetKeyPress(DIK_UP))
			{
				m_moveBuff.z += MOVE;
				m_inputSecondEnable = false;
			}
			if (pInputKeyboard->GetKeyPress(DIK_DOWN))
			{
				m_moveBuff.z += -MOVE;
				m_inputSecondEnable = false;
			}
			if (pInputKeyboard->GetKeyPress(DIK_LEFT))
			{
				m_moveBuff.x += -MOVE;
				m_inputSecondEnable = false;
			}
			if (pInputKeyboard->GetKeyPress(DIK_RIGHT))
			{
				m_moveBuff.x += MOVE;
				m_inputSecondEnable = false;
			}
		}
		m_frameCount++;
		if (m_frameCount > FLAME_MAX)
		{
			m_frameCount = 0;
			if (D3DXVec3Length(&m_moveBuff) <= 0.0f)
			{
				m_inputEnable = true;
				m_inputSecondEnable = true;
			}
			else
			{
				m_bMove = true;
				m_iTurn++;
				m_move += m_moveBuff;
				m_moveBuff = Vector3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	if (m_bMove)
	{
		if (D3DXVec3Length(&m_move) <= 0.0f)
		{
			m_bMove = false;
			m_inputEnable = true;
			m_inputSecondEnable = true;
		}
	}

	// 回転方向補正
	float fDiffRotY = m_rotDest.y - m_rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 回転
	m_rot.y += fDiffRotY * 0.2f;
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_pos += m_move;
	m_move *= 0.8f;
	if (D3DXVec3Length(&m_move) < 0.01f)
	{
		m_move = Vector3(0.0f, 0.0f, 0.0f);
	}

	CDebugProc::Print("ターン数 : %d\n", m_iTurn);
	CDebugProc::Print("フレーム数 : %d\n", m_iCount);

}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CPlane::Draw();
}