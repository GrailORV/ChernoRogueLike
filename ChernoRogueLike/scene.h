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
	const D3DXCOLOR _WHITE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const D3DXCOLOR _BLACK = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR _RED = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR _GREEN = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	const D3DXCOLOR _BLUE = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	const D3DXCOLOR _TRANS = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	const D3DXCOLOR _BACKGROUND = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
}

namespace vector3NS
{
	const D3DXVECTOR3 ZERO = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 ONE = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	const D3DXVECTOR3 UP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3 RIGHT = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 FORWARD = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

//*********************************************************
// �`��ΏۃI�u�W�F�N�g�N���X
//*********************************************************
class CScene
{
private:
	static const UINT NUM_PRIORITY = 8;

public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_2D,					// 2D�|���S��
		OBJTYPE_3D,					// 3D�|���S��
		OBJTYPE_X,					// X�t�@�C��
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

	virtual void SetPosition(D3DXVECTOR3 pos) = 0;
	virtual D3DXVECTOR3 GetPosition(void) = 0;

	virtual void SetRotation(D3DXVECTOR3 rot) = 0;
	virtual D3DXVECTOR3 GetRotation(void) = 0;

	virtual void SetMove(float x, float y, float z) {}
	virtual D3DXVECTOR3 GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }

	void SetObjType(OBJTYPE objType) { m_objType = objType; }
	OBJTYPE GetObjType(void) { return m_objType; }

	void SetEnableUpdate(bool bEnableUpdate) { m_bEnableUpdate = bEnableUpdate; }
	bool GetEnableUpdate(void) { return m_bEnableUpdate; }
	void SetEnableDraw(bool bEnableDraw) { m_bEnableDraw = bEnableDraw; }
	bool GetEnableDraw(void) { return m_bEnableDraw; }

	static void SetEnableUpdateAll(bool bEnableUpdate);
	static void SetEnableDrawAll(bool bEnableDraw);

private:
	// ���X�g�p
	static std::vector<CScene*> m_apScene[NUM_PRIORITY];	// ���X�g�̐擪�ʒu�ւ̃|�C���^
	static UINT m_nCreateObjNum;

	UINT m_nID;
	UINT m_nPriority;						// �v���C�I���e�B�ԍ�

	UINT GetID(void) { return m_nID; }
	UINT GetPriority(void) { return m_nPriority; }

	OBJTYPE m_objType;						// �I�u�W�F�N�g�^�C�v

	bool m_bEnableUpdate;
	bool m_bEnableDraw;
};

