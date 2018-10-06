#pragma once

#include "model.h"
#include "map.h"

class CWall : public CModel
{
public:
	CWall();
	~CWall();

	static CWall *Create(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale);
	HRESULT Init(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale);
	void Uninit();
	void Update();
	void Draw();

	void SetSpawnPoint(MapLocation mapLocation) { m_currentMapLocation = mapLocation; }
	MapLocation GetSpawnPoint(void) { return m_currentMapLocation; }

private:
	MapLocation m_currentMapLocation;
};