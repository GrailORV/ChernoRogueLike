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

	UINT GetWindowWidth(void) { return m_width; }
	UINT GetWindowHeight(void) { return m_height; }

	CRenderer* GetRenderer(void) { return m_pRenderer; }
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }

private:
	CRenderer* m_pRenderer;
	CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h�ւ̃|�C���^
	CInputMouse *m_pInputMouse;			// �}�E�X�ւ̃|�C���^
	CInputJoypad *m_pInputJoypad;		// �W���C�p�b�h�ւ̃|�C���^

	UINT m_width;
	UINT m_height;

#ifdef _DEBUG
	CDebugProc *m_pDebugProc;			// �f�o�b�O�����ւ̃|�C���^
#endif

};
