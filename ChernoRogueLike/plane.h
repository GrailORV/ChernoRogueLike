//=============================================================================
//
// プレーンの処理 [plane.h]
// Author : SORA ENOMOTO
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

	static CPlane *Create(int nType, UINT column, UINT row, float width, float height, Vector3 pos, Vector3 rot, XColor color = colorNS::_WHITE);

	HRESULT Init(int nType, UINT column, UINT row, float width, float height, Vector3 pos, Vector3 rot, XColor color = colorNS::_WHITE);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT MakeVertexBuffer(void);

	void BindTexture(const char* texID);

	void SetPosition(Vector3 pos) { m_pos = pos; }
	Vector3 GetPosition(void) { return m_pos; }

	void SetPivot(Vector3 pivot) { m_pivot = pivot; }
	Vector3 GetPivot(void) { return m_pivot; }

	void SetRotation(Vector3 rot) { m_rot = rot; }
	Vector3 GetRotation(void) { return m_rot; }

	void SetUV(float width, float height, float x, float y) { m_uv = Vector4(width, height, x, y); }
	Vector4 GetUV(void) { return m_uv; }

	void SetColor(XColor color) { m_color = color; }
	XColor GetColor(void) { return m_color; }

	void SetType(int nType) { m_nType = nType; }

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// テクスチャへのポインタ
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// 頂点バッファへのポインタ
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;

	Matrix m_mtxWorld;					// ワールドマトリックス

	UINT m_column, m_row;
	UINT m_numFace, m_numIndex, m_numVertex;
	Vector3 m_pos;						// 位置
	Vector3 m_pivot;
	Vector3 m_rot;						// 向き
	float m_width;
	float m_height;
	Vector4 m_uv;
	XColor m_color;

	int m_nType;							// 種類

};
