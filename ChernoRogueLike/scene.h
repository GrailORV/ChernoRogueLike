//=============================================================================
//
// �`��ΏۃI�u�W�F�N�g�̏��� [scene.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "stdafx.h"
#include <vector>

//*********************************************************
// �萔��`
//*********************************************************
namespace colorNS
{
	const XColor _WHITE = XColor(1.0f, 1.0f, 1.0f, 1.0f);
	const XColor _BLACK = XColor(0.0f, 0.0f, 0.0f, 1.0f);
	const XColor _RED = XColor(1.0f, 0.0f, 0.0f, 1.0f);
	const XColor _GREEN = XColor(0.0f, 1.0f, 0.0f, 1.0f);
	const XColor _BLUE = XColor(0.0f, 0.0f, 1.0f, 1.0f);
	const XColor _TRANS = XColor(0.0f, 0.0f, 0.0f, 0.0f);
	const XColor _BACKGROUND = XColor(0.0f, 0.0f, 0.5f, 1.0f);
}

namespace vector3NS
{
	const Vector3 ZERO = Vector3(0.0f, 0.0f, 0.0f);
	const Vector3 ONE = Vector3(1.0f, 1.0f, 1.0f);
	const Vector3 UP = Vector3(0.0f, 1.0f, 0.0f);
	const Vector3 RIGHT = Vector3(1.0f, 0.0f, 0.0f);
	const Vector3 FORWARD = Vector3(0.0f, 0.0f, 1.0f);
}

//*********************************************************
// �`��ΏۃI�u�W�F�N�g�N���X
//*********************************************************
class CScene
{
protected:
	static const DWORD FVFVertex2D = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	static const DWORD FVFVertex3D = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);

	typedef struct
	{
		Vector3 position;	// ���_���W
		Color color;		// ���_�J���[
		Vector2 uv;	// �e�N�X�`�����W
	} Vertex2D;

	typedef struct
	{
		Vector3 position;	// ���_���W
		Vector3 normal;		// �@��
		Color color;			// ���_�J���[
		Vector2 uv;			// �e�N�X�`�����W
	} Vertex3D;


private:
	static const UINT NUM_PRIORITY = 8;

public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_2D,					// 2D�|���S��
		OBJTYPE_PLANE,				// �v���[��
		OBJTYPE_MODEL,					// X�t�@�C��
		OBJTYPE_BILLBOARD,			// �r���{�[�h
		OBJTYPE_FADE,				// �t�F�[�h
		OBJTYPE_MAX
	} OBJTYPE;

	CScene(UINT nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();

	virtual HRESULT Init(void) { return S_OK; }
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void Release(void);

	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);

	virtual void SetPosition(Vector3 pos) = 0;
	virtual Vector3 GetPosition(void) = 0;

	virtual void SetRotation(Vector3 rot) = 0;
	virtual Vector3 GetRotation(void) = 0;

	virtual void SetMove(float x, float y, float z) {}
	virtual Vector3 GetMove(void) { return Vector3(0.0f, 0.0f, 0.0f); }

	void SetObjType(OBJTYPE objType) { m_objType = objType; }
	OBJTYPE GetObjType(void) { return m_objType; }

	UINT GetID(void) { return m_nID; }
	UINT GetPriority(void) { return m_nPriority; }

	void SetEnableUpdate(bool bEnableUpdate) { m_bEnableUpdate = bEnableUpdate; }
	bool GetEnableUpdate(void) { return m_bEnableUpdate; }
	void SetEnableDraw(bool bEnableDraw) { m_bEnableDraw = bEnableDraw; }
	bool GetEnableDraw(void) { return m_bEnableDraw; }

	static void SetEnableUpdateAll(bool bEnableUpdate);
	static void SetEnableDrawAll(bool bEnableDraw);

private:
	// ���X�g�p
	static std::vector<std::unique_ptr<CScene>> m_apScene[NUM_PRIORITY];	// ���X�g�̐擪�ʒu�ւ̃|�C���^
	static UINT m_nCreateObjNum;

	UINT m_nID;
	UINT m_nPriority;						// �v���C�I���e�B�ԍ�

	OBJTYPE m_objType;						// �I�u�W�F�N�g�^�C�v

	bool m_bEnableUpdate;
	bool m_bEnableDraw;
};
