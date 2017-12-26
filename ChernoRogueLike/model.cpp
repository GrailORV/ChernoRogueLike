//=============================================================================
//
// モデルオブジェクトの処理 [model.cpp]
// Author : 
//
//=============================================================================
#include "stdafx.h"

#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "WinApp.h"


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
// CModel生成
//=============================================================================
CModel *CModel::Create(int nType, const std::string modelID, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CModel *pScene3D;

	pScene3D = new CModel;
	pScene3D->Init(nType, modelID, pos, rot, scale);

	return pScene3D;
}

//=============================================================================
// CModelコンストラクタ
//=============================================================================
CModel::CModel(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nType = 0;
}

//=============================================================================
// CModelデストラクタ
//=============================================================================
CModel::~CModel()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(int nType, const std::string modelID, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	HRESULT hr{};

	// タイプを設定
	m_nType = nType;

	// 位置を設定
	m_pos = pos;

	// 向きを設定
	m_rot = rot;

	// スケールを設定
	m_scale = scale;

	// ピボット位置の設定
	m_pivot = vector3NS::ZERO;

	// モデル取得
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	pManager->GetMeshManager()->BindModelFromString(modelID, &m_model);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	// オブジェクトを破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();
	CCamera *pCamera = pManager->GetCamera();

	// カメラの設定
	pCamera->SetCamera();

	D3DXMATRIX mtxScale, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ピボット位置調整
	D3DXMatrixTranslation(&mtxTranslate, -m_pivot.x, -m_pivot.y, -m_pivot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアル取得
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);
	
	for (UINT i = 0; i < m_model->numMesh; i++)
	{
		for (UINT j = 0; j < m_model->mesh[i]->numMat; j++)
		{
			pDevice->SetMaterial(&m_model->mesh[i]->material[j]);
			pDevice->SetTexture(0, m_model->mesh[i]->tex[j].Get());
			m_model->mesh[i]->mesh->DrawSubset(j);
		}
	}

	// マテリアル元に戻す
	pDevice->SetMaterial(&matDef);
}

