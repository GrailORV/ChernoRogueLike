//=============================================================================
//
// ���C������ [main.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "WinApp.h"

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	ComPtr<CManager> pManager = new CManager(1280, 720);
	return CWinApp::Run(pManager.Get(), hInstance, nCmdShow);
}