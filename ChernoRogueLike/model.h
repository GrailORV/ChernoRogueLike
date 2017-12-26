//=============================================================================
//
// モデルの処理 [model.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"
#include "modelManager.h"

//*********************************************************
// モデルオブジェクトクラス
//*********************************************************
class CModel : public CScene
{
public:
	CModel(int nPriority = 3, OBJTYPE objType = OBJTYPE_MODEL);
	~CModel();

	static CModel *Create(int nType, const std::string modelID, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale = vector3NS::ONE);

	HRESULT Init(int nType, const std::string modelID, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale = vector3NS::ONE);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	void SetPivot(D3DXVECTOR3 pivot) { m_pivot = pivot; }
	D3DXVECTOR3 GetPivot(void) { return m_pivot; }

	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }

	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
	D3DXVECTOR3 GetScale(void) { return m_scale; }

	void SetType(int nType) { m_nType = nType; }

private:
	MODEL_DATA* m_model;

	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス

	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_pivot;
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_scale;

	int m_nType;							// 種類

};
