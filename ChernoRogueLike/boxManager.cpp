//=============================================================================
//
// ボックスの処理 [box.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include "box.h"
#include "boxManager.h"
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
std::vector<CBox*> CBoxManager::m_boxList;
uint32_t CBoxManager::m_numBox = 0;
uint32_t CBoxManager::m_numCreateBox = 0;
bool CBoxManager::m_initialized = false;
ComPtr<IDirect3DVertexBuffer9> CBoxManager::m_pInstanceData;

//=============================================================================
// CBoxManager生成
//=============================================================================
CBoxManager *CBoxManager::Create(int nType)
{
	CBoxManager *pScene3D;

	pScene3D = new CBoxManager;
	pScene3D->Init(nType);

	return pScene3D;
}

//=============================================================================
// CBoxManagerコンストラクタ
//=============================================================================
CBoxManager::CBoxManager(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nType = 0;
}

//=============================================================================
// CBoxManagerデストラクタ
//=============================================================================
CBoxManager::~CBoxManager()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBoxManager::Init(int nType)
{
	HRESULT hr{};

	// タイプを設定
	m_nType = nType;

	//
	hr = MakeVertexBuffer();
	if (FAILED(hr))
	{
		return hr;
	}

	//
	hr = MakeEffect();
	if (FAILED(hr))
	{
		return hr;
	}

	m_initialized = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBoxManager::Uninit(void)
{
	for (; m_boxList.size() != 0;)
	{
		m_boxList[0]->Uninit();
	}

	m_initialized = false;
	m_pInstanceData.Reset();
	m_numBox = 0;
	m_numCreateBox = 0;

	// オブジェクトを破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBoxManager::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CBoxManager::Draw(void)
{
	if (!m_numBox)
	{
		return;
	}

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();
	CCamera *pCamera = pManager->GetCamera();

	// カメラの設定
	pCamera->SetCamera();

	// ビュー行列設定
	m_pEffect->SetMatrix(m_ViewHandle, &pCamera->GetViewMatrix());

	// プロジェクション行列設定
	m_pEffect->SetMatrix(m_ProjHandle, &pCamera->GetProjectionMatrix());

	// 頂点フォーマットの設定
	pDevice->SetVertexDeclaration(m_vtxDecl.Get());

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, m_pVtxBuff.Get(), 0, sizeof(BoxVertex));
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_numBox);

	// インスタンシングデータ設定
	pDevice->SetStreamSource(1, m_pInstanceData.Get(), 0, sizeof(BoxInstanceData));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

	// インデックスバッファを設定
	pDevice->SetIndices(m_pIdxBuff.Get());

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture.Get());

	m_pEffect->SetTechnique(m_TechHandle);

	UINT numPass = 0;
	m_pEffect->Begin(&numPass, 0);
	for (UINT passCnt = 0; passCnt < numPass; passCnt++)
	{
		m_pEffect->BeginPass(passCnt);
		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NUM_INDEX, 0, NUM_POLYGON);

		m_pEffect->EndPass();
	}
	m_pEffect->End();

	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);

}

//=============================================================================
// 頂点バッファとインデックスバッファの生成
//=============================================================================
HRESULT CBoxManager::MakeVertexBuffer(void)
{
	HRESULT hr{};

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	hr = pDevice->CreateVertexBuffer(
		sizeof(BoxVertex)*NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		m_pVtxBuff.ReleaseAndGetAddressOf(),
		NULL
	);
	if (FAILED(hr))
	{
		return hr;
	}

	BoxVertex *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	{// 上面
		pVtx[0].position.x = -0.5f;
		pVtx[0].position.y = 0.5f;
		pVtx[0].position.z = 0.5f;

		pVtx[0].u = 0.5f;
		pVtx[0].v = 0.375f;

		pVtx[1].position.x = 0.5f;
		pVtx[1].position.y = 0.5f;
		pVtx[1].position.z = 0.5f;

		pVtx[1].u = 0.75f;
		pVtx[1].v = 0.375f;

		pVtx[2].position.x = -0.5f;
		pVtx[2].position.y = 0.5f;
		pVtx[2].position.z = -0.5f;

		pVtx[2].u = 0.5f;
		pVtx[2].v = 0.625f;

		pVtx[3].position.x = 0.5f;
		pVtx[3].position.y = 0.5f;
		pVtx[3].position.z = -0.5f;

		pVtx[3].u = 0.75f;
		pVtx[3].v = 0.625f;

		pVtx[0].normal = pVtx[1].normal = pVtx[2].normal = pVtx[3].normal = vector3NS::UP;
	}

	{// 前面
		pVtx[4].position.x = -0.5f;
		pVtx[4].position.y = 0.5f;
		pVtx[4].position.z = -0.5f;

		pVtx[4].u = 0.5f;
		pVtx[4].v = 0.625f;

		pVtx[5].position.x = 0.5f;
		pVtx[5].position.y = 0.5f;
		pVtx[5].position.z = -0.5f;

		pVtx[5].u = 0.75f;
		pVtx[5].v = 0.625f;

		pVtx[6].position.x = -0.5f;
		pVtx[6].position.y = -0.5f;
		pVtx[6].position.z = -0.5f;

		pVtx[6].u = 0.5f;
		pVtx[6].v = 0.875f;

		pVtx[7].position.x = 0.5f;
		pVtx[7].position.y = -0.5f;
		pVtx[7].position.z = -0.5f;

		pVtx[7].u = 0.75f;
		pVtx[7].v = 0.875f;

		pVtx[4].normal = pVtx[5].normal = pVtx[6].normal = pVtx[7].normal = -vector3NS::FORWARD;
	}

	{// 右面
		pVtx[8].position.x = 0.5f;
		pVtx[8].position.y = 0.5f;
		pVtx[8].position.z = -0.5f;

		pVtx[8].u = 0.75f;
		pVtx[8].v = 0.625f;

		pVtx[9].position.x = 0.5f;
		pVtx[9].position.y = 0.5f;
		pVtx[9].position.z = 0.5f;

		pVtx[9].u = 0.75f;
		pVtx[9].v = 0.375f;

		pVtx[10].position.x = 0.5f;
		pVtx[10].position.y = -0.5f;
		pVtx[10].position.z = -0.5f;

		pVtx[10].u = 1.0f;
		pVtx[10].v = 0.625f;

		pVtx[11].position.x = 0.5f;
		pVtx[11].position.y = -0.5f;
		pVtx[11].position.z = 0.5f;

		pVtx[11].u = 1.0f;
		pVtx[11].v = 0.375f;

		pVtx[8].normal = pVtx[9].normal = pVtx[10].normal = pVtx[11].normal = vector3NS::RIGHT;
	}

	{// 左面
		pVtx[12].position.x = -0.5f;
		pVtx[12].position.y = 0.5f;
		pVtx[12].position.z = 0.5f;

		pVtx[12].u = 0.5f;
		pVtx[12].v = 0.375f;

		pVtx[13].position.x = -0.5f;
		pVtx[13].position.y = 0.5f;
		pVtx[13].position.z = -0.5f;

		pVtx[13].u = 0.5f;
		pVtx[13].v = 0.625f;

		pVtx[14].position.x = -0.5f;
		pVtx[14].position.y = -0.5f;
		pVtx[14].position.z = 0.5f;

		pVtx[14].u = 0.25f;
		pVtx[14].v = 0.375f;

		pVtx[15].position.x = -0.5f;
		pVtx[15].position.y = -0.5f;
		pVtx[15].position.z = -0.5f;

		pVtx[15].u = 0.25f;
		pVtx[15].v = 0.625f;

		pVtx[12].normal = pVtx[13].normal = pVtx[14].normal = pVtx[15].normal = -vector3NS::RIGHT;
	}

	{// 奥面
		pVtx[16].position.x = 0.5f;
		pVtx[16].position.y = 0.5f;
		pVtx[16].position.z = 0.5f;

		pVtx[16].u = 0.75f;
		pVtx[16].v = 0.375f;

		pVtx[17].position.x = -0.5f;
		pVtx[17].position.y = 0.5f;
		pVtx[17].position.z = 0.5f;

		pVtx[17].u = 0.5f;
		pVtx[17].v = 0.375f;

		pVtx[18].position.x = 0.5f;
		pVtx[18].position.y = -0.5f;
		pVtx[18].position.z = 0.5f;

		pVtx[18].u = 0.75f;
		pVtx[18].v = 0.125f;

		pVtx[19].position.x = -0.5f;
		pVtx[19].position.y = -0.5f;
		pVtx[19].position.z = 0.5f;

		pVtx[19].u = 0.5f;
		pVtx[19].v = 0.125f;

		pVtx[16].normal = pVtx[17].normal = pVtx[18].normal = pVtx[19].normal = vector3NS::FORWARD;
	}

	{// 底面
		pVtx[20].position.x = 0.5f;
		pVtx[20].position.y = -0.5f;
		pVtx[20].position.z = 0.5f;

		pVtx[20].u = 0.0f;
		pVtx[20].v = 0.375f;

		pVtx[21].position.x = -0.5f;
		pVtx[21].position.y = -0.5f;
		pVtx[21].position.z = 0.5f;

		pVtx[21].u = 0.25f;
		pVtx[21].v = 0.375f;

		pVtx[22].position.x = 0.5f;
		pVtx[22].position.y = -0.5f;
		pVtx[22].position.z = -0.5f;

		pVtx[22].u = 0.0f;
		pVtx[22].v = 0.625f;

		pVtx[23].position.x = -0.5f;
		pVtx[23].position.y = -0.5f;
		pVtx[23].position.z = -0.5f;

		pVtx[23].u = 0.25f;
		pVtx[23].v = 0.625f;

		pVtx[20].normal = pVtx[21].normal = pVtx[22].normal = pVtx[23].normal = -vector3NS::UP;
	}

	m_pVtxBuff->Unlock();

	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD)*NUM_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		m_pIdxBuff.ReleaseAndGetAddressOf(),
		NULL
	);
	if (FAILED(hr))
	{
		return hr;
	}

	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntFace = 0; nCntFace < NUM_POLYGON / 2; nCntFace++)
	{
		pIdx[nCntFace * NUM_POLYGON / 2 + 0] = nCntFace * 4 + 0;
		pIdx[nCntFace * NUM_POLYGON / 2 + 1] = nCntFace * 4 + 1;
		pIdx[nCntFace * NUM_POLYGON / 2 + 2] = nCntFace * 4 + 2;
		pIdx[nCntFace * NUM_POLYGON / 2 + 3] = nCntFace * 4 + 2;
		pIdx[nCntFace * NUM_POLYGON / 2 + 4] = nCntFace * 4 + 1;
		pIdx[nCntFace * NUM_POLYGON / 2 + 5] = nCntFace * 4 + 3;
	}

	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// シェーダーエフェクトの初期化
//=============================================================================
HRESULT CBoxManager::MakeEffect(void)
{
	HRESULT hr{};

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	D3DVERTEXELEMENT9 vtxElem[] =
	{
		{ 0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0 },
		{ 0,3 * 4,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0 },
		{ 0,6 * 4,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0 },
		{ 1,0,D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR,0 },
		{ 1,4,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1 },
		{ 1,5 * 4,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2 },
		{ 1,9 * 4,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,3 },
		{ 1,13 * 4,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,4 },
		D3DDECL_END()
	};
	hr = pDevice->CreateVertexDeclaration(vtxElem, m_vtxDecl.ReleaseAndGetAddressOf());

	ID3DXBuffer* pCompilationErrors = nullptr;

	hr = D3DXCreateEffectFromFile(
		pDevice,
		"Instancing.hlsl",
		nullptr,
		nullptr,
		D3DXSHADER_DEBUG,
		nullptr,
		m_pEffect.ReleaseAndGetAddressOf(),
		&pCompilationErrors
	);

	if (pCompilationErrors)
	{
		MessageBox(nullptr, reinterpret_cast<char*>(pCompilationErrors->GetBufferPointer()), "D3DXCreateEffectFromFile", MB_OK);
		pCompilationErrors->Release();
		return hr;
	}

	m_TechHandle = m_pEffect->GetTechniqueByName("Effect");
	m_ViewHandle = m_pEffect->GetParameterByName(nullptr, "View");
	m_ProjHandle = m_pEffect->GetParameterByName(nullptr, "Proj");

	return S_OK;
}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CBoxManager::BindTexture(const char* texID)
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

//=============================================================================
// ボックス追加
//=============================================================================
HRESULT CBoxManager::CreateBox(CBox* box)
{
	m_boxList.push_back(box);
	box->SetBoxID(m_numCreateBox);
	m_numBox++;
	m_numCreateBox++;

	BoxInstanceData* pCopyData = nullptr;
	BoxInstanceData* dataBuf = nullptr;

	if (m_pInstanceData)
	{
		pCopyData = new BoxInstanceData[m_numBox - 1];
		m_pInstanceData->Lock(0, 0, (void**)&dataBuf, 0);

		memcpy(pCopyData, dataBuf, sizeof(BoxInstanceData) * (m_numBox - 1));
		
		m_pInstanceData->Unlock();
	}

	Matrix mtxWorld, mtxScale, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// サイズ反映
	D3DXMatrixScaling(&mtxScale, box->GetSize().x, box->GetSize().y, box->GetSize().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, box->GetRotation().y, box->GetRotation().x, box->GetRotation().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, box->GetPosition().x, box->GetPosition().y, box->GetPosition().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	pDevice->CreateVertexBuffer(
		sizeof(BoxInstanceData) * m_numBox,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		m_pInstanceData.ReleaseAndGetAddressOf(),
		NULL
	);

	BoxInstanceData* pOutData;
	m_pInstanceData->Lock(0, 0, (void**)&pOutData, 0);

	if (pCopyData)
	{
		memcpy(pOutData, pCopyData, sizeof(BoxInstanceData) * (m_numBox - 1));
	}
	pOutData[m_numBox - 1].color = box->GetColor();
	pOutData[m_numBox - 1].world = mtxWorld;

	m_pInstanceData->Unlock();

	if (pCopyData)
	{
		delete[] pCopyData;
	}

	return S_OK;
}

//=============================================================================
// 消去
//=============================================================================
void CBoxManager::DestroyBox(uint32_t id)
{
	int32_t destroyIndex = -1;
	for (int32_t boxCnt = 0; boxCnt < (int32_t)m_numBox; boxCnt++)
	{
		if (m_boxList[boxCnt]->GetBoxID() == id)
		{
			destroyIndex = boxCnt;
			m_boxList.erase(m_boxList.begin() + boxCnt);
			m_numBox--;
		}
	}

	if (destroyIndex == -1)
	{
		return;
	}

	if (m_numBox == 0)
	{
		m_pInstanceData.Reset();
	}

	BoxInstanceData* pEditData;
	m_pInstanceData->Lock(0, 0, (void**)&pEditData, 0);
	m_pInstanceData->Unlock();

	for (uint32_t boxCnt = 0; boxCnt < m_numBox; boxCnt++)
	{
		if (int(boxCnt) >= destroyIndex)
		{
			pEditData[boxCnt] = pEditData[boxCnt + 1];
		}
	}

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	pDevice->CreateVertexBuffer(
		sizeof(BoxInstanceData) * m_numBox,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		m_pInstanceData.ReleaseAndGetAddressOf(),
		NULL
	);

	BoxInstanceData* pOutData;
	m_pInstanceData->Lock(0, 0, (void**)&pOutData, 0);

	memcpy(pOutData, pEditData, sizeof(BoxInstanceData) * m_numBox);

	m_pInstanceData->Unlock();
}


