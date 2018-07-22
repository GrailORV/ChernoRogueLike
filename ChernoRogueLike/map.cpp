//=============================================================================
//
// 3Dオブジェクトの処理 [scene3D.cpp]
// Author : 
//
//=============================================================================
#include "stdafx.h"

#include "map.h"
#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "manager.h"
#include "WinApp.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAP_WIDTH	50.f

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// 静的変数
//*****************************************************************************
MapLocation CMap::m_respawnPoint(1, 1);
uint8_t** CMap::m_mapState = nullptr;
uint16_t CMap::m_mapMaxX = 3;
uint16_t CMap::m_mapMaxZ = 3;

//=============================================================================
// CPlane生成
//=============================================================================
CMap *CMap::Create(const uint16_t MapSizeX, const uint16_t MapSizeZ)
{
	CMap *pMap;

	pMap = new CMap;
	pMap->Init(MapSizeX, MapSizeZ);

	return pMap;
}

//=============================================================================
// CPlaneコンストラクタ
//=============================================================================
CMap::CMap()
{
}

//=============================================================================
// CPlaneデストラクタ
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMap::Init(const uint16_t MapSizeX, const uint16_t MapSizeZ)
{
	if (MapSizeX < 3)
	{
		m_mapMaxX = 3;
	}
	else
	{
		m_mapMaxX = MapSizeX;
	}

	if (MapSizeZ < 3)
	{
		m_mapMaxZ = 3;
	}
	else
	{
		m_mapMaxZ = MapSizeZ;
	}

	m_mapState = new uint8_t*[m_mapMaxX];
	for (UINT x = 0; x < m_mapMaxX; x++)
	{
		m_mapState[x] = new uint8_t[m_mapMaxZ];
		for (UINT z = 0; z < m_mapMaxZ; z++)
		{
			if (x == 0 || x == m_mapMaxX - 1 ||
				z == 0 || z == m_mapMaxZ - 1)
			{
				m_mapState[x][z] = 1;
			}
			else if (x == m_mapMaxX / 2 && z == m_mapMaxZ / 2)
			{
				m_mapState[x][z] = 4;
			}
			else
			{
				m_mapState[x][z] = 0;
			}
		}
	}

	//m_respawnPoint = MapLocation(m_mapMaxX / (uint16_t)1.5, m_mapMaxZ / (uint16_t)1.5);

	m_fileName[0] = "data/MAP/TestMapAllIn.csv";

	//LoadMapText(m_fileName[0]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMap::Uninit(void)
{
	for (UINT x = 0; x < m_mapMaxX; x++)
	{
		if (!m_mapState[x])
		{
			continue;
		}
		delete[] m_mapState[x];
		m_mapState[x] = nullptr;
	}
	if (m_mapState)
	{
		delete[] m_mapState;
		m_mapState = nullptr;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CMap::Update(void)
{
	CDebugProc::Print("マップ\n");
	for (UINT z = 0; z < m_mapMaxZ; z++)
	{
		for (UINT x = 0; x < m_mapMaxX; x++)
		{
			CDebugProc::Print("%d ", m_mapState[x][z]);
		}
		CDebugProc::Print("\n");
	}

	CDebugProc::Print("RespawnPoint:%d,%d\n", m_respawnPoint.mapX, m_respawnPoint.mapZ);
}

//=============================================================================
// 描画処理
//=============================================================================
void CMap::Draw(void)
{

}

//=============================================================================
// ロケーションからマップ状態取得
//=============================================================================
uint8_t CMap::GetMapStateFromLocation(_In_ uint16_t x, _In_ uint16_t z)
{
	if (x >= m_mapMaxX || z >= m_mapMaxZ)
	{
		return 0;
	}

	return m_mapState[x][z];
}

//=============================================================================
// ロケーションからマップ状態セット
//=============================================================================
void CMap::SetMapStateFromLocation(_In_ uint16_t x, _In_ uint16_t z, _In_  uint8_t state)
{
	if (x >= m_mapMaxX || z >= m_mapMaxZ)
	{
		return;
	}

	m_mapState[x][z] = state;
}

//=============================================================================
// マップと実際の位置を同期
//=============================================================================
Vector3 CMap::MapPositionLink(MapLocation mapLocation)
{
	Vector3 SetPos = vector3NS::ZERO;
	float leftMapLimit = (GetMapWidth() - 1) * -25.0f;
	float depthMapLimit = (GetMapDepth() - 1) * 25.0f;

	SetPos = Vector3(
		leftMapLimit + 50.0f * mapLocation.mapX,
		0.1f,
		depthMapLimit - mapLocation.mapZ * 50.0f);

	return SetPos;
}

//=============================================================================
// マップ読み込み
//=============================================================================
void CMap::LoadMapText(const char *FileName)
{
	// ファイル構造体
	FILE *fp;

	CPlayer * pPlayer = CManager::GetPlayer();
	CEnemy *pEnemy = CManager::GetEnemy();

	// ファイルオープン
	fopen_s(&fp, FileName, "r");

	if (!fp)
	{
		MessageBox(NULL, "マップデータを読み込めませんでした。\n", NULL, MB_OK);
		return;
	}

	for (uint16_t nCntMapY = 0; nCntMapY < m_mapMaxZ; nCntMapY++)
	{
		for (uint16_t nCntMapX = 0; nCntMapX < m_mapMaxX; nCntMapX++)
		{
			fscanf_s(fp, "%d,", &m_MapState);
			m_mapState[nCntMapX][nCntMapY] = m_MapState;

			switch (m_MapState)
			{
			case MAP_STATE_FLOOR:
				break;

			case MAP_STATE_WALL:
			{
				CWall *pWall = pWall->Create(0, "torus", vector3NS::ZERO, vector3NS::ZERO, vector3NS::ONE);
				pWall->SetSpawnPoint(MapLocation(nCntMapX, nCntMapY));
				break;
			}
			
			case MAP_STATE_PLAYER:
			{
				m_respawnPoint = MapLocation(nCntMapX, nCntMapY);
				pPlayer->SetPosition(MapPositionLink(m_respawnPoint));
				break;
			}

			case MAP_STATE_ENEMY:
			{
				pEnemy->SetSpawnPoint(MapLocation(nCntMapX, nCntMapY));
				break;
			}

			case MAP_STATE_ITEM:
			{
				break;
			}

			case MAP_STATE_GOAL:
			{
				break;
			}

			}
		}
	}

	fclose(fp);
}