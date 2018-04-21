//=============================================================================
//
// ボックスの処理 [box.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "main.h"
#include "scene.h"

//*********************************************************
// 3Dオブジェクトクラス
//*********************************************************
class CBox : public CScene
{
private:


public:
	CBox(int nPriority = 3, OBJTYPE objType = OBJTYPE_BOX);
	~CBox();

	static CBox *Create(int nType, Vector3 size, Vector3 pos, Vector3 rot, XColor color = colorNS::_WHITE);

	HRESULT Init(int nType, Vector3 size, Vector3 pos, Vector3 rot, XColor color = colorNS::_WHITE);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(Vector3 pos);
	Vector3 GetPosition(void) { return m_pos; }

	void SetRotation(Vector3 rot);
	Vector3 GetRotation(void) { return m_rot; }

	void SetSize(Vector3 size);
	Vector3 GetSize(void) { return m_size; }

	void SetColor(XColor color);
	XColor GetColor(void) { return m_color; }

	void SetBoxID(uint32_t id) { m_boxID = id; }
	uint32_t GetBoxID(void) { return m_boxID; }

	void SetType(int nType) { m_nType = nType; }

private:
	Vector3 m_pos;
	Vector3 m_rot;
	Vector3 m_size;
	XColor m_color;

	uint32_t m_boxID;
	int m_nType;							// 種類

};
