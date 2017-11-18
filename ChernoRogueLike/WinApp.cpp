//=============================================================================
//
// ウインドウ処理 [WinApp.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "WinApp.h"
#include "manager.h"

//*****************************************************************************
// 静的変数
//*****************************************************************************
HWND CWinApp::m_hwnd = nullptr;
BOOL CWinApp::m_bInit = FALSE;
const LPSTR CWinApp::CLASS_NAME = "DX9Game";
const LPSTR CWinApp::WINDOW_NAME = "ChernoLogueLike";

//*********************************************************
// ウインドウクラス
//*********************************************************
int CWinApp::Run(CManager* pManager, HINSTANCE hInstance, int nCmdShow)
{
#ifdef _DEBUG
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// ウインドウクラスの初期化
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_CLASSDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wcex);

	// ウインドウサイズの初期化
	RECT wndRect = { 0,0,static_cast<LONG>(pManager->GetWindowWidth()),static_cast<LONG>(pManager->GetWindowHeight()) };
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, false);

	// ウインドウの初期化
	m_hwnd = CreateWindow(
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(wndRect.right - wndRect.left),
		(wndRect.bottom - wndRect.top),
		NULL,
		NULL,
		hInstance,
		pManager
	);

	BOOL bWindow{};
#ifdef _DEBUG
	bWindow = TRUE;
#else
	int result;
	result = MessageBox(NULL, "フルスクリーンにしますか？", WINDOW_NAME, MB_YESNOCANCEL | MB_ICONQUESTION);
	while (1)
	{
		if (result == IDYES)
		{
			bWindow = FALSE;
			break;
		}
		else if (result == IDNO)
		{
			bWindow = TRUE;
			break;
		}
		else if (result == IDCANCEL)
		{
			// ウインドウクラスの登録の解除
			UnregisterClass(CLASS_NAME, wcex.hInstance);
			// マネージャの破棄
			SafeDelete(pManager, &CManager::Uninit);
			return 0;
		}
	}
#endif

	// マネージャの初期化
	pManager->Init(hInstance, m_hwnd, bWindow);
	m_bInit = TRUE;

	// 分解能の設定
	timeBeginPeriod(1);

	// ウインドウの表示
	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);

	// メッセージループ
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// メッセージの翻訳とディスパッチ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// ウインドウクラスの登録の解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	// マネージャの破棄
	SafeDelete(pManager, &CManager::Uninit);

	return static_cast<int>(msg.wParam);
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK CWinApp::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// フレームカウント用変数
	static DWORD dwExecLastTime;
	static DWORD dwFPSLastTime;
	static DWORD dwCurrentTime;
	static DWORD dwFrameCount;
#ifdef _DEBUG
	static int nCountFPS = 0;		// FPSカウンタ
#endif

	// マネージャー取得
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_CREATE:
	{
		// フレームカウント初期化
		dwExecLastTime =
			dwFPSLastTime = timeGetTime();
		dwCurrentTime =
			dwFrameCount = 0;

		// マネージャのパス保存
		LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));

	}
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		return 0;

	case WM_PAINT:
		if (m_bInit)
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// マネージャの更新処理
				pManager->Update();

				// マネージャの描画処理
				pManager->Draw();

#ifdef _DEBUG

#endif

				dwFrameCount++;
			}
		}
		return 0;

	}


	return DefWindowProc(hwnd, message, wParam, lParam);
}


