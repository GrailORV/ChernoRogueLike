//=============================================================================
//
// プレーンの処理 [plane.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"

//*********************************************************
// 3Dオブジェクトクラス
//*********************************************************
class CPlane : public CScene
{
public:
	CPlane(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLANE);
	~CPlane();

	static CPlane *Create(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);

	HRESULT Init(int nType, UINT column, UINT row, float width, float height, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR color = colorNS::_WHITE);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT MakeVertexBuffer(void);

	void BindTexture(const char* texID);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	void SetPivot(D3DXVECTOR3 pivot) { m_pivot = pivot; }
	D3DXVECTOR3 GetPivot(void) { return m_pivot; }

	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }

	void SetUV(float width, float height, float x, float y) { m_uv = D3DXVECTOR4(width, height, x, y); }
	D3DXVECTOR4 GetUV(void) { return m_uv; }

	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor(void) { return m_color; }

	void SetType(int nType) { m_nType = nType; }

protected:
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// テクスチャへのポインタ
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// 頂点バッファへのポインタ
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;

	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス

	UINT m_column, m_row;
	UINT m_numFace, m_numIndex, m_numVertex;
	D3DXVECTOR3 m_pivot;
	float m_width;
	float m_height;
	D3DXVECTOR4 m_uv;
	D3DXCOLOR m_color;

	int m_nType;							// 種類

};
