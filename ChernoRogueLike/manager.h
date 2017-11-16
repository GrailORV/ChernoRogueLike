//=============================================================================
//
// �}�l�[�W���[�̏��� [manager.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

//*********************************************************
// �w�b�_�t�@�C��
//*********************************************************
#include "stdafx.h"

class CRenderer;

//*********************************************************
// �}�l�[�W���[�N���X
//*********************************************************
class CManager
{
public:
	CManager(UINT width, UINT height);
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hwnd, BOOL bWIndow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	UINT GetWindowWidth(void) { return m_width; }
	UINT GetWindowHeight(void) { return m_height; }

private:
	CRenderer* m_pRenderer;

	UINT m_width;
	UINT m_height;

};
