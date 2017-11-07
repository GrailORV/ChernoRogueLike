//=============================================================================
//
// ƒƒCƒ“ˆ— [main.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "WinApp.h"

//=============================================================================
// ƒƒCƒ“ŠÖ”
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{

	CManager* pManager = new CManager(960, 540);
	return CWinApp::Run(pManager, hInstance, nCmdShow);
}