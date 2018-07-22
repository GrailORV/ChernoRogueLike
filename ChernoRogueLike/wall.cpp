#include "stdafx.h"
#include "wall.h"
#include "map.h"
#include "WinApp.h"


CWall *CWall::Create(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale)
{
	CWall *pWall = new CWall;
	pWall->Init(nType, modelID, pos, rot, scale);
	return pWall;
}

CWall::CWall():
	m_currentMapLocation(0,0)
{

}

CWall::~CWall()
{

}

HRESULT CWall::Init(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale)
{
	CModel::Init(nType, modelID, pos, rot, scale);

	m_pos = CMap::MapPositionLink(m_currentMapLocation);
	
	return S_OK;
}

void CWall::Uninit()
{
	CModel::Uninit();
}

void CWall::Update()
{

}

void CWall::Draw()
{

}