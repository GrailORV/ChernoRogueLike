//=============================================================================
//
// 描画対象オブジェクトの処理 [scene.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene.h"

using namespace std;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的変数
//*****************************************************************************
// リスト用
vector<CScene*> CScene::m_apScene[NUM_PRIORITY];

//=============================================================================
// リスト内オブジェ更新
//=============================================================================
void CScene::UpdateAll(void)
{
	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			m_apScene[nCntPri][nCntScene]->m_nID = nCntScene;
			if (!m_apScene[nCntPri][nCntScene]->m_bEnableUpdate)
			{
				continue;
			}
			m_apScene[nCntPri][nCntScene]->Update();
		}
	}
}

//=============================================================================
// リスト内オブジェ描画
//=============================================================================
void CScene::DrawAll(void)
{
	//CCamera *pCamera = CManager::GetCamera();

	//// カメラの設定
	//pCamera->SetCamera();

	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			if (!m_apScene[nCntPri][nCntScene]->m_bEnableDraw)
			{
				continue;
			}
			m_apScene[nCntPri][nCntScene]->Draw();
		}
	}
}

//=============================================================================
// オブジェクトを破棄
//=============================================================================
void CScene::Release(void)
{
	m_apScene[m_nPriority].erase(m_apScene[m_nPriority].begin() + m_nID);
}

//=============================================================================
// オブジェクト全体を破棄
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			m_apScene[nCntPri][nCntScene]->Uninit();
		}
	}
}

//=============================================================================
// CSceneコンストラクタ
//=============================================================================
CScene::CScene(UINT nPriority, OBJTYPE objType) :
	m_nPriority(nPriority),
	m_objType(objType),
	m_bEnableUpdate(true),
	m_bEnableDraw(true)
{
	m_apScene[nPriority].push_back(this);
}

//=============================================================================
// CSceneデストラクタ
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// 全てのオブジェクトの更新処理のON/OFF
//=============================================================================
void CScene::SetEnableUpdateAll(bool bEnableUpdate)
{
	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			// オブジェクトの終了処理
			m_apScene[nCntPri][nCntScene]->m_bEnableUpdate = bEnableUpdate;
		}
	}
}

//=============================================================================
// 全てのオブジェクトの描画処理のON/OFF
//=============================================================================
void CScene::SetEnableDrawAll(bool bEnableDraw)
{
	for (UINT nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (UINT nCntScene = 0; nCntScene < m_apScene[nCntPri].size(); nCntScene++)
		{
			// オブジェクトの終了処理
			m_apScene[nCntPri][nCntScene]->m_bEnableDraw = bEnableDraw;
		}
	}
}



