//=============================================================================
//
// モデルオブジェクトの処理 [model.cpp]
// Author : SORA ENOMOTO
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
CModel *CModel::Create(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale)
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
	m_pos = Vector3(0.0f, 0.0f, 0.0f);
	m_rot = Vector3(0.0f, 0.0f, 0.0f);

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
HRESULT CModel::Init(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale)
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
	pManager->GetModelManager()->BindModelFromString(modelID, &m_model);

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

	Matrix mtxScale, mtxRot, mtxTranslate;

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

	// 現在のマテリアル取得
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);
	
	for (UINT i = 0; i < m_model->numMesh; i++)
	{
		// オフセット反映
		Matrix world;
		D3DXMatrixMultiply(&world, &m_model->mesh[i]->offsetTransform, &m_mtxWorld);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &world);

		for (UINT j = 0; j < m_model->mesh[i]->numMat; j++)
		{
			pDevice->SetMaterial(&m_model->mesh[i]->material[j]);
			pDevice->SetTexture(0, m_model->texList[m_model->mesh[i]->texID[j]].Get());
			m_model->mesh[i]->mesh->DrawSubset(0);
		}
	}

	// マテリアル元に戻す
	pDevice->SetMaterial(&matDef);
}

