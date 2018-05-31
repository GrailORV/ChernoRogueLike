//=============================================================================
//
// マップ処理 [map.h]
// Author : 小川 朔哉
//
//=============================================================================
#pragma once
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAP_SIZE_X	3
#define MAP_SIZE_Z	3
#define MAX_MAP	1

//*****************************************************************************
// 構造体
//*****************************************************************************
struct MapLocation
{
	uint16_t mapX, mapZ;

	MapLocation(uint16_t x, uint16_t z)
	{
		mapX = x; mapZ = z;
	}
};

//*****************************************************************************
// マップクラス
//*****************************************************************************
class CMap : public CScene
{
public:
	CMap();
	~CMap();

	typedef enum
	{
		MAP_STATE_FLOOR = 0,
		MAP_STATE_WALL,
		MAP_STATE_PLAYER,
		MAP_STATE_ENEMY,
		MAP_STATE_ITEM,
		MAP_STATE_GOAL,
		MAP_STATE_MAX
	}MAP_STATE;

	static CMap *Create(const uint16_t MapSizeX, const uint16_t MapSizeZ);

	HRESULT Init(const uint16_t MapSizeX, const uint16_t MapSizeZ);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	Vector3 GetPosition(void) { return vector3NS::ZERO; }
	void SetPosition(Vector3 pos) {}

	Vector3 GetRotation(void) { return vector3NS::ZERO; }
	void SetRotation(Vector3 rot) {}

	static uint32_t GetMapWidth() { return m_mapMaxX; }
	static uint32_t GetMapDepth() { return m_mapMaxZ; }

	static MapLocation GetRespawnPoint(void) { return m_respawnPoint; }

	static uint8_t GetMapStateFromLocation(_In_ uint16_t x, _In_ uint16_t z);
	static void SetMapStateFromLocation(_In_ uint16_t x, _In_ uint16_t z, _In_ uint8_t state);
	
	static Vector3 MapPositionLink(MapLocation mapLocation);

	void LoadMapText(const char *FileName);

protected:
	static MapLocation m_respawnPoint;
	static uint8_t** m_mapState;

	static uint16_t m_mapMaxX;
	static uint16_t m_mapMaxZ;

private:
	const char *m_fileName[MAX_MAP];

	MAP_STATE m_MapState;
};