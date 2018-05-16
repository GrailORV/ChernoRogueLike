//=============================================================================
//
// ボックスの処理 [box.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include "box.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "camera.h"
#include "WinApp.h"
#include "boxManager.h"

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
// CBox生成
//=============================================================================
CBox *CBox::Create(int nType, Vector3 size, Vector3 pos, Vector3 rot, XColor color)
{
	CBox *pBox;

	pBox = new CBox;
	pBox->Init(nType, size, pos, rot, color);

	return pBox;
}

//=============================================================================
// CBoxコンストラクタ
//=============================================================================
CBox::CBox(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nType = 0;
}

//=============================================================================
// CBoxデストラクタ
//=============================================================================
CBox::~CBox()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBox::Init(int nType, Vector3 size, Vector3 pos, Vector3 rot, XColor color)
{
	HRESULT hr{};

	if (!CBoxManager::GetInitialized())
	{
		Release();
		return E_FAIL;
	}

	// タイプを設定
	m_nType = nType;

	// ボックスデータ初期化
	m_pos = pos;
	m_rot = rot;
	m_size = size;
	m_color = color;

	// ボックスマネージャに登録
	CBoxManager::CreateBox(this);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBox::Uninit(void)
{
	CBoxManager::DestroyBox(m_boxID);

	// オブジェクトを破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBox::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CBox::Draw(void)
{
}

//=============================================================================
// 位置設定
//=============================================================================
void CBox::SetPosition(Vector3 pos)
{
	m_pos = pos;

	Matrix mtxWorld, mtxScale, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// サイズ反映
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// 情報変更
	CBoxManager::SetWorldByID(m_boxID, mtxWorld);
}

//=============================================================================
// 回転設定
//=============================================================================
void CBox::SetRotation(Vector3 rot)
{
	m_rot = rot;

	Matrix mtxWorld, mtxScale, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// サイズ反映
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// 情報変更
	CBoxManager::SetWorldByID(m_boxID, mtxWorld);

}

//=============================================================================
// サイズ設定
//=============================================================================
void CBox::SetSize(Vector3 size)
{
	m_size = size;

	Matrix mtxWorld, mtxScale, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// サイズ反映
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// 情報変更
	CBoxManager::SetWorldByID(m_boxID, mtxWorld);

}

//=============================================================================
// 色設定
//=============================================================================
void CBox::SetColor(XColor color)
{
	m_color = color;

	CBoxManager::SetColorByID(m_boxID, m_color);
}
