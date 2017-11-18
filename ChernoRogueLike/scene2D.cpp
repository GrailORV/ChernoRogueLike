//=============================================================================
//
// 2Dオブジェクトの処理 [scene2D.cpp]
// Author : 
//
//=============================================================================
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "WinApp.h"
#include "light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的変数
//*****************************************************************************
ComPtr<IDirect3DVertexBuffer9> CScene2D::m_pVtxBuff;

//=============================================================================
// 2D頂点バッファ生成
//=============================================================================
HRESULT CScene2D::MakeVertexBuffer(void)
{
	HRESULT hr{};

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// オブジェクトの頂点バッファを生成
	hr = pDevice->CreateVertexBuffer(sizeof(Vertex2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVFVertex2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		m_pVtxBuff.GetAddressOf(),				// 頂点バッファインターフェースへのポインタ
		NULL);
	if (FAILED(hr))						// NULLに設定
	{
		return hr;
	}

	{//頂点バッファの中身を埋める
		Vertex2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		for (UINT i = 0; i < NUM_VERTEX; i++)
		{
			pVtx->position.x = static_cast<float>(i % 2);
			pVtx->position.y = static_cast<float>(i / 2);
			pVtx->position.z = 0.0f;

			pVtx->color = colorNS::_WHITE;

			pVtx->uv.x = static_cast<float>(i % 2);
			pVtx->uv.y = static_cast<float>(i / 2);

			pVtx++;
		}

		// 頂点データをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// CScene2D生成
//=============================================================================
CScene2D *CScene2D::Create(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, D3DXCOLOR color)
{
	CScene2D *pScene2D;

	pScene2D = new CScene2D;
	pScene2D->Init(nType, pos, rot, width, height, color);

	return pScene2D;
}

//=============================================================================
// CScene2Dコンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPri, OBJTYPE objType) : CScene(nPri, objType),
m_pos{},
m_pivot{},
m_rot{},
m_scale{},
m_fWidth{},
m_fHeight{},
m_uv{},
m_color{},
m_nType{},
m_bLoadTex{}
{
}

//=============================================================================
// CScene2Dデストラクタ
//=============================================================================
CScene2D::~CScene2D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, D3DXCOLOR color)
{
	// タイプを設定
	m_nType = nType;

	// ポリゴン表示位置の中心座標を設定
	m_pos = pos;

	// ポリゴンの回転角を設定
	m_rot = rot;

	// ポリゴンのスケール値の変更
	m_scale = vector3NS::ONE;

	// ポリゴンのピボットの設定
	m_pivot = vector3NS::ZERO;

	// ポリゴンのサイズの設定
	m_fWidth = width;
	m_fHeight = height;

	// テクスチャ座標の設定
	m_uv = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.0f);

	// 色の設定
	m_color = color;

	// 画像読み込み判別変数の設定
	m_bLoadTex = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff.Get(), 0, sizeof(Vertex2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVFVertex2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture.Get());

	// ワールド変換行列の設定
	D3DXMATRIX world, scale, rot, translation, projection;
	D3DXMatrixScaling(&world, m_fWidth, m_fHeight, 1.0f);
	D3DXMatrixTranslation(&translation, -m_pivot.x, -m_pivot.y, -m_pivot.z);
	D3DXMatrixRotationZ(&rot, m_rot.z);
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	world *= translation;
	world = world * scale * rot;
	D3DXMatrixTranslation(&translation, m_pos.x, m_pos.y, m_pos.z);
	world *= translation;
	projection = D3DXMATRIX(
		2.0f / pManager->GetWindowWidth(), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / pManager->GetWindowHeight(), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	);
	world *= projection;
	pDevice->SetTransform(D3DTS_WORLD, &world);

	// テクスチャ座標返還行列の設定
	D3DXMATRIX texture(
		m_uv.x, 0.0f, 0.0f, 0.0f,
		0.0f, m_uv.y, 0.0f, 0.0f,
		m_uv.z, m_uv.w, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	pDevice->SetTransform(D3DTS_TEXTURE0, &texture);

	// 色設定
	D3DMATERIAL9 mat, defMat;
	pDevice->GetMaterial(&defMat);
	mat.Diffuse = m_color;
	pDevice->SetMaterial(&mat);
	
	// ライティングオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pDevice->SetMaterial(&defMat);

	// ライティングオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DXMATRIX uvDefault;
	D3DXMatrixIdentity(&uvDefault);
	pDevice->SetTransform(D3DTS_TEXTURE0, &uvDefault);
}

//=============================================================================
// サイズの設定
//=============================================================================
void CScene2D::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CScene2D::LoadTexture(const char *pTextureName)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	if (m_bLoadTex)
	{
		m_pTexture.Reset();
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		pTextureName,		// ファイルの名前
		m_pTexture.GetAddressOf());		// 読み込むメモリー
	m_bLoadTex = true;

}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	if (m_bLoadTex)
	{
		m_pTexture.Reset();
	}

	m_pTexture = pTexture;
	m_bLoadTex = false;
}
