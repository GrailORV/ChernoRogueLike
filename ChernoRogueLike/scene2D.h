//=============================================================================
//
// 2Dオブジェクトの処理 [scene2D.h]
// Author : SORA ENOMOTO
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
	static const UINT NUM_VERTEX = 4;
	static const UINT NUM_POLYGON = 2;

public:
	CScene2D(int nPri = 3, OBJTYPE objType = OBJTYPE_2D);
	virtual ~CScene2D();

	static HRESULT MakeVertexBuffer(void);
	static CScene2D *Create(int nType, Vector3 pos, Vector3 rot, float width, float height, XColor color = colorNS::_WHITE);

	HRESULT Init(int nType, Vector3 pos, Vector3 rot, float width, float height, XColor color = colorNS::_WHITE);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(const char* texID);

	void SetPosition(Vector3 pos) { m_pos = pos; }
	Vector3 GetPosition(void) { return m_pos; }

	void SetPivot(Vector3 pivot) { m_pivot = pivot; }
	Vector3 GetPivot(void) { return m_pivot; }

	void SetRotation(Vector3 rot) { m_rot = rot; }
	Vector3 GetRotation(void) { return m_rot; }

	void SetScale(Vector3 scale) { m_scale = scale; }
	Vector3 GetScale(void) { return m_scale; }

	void SetUV(float width, float height, float x, float y) { m_uv = Vector4(width, height, x, y); }
	Vector4 GetUV(void) { return m_uv; }
	
	void SetColor(XColor color) { m_color = color; }
	XColor GetColor(void) { return m_color; }

	void SetSize(float fWidth, float fHeight);

	int GetType(void) { return m_nType; }

protected:
	Vector3 m_pos;						// 位置
	Vector3 m_pivot;
	Vector3 m_rot;						// 向き
	Vector3 m_scale;
	float m_fWidth;
	float m_fHeight;
	Vector4 m_uv;						// テクスチャ座標(x : width, y : height, z : xStart, w : yStart)
	XColor m_color;

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// テクスチャへのポインタ
	static ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// 頂点バッファへのポインタ

	DWORD m_texMod;

	int m_nType;							// 種類
};
