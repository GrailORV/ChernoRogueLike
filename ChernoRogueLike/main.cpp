//=============================================================================
//
// メイン処理 [main.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "WinApp.h"

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{

	CManager* pManager = new CManager(960, 540);
	return CWinApp::Run(pManager, hInstance, nCmdShow);
}