//=============================================================================
//
// モデルの処理 [model.h]
// Author : SORA ENOMOTO
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

	static CModel *Create(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale = vector3NS::ONE);

	HRESULT Init(int nType, const std::string modelID, Vector3 pos, Vector3 rot, Vector3 scale = vector3NS::ONE);
	HRESULT Init(void) { return S_OK; }
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(Vector3 pos) { m_pos = pos; }
	Vector3 GetPosition(void) { return m_pos; }

	void SetPivot(Vector3 pivot) { m_pivot = pivot; }
	Vector3 GetPivot(void) { return m_pivot; }

	void SetRotation(Vector3 rot) { m_rot = rot; }
	Vector3 GetRotation(void) { return m_rot; }

	void SetScale(Vector3 scale) { m_scale = scale; }
	Vector3 GetScale(void) { return m_scale; }

	void SetType(int nType) { m_nType = nType; }

private:
	MODEL_DATA* m_model;

	Matrix m_mtxWorld;					// ワールドマトリックス

	Vector3 m_pos;						// 位置
	Vector3 m_pivot;
	Vector3 m_rot;						// 向き
	Vector3 m_scale;

	int m_nType;							// 種類

};
