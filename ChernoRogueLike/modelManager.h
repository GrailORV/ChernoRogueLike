//=============================================================================
//
// モデル管理処理 [modelManager.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "stdafx.h"

#include <fbxsdk.h>
#include <vector>
#include <map>

struct MESH_DATA
{
	ComPtr<ID3DXMesh> mesh;
	std::vector<D3DMATERIAL9> material;
	std::vector<std::string> texID;
	Matrix offsetTransform;
	DWORD numMat;
};

struct MODEL_DATA
{
	std::vector<std::unique_ptr<MESH_DATA>> mesh;
	std::map<std::string, ComPtr<IDirect3DTexture9>> texList;
	UINT numMesh;
};

//*********************************************************
// モデル管理クラス
//*********************************************************
class CModelManager :public IUnknown
{
public:
	CModelManager();
	~CModelManager();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(void);

	void LoadSceneMesh(int mode);
	void BindModelFromString(std::string modelID, MODEL_DATA** ppModel);

	void OnLostDevice(void);

private:
	void probeNode(FbxNode* node, std::string name, UINT* count);
	bool isMesh(FbxNode* node);

	DWORD m_dwRef;

	std::map<std::string, std::unique_ptr<MODEL_DATA>> m_modelData;

};