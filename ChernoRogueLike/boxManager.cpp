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
ComPtr<IDirect3DVertexBuffer9> CBoxManager::m_pColorData;
ComPtr<IDirect3DVertexBuffer9> CBoxManager::m_pWorldData;

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
	m_pColorData.Reset();
	m_pWorldData.Reset();
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
	CLight* pLight = pManager->GetLight();

	// カメラの設定
	pCamera->SetCamera();

	// ビュー行列設定
	m_pEffect->SetMatrix(m_ViewHandle, &pCamera->GetViewMatrix());

	// プロジェクション行列設定
	m_pEffect->SetMatrix(m_ProjHandle, &pCamera->GetProjectionMatrix());

	// ライトの設定
	m_pEffect->SetValue(m_LightHandle, pLight->GetLight(), sizeof(D3DLIGHT9)*pLight->NUM_LIGHT);
	m_pEffect->SetValue(m_AmbientLight, &pLight->GetAmbientLight(), sizeof(XColor));

	// 頂点フォーマットの設定
	pDevice->SetVertexDeclaration(m_vtxDecl.Get());

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, m_pVtxBuff.Get(), 0, sizeof(BoxVertex));
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_numBox);

	// 色情報設定
	pDevice->SetStreamSource(1, m_pColorData.Get(), 0, sizeof(Color));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

	// ワールドマトリクス情報設定
	pDevice->SetStreamSource(2, m_pWorldData.Get(), 0, sizeof(Matrix));
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

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
	pDevice->SetStreamSourceFreq(2, 1);

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
		{ 2,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1 },
		{ 2,4 * 4,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2 },
		{ 2,8 * 4,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,3 },
		{ 2,12 * 4,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,4 },
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
	m_LightHandle = m_pEffect->GetParameterByName(nullptr, "g_Light");
	m_AmbientLight = m_pEffect->GetParameterByName(nullptr, "g_LightAmbient");

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

	Color* pCopyColorData = nullptr;
	Matrix* pCopyWorldData = nullptr;

	if (m_pColorData && m_pWorldData)
	{
		// 既定の色情報コピー
		Color* dataColorBuf = nullptr;

		pCopyColorData = new Color[m_numBox - 1];
		m_pColorData->Lock(0, 0, (void**)&dataColorBuf, 0);

		memcpy(pCopyColorData, dataColorBuf, sizeof(Color) * (m_numBox - 1));
		
		m_pColorData->Unlock();

		// 既定のワールドマトリクス情報コピー
		Matrix* dataWorldBuf = nullptr;

		pCopyWorldData = new Matrix[m_numBox - 1];
		m_pWorldData->Lock(0, 0, (void**)&dataWorldBuf, 0);

		memcpy(pCopyWorldData, dataWorldBuf, sizeof(Matrix) * (m_numBox - 1));

		m_pWorldData->Unlock();
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

	// 色情報設定
	pDevice->CreateVertexBuffer(
		sizeof(Color) * m_numBox,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		m_pColorData.ReleaseAndGetAddressOf(),
		NULL
	);

	Color* pOutColorData;
	m_pColorData->Lock(0, 0, (void**)&pOutColorData, 0);

	if (pCopyColorData)
	{
		memcpy(pOutColorData, pCopyColorData, sizeof(Color) * (m_numBox - 1));
	}
	pOutColorData[m_numBox - 1] = box->GetColor();

	m_pColorData->Unlock();

	if (pCopyColorData)
	{
		delete[] pCopyColorData;
	}

	// ワールドマトリクス情報設定
	pDevice->CreateVertexBuffer(
		sizeof(Matrix) * m_numBox,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		m_pWorldData.ReleaseAndGetAddressOf(),
		NULL
	);

	Matrix* pOutWorldData;
	m_pWorldData->Lock(0, 0, (void**)&pOutWorldData, 0);

	if (pCopyWorldData)
	{
		memcpy(pOutWorldData, pCopyWorldData, sizeof(Matrix) * (m_numBox - 1));
	}
	pOutWorldData[m_numBox - 1] = mtxWorld;

	m_pWorldData->Unlock();

	if (pCopyWorldData)
	{
		delete[] pCopyWorldData;
	}

	return S_OK;
}

//=============================================================================
// 消去
//=============================================================================
void CBoxManager::DestroyBox(uint32_t id)
{
	if (m_numBox == 0)
	{
		return;
	}

	int32_t destroyIndex = -1;
	for (int32_t boxCnt = 0; boxCnt < (int32_t)m_numBox; boxCnt++)
	{
		if (m_boxList[boxCnt]->GetBoxID() == id)
		{
			destroyIndex = boxCnt;
			m_boxList.erase(m_boxList.begin() + boxCnt);
			m_numBox--;
			break;
		}
	}

	if (destroyIndex == -1)
	{
		return;
	}

	if (m_numBox == 0)
	{
		m_pColorData.Reset();
		m_pWorldData.Reset();
		return;
	}

	// 色情報編集
	Color* pEditColorData = nullptr;
	pEditColorData = new Color[m_numBox + 1];
	Color* pColorDataBuf = nullptr;
	m_pColorData->Lock(0, 0, (void**)&pColorDataBuf, 0);
	memcpy(pEditColorData, pColorDataBuf, sizeof(Color) * (m_numBox + 1));
	m_pColorData->Unlock();

	for (uint32_t boxCnt = 0; boxCnt < m_numBox; boxCnt++)
	{
		if (int(boxCnt) >= destroyIndex)
		{
			pEditColorData[boxCnt] = pEditColorData[boxCnt + 1];
		}
	}

	// ワールドマトリクス情報情報編集
	Matrix* pEditWorldData = nullptr;
	pEditWorldData = new Matrix[m_numBox + 1];
	Matrix* pWorldDataBuf = nullptr;
	m_pWorldData->Lock(0, 0, (void**)&pWorldDataBuf, 0);
	memcpy(pEditWorldData, pWorldDataBuf, sizeof(Matrix) * (m_numBox + 1));
	m_pWorldData->Unlock();

	for (uint32_t boxCnt = 0; boxCnt < m_numBox; boxCnt++)
	{
		if (int(boxCnt) >= destroyIndex)
		{
			pEditWorldData[boxCnt] = pEditWorldData[boxCnt + 1];
		}
	}

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// 色情報設定
	pDevice->CreateVertexBuffer(
		sizeof(Color) * m_numBox,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		m_pColorData.ReleaseAndGetAddressOf(),
		NULL
	);

	Color* pOutColorData;
	m_pColorData->Lock(0, 0, (void**)&pOutColorData, 0);

	memcpy(pOutColorData, pEditColorData, sizeof(Color) * m_numBox);

	m_pColorData->Unlock();

	// ワールドマトリクス情報設定
	pDevice->CreateVertexBuffer(
		sizeof(Matrix) * m_numBox,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		m_pWorldData.ReleaseAndGetAddressOf(),
		NULL
	);

	Matrix* pOutWorldData;
	m_pWorldData->Lock(0, 0, (void**)&pOutWorldData, 0);

	memcpy(pOutWorldData, pEditWorldData, sizeof(Matrix) * m_numBox);

	m_pWorldData->Unlock();
}

//=============================================================================
// IDから色情報変更
//=============================================================================
void CBoxManager::SetColorByID(uint32_t id, XColor color)
{
	if (m_numBox == 0)
	{
		return;
	}

	int32_t editIndex = -1;
	for (int32_t boxCnt = 0; boxCnt < (int32_t)m_numBox; boxCnt++)
	{
		if (m_boxList[boxCnt]->GetBoxID() == id)
		{
			editIndex = boxCnt;
			break;
		}
	}

	if (editIndex == -1)
	{
		return;
	}

	// 色情報編集
	Color* pEditColorData = nullptr;
	pEditColorData = new Color[m_numBox];
	Color* pColorDataBuf = nullptr;
	m_pColorData->Lock(0, 0, (void**)&pColorDataBuf, 0);
	memcpy(pEditColorData, pColorDataBuf, sizeof(Color) * m_numBox);
	m_pColorData->Unlock();

	pEditColorData[editIndex] = color;

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// 色情報設定
	pDevice->CreateVertexBuffer(
		sizeof(Color) * m_numBox,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		m_pColorData.ReleaseAndGetAddressOf(),
		NULL
	);

	Color* pOutColorData;
	m_pColorData->Lock(0, 0, (void**)&pOutColorData, 0);

	memcpy(pOutColorData, pEditColorData, sizeof(Color) * m_numBox);

	m_pColorData->Unlock();

}

//=============================================================================
// IDからワールドマトリクス情報変更
//=============================================================================
void CBoxManager::SetWorldByID(uint32_t id, Matrix world)
{
	if (m_numBox == 0)
	{
		return;
	}

	int32_t editIndex = -1;
	for (int32_t boxCnt = 0; boxCnt < (int32_t)m_numBox; boxCnt++)
	{
		if (m_boxList[boxCnt]->GetBoxID() == id)
		{
			editIndex = boxCnt;
			break;
		}
	}

	if (editIndex == -1)
	{
		return;
	}

	// ワールドマトリクス情報情報編集
	Matrix* pEditWorldData = nullptr;
	pEditWorldData = new Matrix[m_numBox];
	Matrix* pWorldDataBuf = nullptr;
	m_pWorldData->Lock(0, 0, (void**)&pWorldDataBuf, 0);
	memcpy(pEditWorldData, pWorldDataBuf, sizeof(Matrix) * m_numBox);
	m_pWorldData->Unlock();

	pEditWorldData[editIndex] = world;

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// ワールドマトリクス情報設定
	pDevice->CreateVertexBuffer(
		sizeof(Matrix) * m_numBox,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		m_pWorldData.ReleaseAndGetAddressOf(),
		NULL
	);

	Matrix* pOutWorldData;
	m_pWorldData->Lock(0, 0, (void**)&pOutWorldData, 0);

	memcpy(pOutWorldData, pEditWorldData, sizeof(Matrix) * m_numBox);

	m_pWorldData->Unlock();
}



