//=============================================================================
//
// 2Dオブジェクトの処理 [scene2D.h]
// Author : 
//
//=============================================================================
#pragma once

#include "stdafx.h"
#include "scene.h"

//*********************************************************
// 2Dオブジェクトクラス
//*********************************************************
class CScene2D : public CScene
{
public:
	static const DWORD FVFVertex2D = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	static const UINT NUM_VERTEX = 4;
	static const UINT NUM_POLYGON = 2;

	typedef struct
	{
		D3DXVECTOR3 position;	// 頂点座標
		D3DCOLOR color;		// 頂点カラー
		D3DXVECTOR2 uv;	// テクスチャ座標
	} Vertex2D;

public:
	CScene2D(int nPri = 3, OBJTYPE objType = OBJTYPE_2D);
	virtual ~CScene2D();

	static HRESULT MakeVertexBuffer(void);
	static CScene2D *Create(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, D3DXCOLOR color = colorNS::_WHITE);

	HRESULT Init(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, D3DXCOLOR color = colorNS::_WHITE);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void LoadTexture(const char *pTextureName);
	void BindTexture(IDirect3DTexture9* pTex);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	void SetPivot(D3DXVECTOR3 pivot) { m_pivot = pivot; }
	D3DXVECTOR3 GetPivot(void) { return m_pivot; }

	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }

	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
	D3DXVECTOR3 GetScale(void) { return m_scale; }

	void SetUV(float width, float height, float x, float y) { m_uv = D3DXVECTOR4(width, height, x, y); }
	D3DXVECTOR4 GetUV(void) { return m_uv; }
	
	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor(void) { return m_color; }

	void SetSize(float fWidth, float fHeight);

	int GetType(void) { return m_nType; }

protected:
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_pivot;
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_scale;
	float m_fWidth;
	float m_fHeight;
	D3DXVECTOR4 m_uv;						// テクスチャ座標(x : width, y : height, z : xStart, w : yStart)
	D3DXCOLOR m_color;

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// テクスチャへのポインタ
	static ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// 頂点バッファへのポインタ

	int m_nType;							// 種類

	bool m_bLoadTex;						// テクスチャが読み込まれたどうか
};
