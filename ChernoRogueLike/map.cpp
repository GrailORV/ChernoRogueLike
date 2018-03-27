//=============================================================================
//
// 3Dオブジェクトの処理 [scene3D.cpp]
// Author : 
//
//=============================================================================
#include "stdafx.h"

#include "map.h"
#include "manager.h"
#include "WinApp.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// 静的変数
//*****************************************************************************


//=============================================================================
// CPlane生成
//=============================================================================
CMap *CMap::Create(int nType, D3DXVECTOR3 pos)
{
	CMap *pScene2D;

	pScene2D = new CMap;
	pScene2D->Init();

	return pScene2D;
}

//=============================================================================
// CPlaneコンストラクタ
//=============================================================================
CMap::CMap(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_pos = vector3NS::ZERO;
	m_rot = vector3NS::ZERO;
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
HRESULT CMap::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CScene2D::Init(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, D3DXCOLOR color = colorNS::_WHITE);

	CMap* CMap = CMap::Create(0, 4, 4, 50.f, 50.f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), vector3NS::ZERO);
	pEnemy->BindTexture("morizo");

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	CScene2D::Update();

	CDebugProc::Print("OK\n");
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();
}