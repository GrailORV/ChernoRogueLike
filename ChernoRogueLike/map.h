//=============================================================================
//
// �}�b�v���� [map.h]
// Author : ���� ���
//
//=============================================================================
#pragma once
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAP_SIZE_X	3
#define MAP_SIZE_Z	3

//*****************************************************************************
// �\����
//*****************************************************************************
typedef enum
{
	OBJ_NONE = 0,
	OBJ_WALL,
	OBJ_PLAYER,
}OBJ_TYPE;

struct MapLocation
{
	uint16_t mapX, mapZ;

	MapLocation(uint16_t x, uint16_t z)
	{
		mapX = x; mapZ = z;
	}
};

//*****************************************************************************
// �}�b�v�N���X
//*****************************************************************************
class CMap : public CScene
{
public:
	CMap();
	~CMap();

	enum MAP_STATE
	{
		MAP_STATE_FLOOR = 0,
		MAP_STATE_WALL,
		MAP_STATE_PLAYER
	};

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

	static MapLocation GetRespawnPoint(void) { return m_respawnPoint; }

	static uint8_t GetMapStateFromLocation(_In_ uint16_t x, _In_ uint16_t z);
	static void SetMapStateFromLocation(_In_ uint16_t x, _In_ uint16_t z, _In_ uint8_t state);

protected:
	static MapLocation m_respawnPoint;
	static uint8_t** m_mapState;

	static uint16_t m_mapMaxX;
	static uint16_t m_mapMaxZ;
};