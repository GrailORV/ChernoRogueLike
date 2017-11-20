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
#include "debugproc.h"
#include "renderer.h"
#include "light.h"
#include "input.h"

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

	CRenderer* GetRenderer(void) { return m_pRenderer.Get(); }
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard.Get(); }
	CInputMouse *GetInputMouse(void) { return m_pInputMouse.Get(); }
	CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad.Get();; }
	CLight *GetLight(void) { return m_pLight.Get(); }

private:
	ComPtr<CRenderer> m_pRenderer;
	ComPtr<CInputKeyboard> m_pInputKeyboard;	// �L�[�{�[�h�ւ̃|�C���^
	ComPtr<CInputMouse> m_pInputMouse;			// �}�E�X�ւ̃|�C���^
	ComPtr<CInputJoypad> m_pInputJoypad;		// �W���C�p�b�h�ւ̃|�C���^
	ComPtr<CLight> m_pLight;

	UINT m_width;
	UINT m_height;

#ifdef _DEBUG
	ComPtr<CDebugProc> m_pDebugProc;			// �f�o�b�O�����ւ̃|�C���^
#endif

};
