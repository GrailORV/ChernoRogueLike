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
<<<<<<< HEAD
#include "Menu.h"
=======
#include "enemy.h"
>>>>>>> c18c7c42d4c821fa7b84c67b0387663369610a35

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE		50.0f

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{

};

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
	CPlayer *pPlayer;

	pPlayer = new CPlayer;
	pPlayer->Init(nType, column, row, width, height, pos, rot, color);

	return pPlayer;
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
	m_moveBuff(0, 0),
	m_front(0, 0)
{
	m_iCount = 0;
	m_iTurn = 0;
	m_frameCount = 0;
	m_pos = vector3NS::ZERO;
	m_move = vector3NS::ZERO;
	m_bMove = false;
	m_inputEnable = false;
	m_inputSecondEnable = false;
	m_bTurningPlayer = false;
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
<<<<<<< HEAD
	HRESULT hr{};
	m_pMenu = CMenu::Create(1, 2, 4, 400.0f, 200.0f, D3DXVECTOR3(650.0f, 0, 0), D3DXVECTOR3(0, 0, 0), color);
=======
	m_currentMapLocation = CMap::GetRespawnPoint();
	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_PLAYER);

	CPlane::Init(nType, column, row, width, height, pos, rot, color);
>>>>>>> c18c7c42d4c821fa7b84c67b0387663369610a35

	m_pos = CMap::MapPositionLink(m_currentMapLocation);

	m_Status.ATK = 1;

	m_ATK = m_Status.ATK;

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
	Item();

	CDebugProc::Print("ターン数 : %d\n", m_iTurn);
	CDebugProc::Print("RotAngle : %f\n", D3DXToDegree(m_rot.y));
	CDebugProc::Print("%f,%f,%f\n", m_pos.x, m_pos.y, m_pos.z);

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
			m_pos = m_prePos + m_move;
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
	m_front = INT8_2(moveBuff.x, -moveBuff.z);

	if (CMap::GetMapStateFromLocation(m_currentMapLocation.mapX + moveBuff.x, m_currentMapLocation.mapZ - moveBuff.z) == CMap::MAP_STATE_WALL ||
		CMap::GetMapStateFromLocation(m_currentMapLocation.mapX + moveBuff.x, m_currentMapLocation.mapZ - moveBuff.z) == CMap::MAP_STATE_ENEMY ||
		m_bTurningPlayer)
	{
		return;
	}

	m_iTurn++;
	m_move += Vector3(moveBuff.x, 0.0f, moveBuff.z) * MOVE;
	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_FLOOR);
	m_currentMapLocation.mapX += moveBuff.x;
	m_currentMapLocation.mapZ -= moveBuff.z;
	CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_PLAYER);
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
		// 攻撃処理
		Attack();
	}

	// Pキーでメニューウィンドウ表示
	else if (pInputKeyboard->GetKeyTrigger(DIK_P))
	{
		// ウィンドウ表示

	}

	// 方向転換キー（押し続けている間）
	if (pInputKeyboard->GetKeyPress(DIK_O))
	{
		m_bTurningPlayer = true;
	}
	else
	{
		m_bTurningPlayer = false;
	}
}

//=============================================================================
// 攻撃処理
//=============================================================================
void CPlayer::Attack()
{
	CEnemy *pEnemy = CManager::GetEnemy();
	// 目の前に敵がいるか判定
	if (CMap::GetMapStateFromLocation(m_currentMapLocation.mapX + m_front.x, m_currentMapLocation.mapZ + m_front.z) == CMap::MAP_STATE_ENEMY)
	{
		// 攻撃力分のダメージ与える
		pEnemy->AddLife(-m_ATK);
		m_iTurn++;
	}

	// 敵がいなかったら空振り
	else
		m_iTurn++;
}

//=============================================================================
// アイテム取得処理
//=============================================================================
void CPlayer::Item()
{
	// 足元にアイテムがあるか判定
	if (CMap::GetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ) == CMap::MAP_STATE_ITEM)
	{
		/*アイテムを取得します。
		　インベントリに追加されたりされなかったり・・・
		 　今はとりあえずマップから消しときます*/
		CMap::SetMapStateFromLocation(m_currentMapLocation.mapX, m_currentMapLocation.mapZ, CMap::MAP_STATE_FLOOR);
	}
}