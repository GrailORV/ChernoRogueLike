//=============================================================================
//
// ウインドウ処理 [WinApp.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "stdafx.h"

//メモリーリーク対応処置
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
// ウインドウクラス
//*********************************************************
class CWinApp
{
	
private:	// 定数定義
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

