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
class CInputKeyboard;
class CInputMouse;
class CInputJoypad;
class CLight;

#ifdef _DEBUG
class CDebugProc;
#endif

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

	void SetWindowWidth(UINT width) { m_width = width; }
	UINT GetWindowWidth(void) { return m_width; }
	void SetWindowHeight(UINT height) { m_height = height; }
	UINT GetWindowHeight(void) { return m_height; }

	CRenderer* GetRenderer(void) { return m_pRenderer; }
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
	CLight *GetLight(void) { return m_pLight; }

private:
	CRenderer* m_pRenderer;
	CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h�ւ̃|�C���^
	CInputMouse *m_pInputMouse;			// �}�E�X�ւ̃|�C���^
	CInputJoypad *m_pInputJoypad;		// �W���C�p�b�h�ւ̃|�C���^
	CLight * m_pLight;

	UINT m_width;
	UINT m_height;

#ifdef _DEBUG
	CDebugProc *m_pDebugProc;			// �f�o�b�O�����ւ̃|�C���^
#endif

};
