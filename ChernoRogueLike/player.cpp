//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : 小川　朔哉
//
//=============================================================================
#include "stdafx.h"

#include "player.h"
#include "WinApp.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE		50.0f

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// 静的変数
//*****************************************************************************
const int CPlayer::FRAME_MAX = 5;		// キー入力の実行までの待機時間 : 0.6秒
const UINT CPlayer::MOVE_FRAME = 20;

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
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objtype) :
	CPlane(nPriority, objtype),
	m_rotDest(vector3NS::ZERO),
	m_prePos(vector3NS::ZERO),
	m_moveFrameCnt(0),
	m_currentMapLocation(0, 0),
	m_moveBuff(0, 0)
{
	m_iCount = 0;
	m_iTurn = 0;
	m_frameCount = 0;
	m_pos = vector3NS::ZERO;
	m_move = vector3NS::ZERO;
	m_bMove = false;
	m_inputEnable = false;
	m_inputSecondEnable = false;
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
	m_currentMapLocation = CMap::GetRespawnPoint();
	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, 2);

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

	Behavior();
	Move();


	CDebugProc::Print("ターン数 : %d\n", m_iTurn);
	CDebugProc::Print("フレーム数 : %d\n", m_iCount);
	CDebugProc::Print("RotAngle : %f\n", D3DXToDegree(m_rot.y));
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CPlane::Draw();
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::Move(void)
{
	if (m_inputEnable)
	{
		InputMove(m_inputEnable);
	}
	else if (!m_bMove)
	{
		if (m_inputSecondEnable)
		{
			InputMove(m_inputSecondEnable);
		}
		m_frameCount++;
		if (m_frameCount > FRAME_MAX)
		{
			m_frameCount = 0;
			MoveMap(m_moveBuff);
		}
	}

	MovePosition(MOVE_FRAME);
}

//=============================================================================
// 実移動処理
//=============================================================================
void CPlayer::MovePosition(UINT moveFrame)
{
	if (m_bMove)
	{
		m_moveFrameCnt++;
		D3DXVec3Lerp(&m_pos, &m_prePos, &(m_prePos + m_move), float(m_moveFrameCnt) / float(moveFrame));
		if (m_moveFrameCnt > moveFrame)
		{
			m_moveFrameCnt = 0;
			m_move = vector3NS::ZERO;
			m_bMove = false;
			m_inputEnable = true;
			m_inputSecondEnable = true;
		}
	}
}

//=============================================================================
// マップ移動処理
//=============================================================================
void CPlayer::MoveMap(INT8_2 moveBuff)
{
	m_bMove = true;
	m_prePos = m_pos;
	m_moveBuff = INT8_2(0, 0);

	float angleBuff = (D3DX_PI / 2.0f - atan2f(float(moveBuff.z), float(moveBuff.x)));
	float rotAngle = (signbit(angleBuff) * 2.0f - 1.0f) * D3DX_PI + angleBuff;
	m_rot.y = rotAngle;

	if (CMap::GetMapStateFromLocation(m_currentMapLocation.mapX + moveBuff.x, m_currentMapLocation.mapZ - moveBuff.z) == CMap::MAP_STATE_WALL)
	{
		return;
	}

	m_iTurn++;
	m_move += Vector3(moveBuff.x, 0.0f, moveBuff.z) * MOVE;
	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, 0);
	m_currentMapLocation.mapX += moveBuff.x;
	m_currentMapLocation.mapZ -= moveBuff.z;
	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, 2);
}

//=============================================================================
// 移動の入力処理
//=============================================================================
void CPlayer::InputMove(bool& inputEnable)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

	// 上方向に移動
	if (pInputKeyboard->GetKeyPress(DIK_UP) && m_moveBuff.z <= 0)
	{
		m_moveBuff.z++;
		inputEnable = false;
		return;
	}

	// 下方向に移動
	if (pInputKeyboard->GetKeyPress(DIK_DOWN) && m_moveBuff.z >= 0)
	{
		m_moveBuff.z--;
		inputEnable = false;
		return;
	}
	// 左方向に移動
	if (pInputKeyboard->GetKeyPress(DIK_LEFT) && m_moveBuff.x >= 0)
	{
		m_moveBuff.x--;
		inputEnable = false;
		return;
	}
	// 右方向に移動
	if (pInputKeyboard->GetKeyPress(DIK_RIGHT) && m_moveBuff.x <= 0)
	{
		m_moveBuff.x++;
		inputEnable = false;
		return;
	}
	
}

//=============================================================================
// 行動の入力処理
//=============================================================================
void CPlayer::Behavior()
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();

	// 攻撃
	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))
	{
		Attack();
		return;
	}
}

//=============================================================================
// 攻撃処理
//=============================================================================
void CPlayer::Attack()
{
	// 目の前に敵がいるか判定
	if (CMap::GetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ) == CMap::MAP_STATE_ENEMY)
	{

		m_iTurn++;
	}
	// いなかった場合、空振りしてターン消費
	else
		m_iTurn++;
}