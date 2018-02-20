//=============================================================================
//
// 3Dオブジェクトの処理 [scene3D.cpp]
// Author : 
//
//=============================================================================
#include "stdafx.h"

#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "camera.h"
#include "WinApp.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE		10.f

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// 静的変数
//*****************************************************************************
const int CPlayer::FLAME_MAX = 5;		// キー入力の実行までの待機時間 : 0.6秒

//=============================================================================
// CPlane生成
//=============================================================================
CPlayer *CPlayer::Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	CPlayer *pScene3D;

	pScene3D = new CPlayer;
	pScene3D->Init(nType, column, row, width, height, pos, rot, color);

	return pScene3D;
}

//=============================================================================
// CPlaneコンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nType = 0;
}

//=============================================================================
// CPlaneデストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color)
{
	HRESULT hr{};


	m_iTurn = 0;
	m_iCount = 0;
	m_frameCount = 0;
	m_bMove = false;
	m_inputEnable = true;
	m_inputSecondEnable = true;
	m_moveBuff = Vector3(0.0f, 0.0f, 0.0f);

	// タイプを設定
	m_nType = nType;

	// 分割数の設定
	m_column = column;
	m_row = row;
	m_numFace = column * row * 2 + (column - 1) * 4;
	m_numIndex = m_numFace + 2;
	m_numVertex = (column + 1) * (row + 1);

	// 位置を設定
	m_pos = pos + vector3NS::UP;

	// 向きを設定
	m_rot = rot;

	// 対角線の長さ・角度を設定
	m_width = width;
	m_height = height;

	// ピボット位置の設定
	m_pivot = vector3NS::ZERO;

	// テクスチャ座標の設定
	m_uv = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 0.0f);

	// 色の設定
	m_color = color;

	m_move = vector3NS::ZERO;

	m_rotDest = vector3NS::ZERO;

	// オブジェクトの頂点バッファを生成
	hr = MakeVertexBuffer();
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// オブジェクトを破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	CCamera *pCamera = pManager->GetCamera();
	CInputKeyboard *pInputKeyboard;
	// キーボード取得
	pInputKeyboard = pManager->GetInputKeyboard();

	if (m_inputEnable)
	{
		if (pInputKeyboard->GetKeyPress(DIK_UP))
		{
			m_moveBuff.z += MOVE;
			m_inputEnable = false;
		}
		if (pInputKeyboard->GetKeyPress(DIK_DOWN))
		{
			m_moveBuff.z += -MOVE;
			m_inputEnable = false;
		}
		if (pInputKeyboard->GetKeyPress(DIK_LEFT))
		{
			m_moveBuff.x += -MOVE;
			m_inputEnable = false;
		}
		if (pInputKeyboard->GetKeyPress(DIK_RIGHT))
		{
			m_moveBuff.x += MOVE;
			m_inputEnable = false;
		}
	}
	else if (!m_bMove)
	{
		if (m_inputSecondEnable)
		{
			if (pInputKeyboard->GetKeyPress(DIK_UP))
			{
				m_moveBuff.z += MOVE;
				m_inputSecondEnable = false;
			}
			if (pInputKeyboard->GetKeyPress(DIK_DOWN))
			{
				m_moveBuff.z += -MOVE;
				m_inputSecondEnable = false;
			}
			if (pInputKeyboard->GetKeyPress(DIK_LEFT))
			{
				m_moveBuff.x += -MOVE;
				m_inputSecondEnable = false;
			}
			if (pInputKeyboard->GetKeyPress(DIK_RIGHT))
			{
				m_moveBuff.x += MOVE;
				m_inputSecondEnable = false;
			}
		}
		m_frameCount++;
		if (m_frameCount > FLAME_MAX)
		{
			m_frameCount = 0;
			if (D3DXVec3Length(&m_moveBuff) <= 0.0f)
			{
				m_inputEnable = true;
				m_inputSecondEnable = true;
			}
			else
			{
				m_bMove = true;
				m_iTurn++;
				m_move += m_moveBuff;
				m_moveBuff = Vector3(0.0f, 0.0f, 0.0f);
			}
		}
	}


	//if (pInputKeyboard->GetKeyTrigger(DIK_UP)) {
	//	m_bMove = true;
	//	// 左上
	//	if (pInputKeyboard->GetKeyTrigger(DIK_LEFT) && m_iCount >= FLAME_MAX) {
	//		m_move.x += sinf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*0.75f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//	// 右上
	//	else if (pInputKeyboard->GetKeyTrigger(DIK_RIGHT) && m_iCount >= FLAME_MAX) {
	//		m_move.x += sinf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*-0.75f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//	// 上
	//	else if(m_iCount >= FLAME_MAX){
	//		m_move.x -= sinf(pCamera->GetRot().y + D3DX_PI)* MOVE;
	//		m_move.z -= cosf(pCamera->GetRot().y + D3DX_PI)* MOVE;
	//		m_rotDest.y = D3DX_PI*1.f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//}


	//else if (pInputKeyboard->GetKeyTrigger(DIK_DOWN)) {
	//	m_bMove = true;
	//	// 左下
	//	if (pInputKeyboard->GetKeyTrigger(DIK_LEFT) && m_iCount >= FLAME_MAX) {
	//		m_move.x += sinf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*0.5f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//	// 右下
	//	else if (pInputKeyboard->GetKeyTrigger(DIK_RIGHT) && m_iCount >= FLAME_MAX) {
	//		m_move.x += sinf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*-0.5f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//	// 後ろ
	//	else if(m_iCount >= FLAME_MAX){
	//		m_move.x += sinf(pCamera->GetRot().y - D3DX_PI)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y - D3DX_PI)* MOVE;
	//		m_rotDest.y = D3DX_PI*0.f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//}


	//else if (pInputKeyboard->GetKeyTrigger(DIK_LEFT)) {
	//	m_bMove = true;
	//	// 左上
	//	if (pInputKeyboard->GetKeyTrigger(DIK_UP) && m_iCount >= FLAME_MAX) {
	//		m_move.x += sinf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*0.75f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//	// 右上
	//	else if (pInputKeyboard->GetKeyTrigger(DIK_DOWN) && m_iCount >= FLAME_MAX) {
	//		m_move.x += sinf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*-0.25f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//	//左
	//	else if(m_iCount >= FLAME_MAX){
	//		m_move.x += sinf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*0.5f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//}



	//else if (pInputKeyboard->GetKeyTrigger(DIK_RIGHT)) {
	//	m_bMove = true;
	//	// 左上
	//	if (pInputKeyboard->GetKeyTrigger(DIK_UP) && m_iCount >= FLAME_MAX) {
	//		m_move.x += sinf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y - D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*0.75f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//	// 右上
	//	else if (pInputKeyboard->GetKeyTrigger(DIK_DOWN) && m_iCount >= FLAME_MAX) {
	//		m_move.x += sinf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*-0.25f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//	//右
	//	else if(m_iCount >= FLAME_MAX){
	//		m_move.x += sinf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_move.z += cosf(pCamera->GetRot().y + D3DX_PI*0.5f)* MOVE;
	//		m_rotDest.y = D3DX_PI*-0.5f + pCamera->GetRot().y;
	//		m_iTurn++;
	//	}
	//}

	if (m_bMove)
	{
		if (D3DXVec3Length(&m_move) <= 0.0f)
		{
			m_bMove = false;
			m_inputEnable = true;
			m_inputSecondEnable = true;
		}
	}

	// 回転方向補正
	float fDiffRotY = m_rotDest.y - m_rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 回転
	m_rot.y += fDiffRotY * 0.2f;
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_pos += m_move;
	m_move *= 0.8f;
	if (D3DXVec3Length(&m_move) < 0.01f)
	{
		m_move = Vector3(0.0f, 0.0f, 0.0f);
	}

	CDebugProc::Print("ターン数 : %d\n", m_iTurn);
	CDebugProc::Print("フレーム数 : %d\n", m_iCount);

}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
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
	pDevice->SetFVF(FVFVertex3D);

	// テクスチャ座標返還行列の設定
	D3DXMATRIX texture(
		m_uv.x, 0.0f, 0.0f, 0.0f,
		0.0f, m_uv.y, 0.0f, 0.0f,
		m_uv.z, m_uv.w, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	pDevice->SetTransform(D3DTS_TEXTURE0, &texture);

	// 色設定
	D3DMATERIAL9 matCurr, matDef;
	pDevice->GetMaterial(&matDef);
	matCurr = matDef;
	matCurr.Diffuse = m_color;
	pDevice->SetMaterial(&matCurr);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture.Get());

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_numIndex, 0, m_numFace);

	// マテリアル元に戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 頂点バッファとインデックスバッファの生成
//=============================================================================
HRESULT CPlayer::MakeVertexBuffer(void)
{
	HRESULT hr{};

	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
	IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

	hr = pDevice->CreateVertexBuffer(
		sizeof(Vertex3D)*m_numVertex,
		D3DUSAGE_WRITEONLY,
		FVFVertex3D,
		D3DPOOL_MANAGED,
		m_pVtxBuff.GetAddressOf(),
		NULL
	);
	if (FAILED(hr))
	{
		return hr;
	}

	Vertex3D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (UINT row = 0; row <= m_row; row++)
	{
		for (UINT column = 0; column <= m_column; column++)
		{
			pVtx->position.x = (column / (float)m_column - 0.5f) * m_width;
			pVtx->position.y = 0.0f;
			pVtx->position.z = (0.5f - row / (float)m_row) * m_height;

			pVtx->normal = vector3NS::UP;

			pVtx->color = 0xffffffff;

			pVtx->uv.x = column / (float)m_column;
			pVtx->uv.y = row / (float)m_row;

			pVtx++;
		}
	}
	m_pVtxBuff->Unlock();

	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD)*m_numIndex,
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
	int nDivide = (m_column + 2) * 2;
	int nVtxNum;
	for (UINT nCntVtx = 0; nCntVtx < m_numIndex; nCntVtx++)
	{
		int nVtxBuff = nCntVtx%nDivide;
		// 縮退分
		if (nVtxBuff == nDivide - 2)
		{
			// 前の頂点番号流用
		}
		else if (nVtxBuff == nDivide - 1)
		{
			nVtxNum = ((nVtxBuff % 2) + (nCntVtx / nDivide))*(m_column + 1) + (nVtxBuff / 2);
		}
		// 通常の頂点
		else
		{
			nVtxNum = (((nVtxBuff + 1) % 2) + (nCntVtx / nDivide))*(m_column + 1) + (nVtxBuff / 2);
		}
		*pIdx = (WORD)nVtxNum;
		pIdx++;
	}

	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CPlayer::BindTexture(const char* texID)
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

