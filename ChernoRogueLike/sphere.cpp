//=============================================================================
//
// スフィアの処理 [sphere.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include "sphere.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
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
// CSphere生成
//=============================================================================
CSphere *CSphere::Create(int nType, UINT tessellation, Vector3 size, Vector3 pos, Vector3 rot, XColor color)
{
	CSphere *pScene3D;

	pScene3D = new CSphere;
	pScene3D->Init(nType, tessellation, size, pos, rot, color);

	return pScene3D;
}

//=============================================================================
// CSphereコンストラクタ
//=============================================================================
CSphere::CSphere(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = vector3NS::ZERO;
	m_rot = vector3NS::ZERO;
	m_pivot = vector3NS::ZERO;
	m_size = vector3NS::ZERO;

	m_nType = 0;
}

//=============================================================================
// CSphereデストラクタ
//=============================================================================
CSphere::~CSphere()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSphere::Init(int nType, UINT tessellation, Vector3 size, Vector3 pos, Vector3 rot, XColor color)
{
	HRESULT hr{};

	// タイプを設定
	m_nType = nType;

	// 位置を設定
	m_pos = pos;

	// 向きを設定
	m_rot = rot;

	// 対角線の長さ・角度を設定
	m_size = size;

	// ピボット位置の設定
	m_pivot = vector3NS::ZERO;

	// 色の設定
	m_color = color;

	// 頂点確保
	MakeVertexBuffer(tessellation);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSphere::Uninit(void)
{
	// オブジェクトを破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSphere::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CSphere::Draw(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();
	CCamera *pCamera = pManager->GetCamera();

	// カメラの設定
	pCamera->SetCamera();

	Matrix mtxScale, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズ反映
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ピボット位置調整
	D3DXMatrixTranslation(&mtxTranslate, -m_pivot.x, -m_pivot.y, -m_pivot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, m_pVtxBuff.Get(), 0, sizeof(Vertex3D));

	// インデックスバッファを設定
	pDevice->SetIndices(m_pIdxBuff.Get());

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVFVertexGeo);

	// 色設定
	D3DMATERIAL9 matCurr, matDef;
	pDevice->GetMaterial(&matDef);
	matCurr = matDef;
	matCurr.Diffuse = m_color;
	pDevice->SetMaterial(&matCurr);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture.Get());

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_numIdx, 0, m_numFace);

	// マテリアル元に戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 頂点バッファとインデックスバッファの生成
//=============================================================================
HRESULT CSphere::MakeVertexBuffer(UINT tessellation)
{
	HRESULT hr{};

	m_numFace = 20 * UINT(powf(4.0f, float(tessellation)));
	m_numVtx = 10 * int(powf(float(tessellation + 1), 2.0f)) + 2;
	m_numIdx = m_numFace * 3;

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	hr = pDevice->CreateVertexBuffer(
		sizeof(Vertex3D)*m_numVtx,
		D3DUSAGE_WRITEONLY,
		FVFVertexGeo,
		D3DPOOL_MANAGED,
		m_pVtxBuff.GetAddressOf(),
		NULL
	);
	if (FAILED(hr))
	{
		return hr;
	}

	float a = 1.0f / sqrtf(5.0f);
	float b = (1.0f - a) / 2.0f;
	float c = (1.0f + a) / 2.0f;
	float d = sqrtf(b);
	float e = sqrtf(c);
	Vertex3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx->position = pVtx->normal = Vector3(0.0f, 1.0f, 0.0f);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(0.0f, a, 2.0f * a);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(e, a, b);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(d, a, -c);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(-d, a, -c);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(-e, a, b);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(d, -a, c);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(e, -a, -b);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(0.0f, -a, -2.0f * a);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(-e, -a, -b);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(-d, -a, c);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	pVtx->position = pVtx->normal = Vector3(0.0f, -1.0f, 0.0f);
	pVtx->uv = Vector2();
	pVtx->color = colorNS::_WHITE;
	pVtx++;

	m_pVtxBuff->Unlock();

	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD)*m_numIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		m_pIdxBuff.GetAddressOf(),
		NULL
	);
	if (FAILED(hr))
	{
		return hr;
	}

	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	{// 上半分
		for (UINT nCntFace = 0; nCntFace < m_numFace / 2; nCntFace++)
		{
			if (nCntFace / 5 < 1)
			{
				*pIdx = 0;
				pIdx++;
				*pIdx = nCntFace + 1;
				pIdx++;
				*pIdx = (nCntFace + 1) % 5 + 1;
				pIdx++;
			}
			else
			{
				*pIdx = nCntFace % 5 + 1;
				pIdx++;
				*pIdx = nCntFace % 5 + 6;
				pIdx++;
				*pIdx = (nCntFace + 1) % 5 + 1;
				pIdx++;
			}
		}
	}

	{// 下半分
		for (UINT nCntFace = 0; nCntFace < m_numFace / 2; nCntFace++)
		{
			if (nCntFace / 5 < 1)
			{
				*pIdx = 11;
				pIdx++;
				*pIdx = 11 - int(nCntFace + 1);
				pIdx++;
				*pIdx = 11 - int((nCntFace + 1) % 5 + 1);
				pIdx++;
			}
			else
			{
				*pIdx = 11 - int(nCntFace % 5 + 1);
				pIdx++;
				*pIdx = 11 - int(nCntFace % 5 + 6);
				pIdx++;
				*pIdx = 11 - int((nCntFace + 1) % 5 + 1);
				pIdx++;
			}
		}
	}

	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CSphere::BindTexture(const char* texID)
{
	if (!texID)
	{
		m_pTexture.Reset();
		return;
	}

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	pTextureManager->BindtextureFromString(texID, m_pTexture.GetAddressOf());
}

