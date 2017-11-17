//=============================================================================
//
// �E�C���h�E���� [WinApp.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include "stdafx.h"

//�������[���[�N�Ή����u
#ifndef MEMORY_LEAK_H
#define MEMORY_LEAK_H
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif
#endif

class CManager;

//*********************************************************
// �E�C���h�E�N���X
//*********************************************************
class CWinApp
{
	
private:	// �萔��`
	static const LPSTR CLASS_NAME;
	static const LPSTR WINDOW_NAME;

public:
	static int Run(CManager* pManager, HINSTANCE hInstance, int nCmdShow);
	static HWND GetHwnd(void) { return m_hwnd; }

protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static HWND m_hwnd;
	static BOOL m_bInit;

};

