//=============================================================================
//
// モデル管理処理 [modelManager.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "stdafx.h"

#include <vector>
#include "modelManager.h"
#include "renderer.h"
#include "WinApp.h"

using namespace fbxsdk;

//=============================================================================
// CModelManagerコンストラクタ
//=============================================================================
CModelManager::CModelManager() :m_dwRef(0)
{
}

//=============================================================================
// CModelManagerデストラクタ
//=============================================================================
CModelManager::~CModelManager()
{
	Release();
}

//=============================================================================
// 参照コピー
//=============================================================================
HRESULT CModelManager::QueryInterface(REFIID riid, void FAR* FAR* ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown))
	{
		*ppvObject = this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

//=============================================================================
// 参照カウンタインクリメント
//=============================================================================
ULONG CModelManager::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// 終了処理
//=============================================================================
ULONG CModelManager::Release(void)
{
	if (--m_dwRef == 0)
	{
		delete this;
		return 0;
	}
	return m_dwRef;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModelManager::Init(void)
{

	return S_OK;
}

//=============================================================================
// シーンごとのモデル読み込み
//=============================================================================
void CModelManager::LoadSceneMesh(int mode)
{
	std::vector<std::string> fileList;
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	std::string searchStr = "data/MODEL/*.*";

	hFind = FindFirstFile(searchStr.c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			std::string fileName = "data/MODEL/";
			fileName += fd.cFileName;
			fileList.push_back(fileName);
		}
	} while (FindNextFile(hFind, &fd));

	HRESULT hr{};

	FbxManager* fbxManager = FbxManager::Create();
	if (!fbxManager)
	{
		return;
	}


	for (UINT nCntFile = 0; nCntFile < fileList.size(); nCntFile++)
	{
		FbxImporter* importer = FbxImporter::Create(fbxManager, "Importer");
		hr = importer->Initialize(fileList[nCntFile].c_str());
		if (FAILED(hr))
		{
			importer->Destroy();
			continue;
		}

		FbxScene* scene = FbxScene::Create(fbxManager, "Scene");
		hr = importer->Import(scene);
		if (FAILED(hr))
		{
			importer->Destroy();
			scene->Destroy();
			continue;
		}

		importer->Destroy();


		std::string modelID = fileList[nCntFile];
		modelID.erase(0, 11);
		modelID.erase(modelID.find('.'));
		FbxNode* rootNode = scene->GetRootNode();
		UINT count = 0;
		if (rootNode)
		{
			for (int nCntNode = 0; nCntNode < rootNode->GetChildCount(); nCntNode++)
			{
				probeNode(rootNode->GetChild(nCntNode), modelID, &count);
			}
		}
	}

	fbxManager->Destroy();

}

//=============================================================================
// モデルの呼び出し
//=============================================================================
void CModelManager::BindModelFromString(std::string modelID, MODEL_DATA** ppModel)
{
	if (!m_modelData[modelID])
	{
		*ppModel = m_modelData["torus"].get();
		return;
	}
	*ppModel = m_modelData[modelID].get();
}


//=============================================================================
// デバイスロスト時
//=============================================================================
void CModelManager::OnLostDevice(void)
{
	for (auto iterator = m_modelData.begin(); iterator != m_modelData.end(); iterator++)
	{
		// メッシュの解放
		for (auto itMesh = iterator->second->mesh.begin(); itMesh != iterator->second->mesh.end(); itMesh++)
		{
			itMesh->get()->material.clear();
			itMesh->get()->mesh.Reset();
		}
		iterator->second->mesh.clear();

		iterator->second->texList.clear();

	};

	m_modelData.clear();
}

//=============================================================================
// ノードの探査関数
//=============================================================================
void CModelManager::probeNode(FbxNode* node, std::string name, UINT* count)
{
	if (node)
	{
		if (isMesh(node))
		{
			FbxMesh* mesh = node->GetMesh();

			if (mesh)
			{
				// モデルデータ格納領域確保
				if (*count == 0)
				{
					m_modelData[name] = nullptr;
					m_modelData[name] = std::make_unique<MODEL_DATA>();
				}
				m_modelData[name]->mesh.push_back(nullptr);
				m_modelData[name]->mesh[*count] = std::make_unique<MESH_DATA>();

				CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(CWinApp::GetHwnd(), GWLP_USERDATA));
				IDirect3DDevice9* pDevice = pManager->GetRenderer()->GetDevice();

				DWORD numFace = mesh->GetPolygonCount();
				DWORD numVertex = mesh->GetControlPointsCount();

				HRESULT hr = D3DXCreateMeshFVF(numFace, numVertex, D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, pDevice, m_modelData[name]->mesh[*count]->mesh.ReleaseAndGetAddressOf());

				// インデックス配列取得
				WORD* pIdx;
				m_modelData[name]->mesh[*count]->mesh->LockIndexBuffer(D3DLOCK_DISCARD, (LPVOID*)&pIdx);

				for (UINT nCntFace = 0; nCntFace < numFace; nCntFace++)
				{
					UINT indexNumInPolygon = mesh->GetPolygonSize(nCntFace);
					for (UINT nCntIdx = 0; nCntIdx < indexNumInPolygon; nCntIdx++)
					{
						*pIdx = static_cast<WORD>(mesh->GetPolygonVertex(nCntFace, nCntIdx));
						pIdx++;
					}
				}

				m_modelData[name]->mesh[*count]->mesh->UnlockIndexBuffer();

				// グローバルトランスフォームマトリクス取得
				FbxAMatrix globalTransform = node->EvaluateGlobalTransform();
				m_modelData[name]->mesh[*count]->offsetTransform =
					Matrix(
						float(globalTransform[0][0]), float(globalTransform[0][1]), float(globalTransform[0][2]), float(globalTransform[0][3]),
						float(globalTransform[1][0]), float(globalTransform[1][1]), float(globalTransform[1][2]), float(globalTransform[1][3]),
						float(globalTransform[2][0]), float(globalTransform[2][1]), float(globalTransform[2][2]), float(globalTransform[2][3]),
						float(globalTransform[3][0]), float(globalTransform[3][1]), float(globalTransform[3][2]), float(globalTransform[3][3])
					);

				// 頂点配列取得
				FbxVector4* v = mesh->GetControlPoints();
				std::unique_ptr<Vector3[]> vtxBuff;
				vtxBuff = std::make_unique<Vector3[]>(numVertex);
				for (UINT nCntVtx = 0; nCntVtx < numVertex; nCntVtx++)
				{
					vtxBuff[nCntVtx].x = float(v[nCntVtx][0]);
					vtxBuff[nCntVtx].y = float(v[nCntVtx][1]);
					vtxBuff[nCntVtx].z = float(v[nCntVtx][2]);
				}

				// エレメント取得
				UINT numNormal = 0;
				std::unique_ptr<Vector3[]> normalBuff;
				UINT numUV = 0;
				std::unique_ptr<Vector2[]> uvBuff;
				UINT layerNum = mesh->GetLayerCount();
				for (UINT nCntLayer = 0; nCntLayer < layerNum; nCntLayer++)
				{
					FbxLayer* layer = mesh->GetLayer(nCntLayer);
					// 法線取得
					FbxLayerElementNormal* normalElem = layer->GetNormals();
					if (normalElem)
					{
						numNormal = normalElem->GetDirectArray().GetCount();
						normalBuff = std::make_unique<Vector3[]>(numNormal);
						FbxLayerElement::EMappingMode mappingMode = normalElem->GetMappingMode();
						FbxLayerElement::EReferenceMode refMode = normalElem->GetReferenceMode();
						if (mappingMode == FbxLayerElement::eByPolygonVertex || mappingMode == FbxLayerElement::eByControlPoint)
						{
							if (refMode == FbxLayerElement::eDirect)
							{
								for (UINT nCntNml = 0; nCntNml < numNormal; nCntNml++)
								{
									normalBuff[nCntNml].x = float(normalElem->GetDirectArray().GetAt(nCntNml)[0]);
									normalBuff[nCntNml].y = float(normalElem->GetDirectArray().GetAt(nCntNml)[1]);
									normalBuff[nCntNml].z = float(normalElem->GetDirectArray().GetAt(nCntNml)[2]);
								}
							}
						}
					}
					// UV取得
					FbxLayerElementUV* uvElem = layer->GetUVs();
					if (uvElem)
					{
						numUV = uvElem->GetDirectArray().GetCount();
						uvBuff = std::make_unique<Vector2[]>(numUV);
						FbxLayerElement::EMappingMode mappingMode = uvElem->GetMappingMode();
						FbxLayerElement::EReferenceMode refMode = uvElem->GetReferenceMode();
						if (mappingMode == FbxLayerElement::eByPolygonVertex || mappingMode == FbxLayerElement::eByControlPoint)
						{
							if (refMode == FbxLayerElement::eDirect)
							{
								for (UINT nCntUV = 0; nCntUV < numUV; nCntUV++)
								{
									uvBuff[nCntUV].x = float(uvElem->GetDirectArray().GetAt(nCntUV)[0]);
									uvBuff[nCntUV].y = 1.0f - float(uvElem->GetDirectArray().GetAt(nCntUV)[1]);
								}
							}
						}
					}
				}

				// 頂点情報書き込み
				DWORD sizeFVF = D3DXGetFVFVertexSize(m_modelData[name]->mesh[*count]->mesh->GetFVF());
				BYTE* pVtx;
				m_modelData[name]->mesh[*count]->mesh->LockVertexBuffer(D3DLOCK_DISCARD, (LPVOID*)&pVtx);

				for (UINT nCntVtx = 0; nCntVtx < numVertex; nCntVtx++)
				{
					BYTE* p = pVtx;
					memcpy(p, &vtxBuff[nCntVtx], sizeof(Vector3));
					p += sizeof(Vector3);
					if (normalBuff)
					{
						memcpy(p, &normalBuff[nCntVtx], sizeof(Vector3));
					}
					p += sizeof(Vector3);
					if (uvBuff)
					{
						memcpy(p, &uvBuff[nCntVtx], sizeof(Vector2));
					}
					pVtx += sizeFVF;
				}

				m_modelData[name]->mesh[*count]->mesh->UnlockVertexBuffer();

				{// マテリアル取得
					UINT numMaterial = node->GetMaterialCount();
					m_modelData[name]->mesh[*count]->numMat = numMaterial;
					m_modelData[name]->mesh[*count]->material.resize(numMaterial);
					m_modelData[name]->mesh[*count]->texID.resize(numMaterial);
					for (UINT nCntMat = 0; nCntMat < numMaterial; nCntMat++)
					{
						FbxSurfaceMaterial* material = node->GetMaterial(nCntMat);
						// diffuse
						FbxProperty diffuse = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
						if (diffuse.IsValid())
						{
							const auto& color = diffuse.Get<FbxDouble3>();
							m_modelData[name]->mesh[*count]->material[nCntMat].Diffuse.r = float(color[0]);
							m_modelData[name]->mesh[*count]->material[nCntMat].Diffuse.g = float(color[1]);
							m_modelData[name]->mesh[*count]->material[nCntMat].Diffuse.b = float(color[2]);
							UINT layerTextureCount = diffuse.GetSrcObjectCount<FbxLayeredTexture>();
							if (layerTextureCount == 0)
							{
								UINT fileTextureCount = diffuse.GetSrcObjectCount<FbxFileTexture>();
								if (fileTextureCount > 0)
								{
									for (UINT nCntTex = 0; nCntTex < fileTextureCount; nCntTex++)
									{
										FbxFileTexture* texture = diffuse.GetSrcObject<FbxFileTexture>(nCntTex);
										if (texture)
										{
											m_modelData[name]->mesh[*count]->texID[nCntMat] = texture->GetRelativeFileName();
											if (!m_modelData[name]->texList[m_modelData[name]->mesh[*count]->texID[nCntMat]])
											{
												m_modelData[name]->texList[m_modelData[name]->mesh[*count]->texID[nCntMat]] = nullptr;
												std::string texturePath = "data/MODEL/" + m_modelData[name]->mesh[*count]->texID[nCntMat];
												D3DXCreateTextureFromFile(pDevice, texturePath.c_str(), m_modelData[name]->texList[m_modelData[name]->mesh[*count]->texID[nCntMat]].ReleaseAndGetAddressOf());
											}
										}
									}
								}
							}
						}
						// ambient
						FbxProperty ambient = material->FindProperty(FbxSurfaceMaterial::sAmbient);
						if (ambient.IsValid())
						{
							const auto& color = ambient.Get<FbxDouble3>();
							m_modelData[name]->mesh[*count]->material[nCntMat].Ambient.r = float(color[0]);
							m_modelData[name]->mesh[*count]->material[nCntMat].Ambient.g = float(color[1]);
							m_modelData[name]->mesh[*count]->material[nCntMat].Ambient.b = float(color[2]);
						}
						// emissive
						FbxProperty emissive = material->FindProperty(FbxSurfaceMaterial::sEmissive);
						if (emissive.IsValid())
						{
							const auto& color = emissive.Get<FbxDouble3>();
							m_modelData[name]->mesh[*count]->material[nCntMat].Emissive.r = float(color[0]);
							m_modelData[name]->mesh[*count]->material[nCntMat].Emissive.g = float(color[1]);
							m_modelData[name]->mesh[*count]->material[nCntMat].Emissive.b = float(color[2]);
						}
						// specular
						FbxProperty specular = material->FindProperty(FbxSurfaceMaterial::sSpecular);
						if (specular.IsValid())
						{
							const auto& color = specular.Get<FbxDouble3>();
							m_modelData[name]->mesh[*count]->material[nCntMat].Specular.r = float(color[0]);
							m_modelData[name]->mesh[*count]->material[nCntMat].Specular.g = float(color[1]);
							m_modelData[name]->mesh[*count]->material[nCntMat].Specular.b = float(color[2]);
						}
						// shininess
						FbxProperty shininess = material->FindProperty(FbxSurfaceMaterial::sShininess);
						if (shininess.IsValid())
						{
							const auto& power = shininess.Get<FbxDouble>();
							m_modelData[name]->mesh[*count]->material[nCntMat].Power = float(power);
						}
						// tranceparencyFactor
						FbxProperty transFactor = material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
						if (transFactor.IsValid())
						{
							const auto& trans = transFactor.Get<FbxDouble>();
							m_modelData[name]->mesh[*count]->material[nCntMat].Diffuse.a = float(trans);
						}
					}
				}

				*count += 1;
				m_modelData[name]->numMesh = *count;
			}
		}

		for (int i = 0; i < node->GetChildCount(); i++)
		{
			probeNode(node->GetChild(i), name, count);
		}
	}
}

//=============================================================================
// ノードがメッシュかどうか判別する
//=============================================================================
bool CModelManager::isMesh(FbxNode* node)
{
	if (node)
	{
		int attrCount = node->GetNodeAttributeCount();
		for (int i = 0; i < attrCount; i++)
		{
			FbxNodeAttribute::EType attrType = node->GetNodeAttributeByIndex(i)->GetAttributeType();

			if (attrType == FbxNodeAttribute::EType::eMesh)
			{
				return true;
			}
		}
	}
	return false;
}