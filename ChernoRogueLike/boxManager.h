//=============================================================================
//
// ボックスマネージャの処理 [boxManager.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"

class CBox;

//*********************************************************
// ボックスマネージャークラス
//*********************************************************
class CBoxManager : public CScene
{
private:
	static const UINT NUM_VERTEX = 24;
	static const UINT NUM_POLYGON = 12;
	static const UINT NUM_INDEX = 36;

	struct BoxVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		float u, v;
	};

public:
	CBoxManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_MANAGER);
	~CBoxManager();

	static CBoxManager *Create(int nType);

	HRESULT Init(int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT CreateBox(CBox* box);
	static void DestroyBox(uint32_t id);

	HRESULT MakeVertexBuffer(void);
	HRESULT MakeEffect(void);

	void BindTexture(const char* texID);

	void SetPosition(Vector3 pos) {}
	Vector3 GetPosition(void) { return vector3NS::ZERO; }

	void SetRotation(Vector3 rot) {}
	Vector3 GetRotation(void) { return vector3NS::ZERO; }

	static void SetColorByID(uint32_t id, XColor color);
	static void SetWorldByID(uint32_t id, Matrix world);

	static bool GetInitialized(void) { return m_initialized; }
	static uint32_t GetBoxNum(void) { return m_numBox; }
	static std::vector<CBox*> GetBoxList(void) { return m_boxList; }

	void SetType(int nType) { m_nType = nType; }

private:
	ComPtr<IDirect3DTexture9> m_pTexture;			// テクスチャへのポインタ
	ComPtr<IDirect3DVertexBuffer9> m_pVtxBuff;		// 頂点バッファへのポインタ
	ComPtr<IDirect3DIndexBuffer9> m_pIdxBuff;
	static ComPtr<IDirect3DVertexBuffer9> m_pColorData;
	static ComPtr<IDirect3DVertexBuffer9> m_pWorldData;

	ComPtr<ID3DXEffect> m_pEffect;
	ComPtr<IDirect3DVertexDeclaration9> m_vtxDecl;
	D3DXHANDLE m_TechHandle;
	D3DXHANDLE m_ViewHandle;
	D3DXHANDLE m_ProjHandle;
	D3DXHANDLE m_LightHandle;
	D3DXHANDLE m_AmbientLight;

	static std::vector<CBox*> m_boxList;
	static uint32_t m_numBox;
	static uint32_t m_numCreateBox;
	static bool m_initialized;

	int m_nType;							// 種類

};
