//=============================================================================
//
// スフィアの処理 [sphere.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"

//*********************************************************
// 3Dオブジェクトクラス
//*********************************************************
class CSphere : public CScene
{
private:
	static const UINT PER_DIMENSION = 4;

public:
	CSphere(int nPriority = 3, OBJTYPE objType = OBJTYPE_BOX);
	~CSphere();

	static CSphere *Create(int nType, UINT tessellation, Vector3 size, Vector3 pos, Vector3 rot, XColor color = colorNS::_WHITE);

	HRESULT Init(int nType, UINT tessellation, Vector3 size, Vector3 pos, Vector3 rot, XColor color = colorNS::_WHITE);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT MakeVertexBuffer(UINT tessellation);

	void BindTexture(const char* texID);

	void SetPosition(Vector3 pos) { m_pos = pos; }
	Vector3 GetPosition(void) { return m_pos; }

	void SetPivot(Vector3 pivot) { m_pivot = pivot; }
	Vector3 GetPivot(void) { return m_pivot; }

	void SetRotation(Vector3 rot) { m_rot = rot; }
	Vector3 GetRotation(void) { return m_rot; }

	void SetSize(Vector3 size) { m_size = size; }
	Vector3 GetSize(void) { return m_size; }

	void SetColor(XColor color) { m_color = color; }
	XColor GetColor(void) { return m_color; }

	void SetType(int nType) { m_nType = nType; }

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// テクスチャへのポインタ
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// 頂点バッファへのポインタ
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;

	Matrix m_mtxWorld;					// ワールドマトリックス

	UINT m_numVtx, m_numIdx, m_numFace;
	Vector3 m_pos;						// 位置
	Vector3 m_pivot;
	Vector3 m_rot;						// 向き
	Vector3 m_size;
	XColor m_color;

	int m_nType;							// 種類

};
