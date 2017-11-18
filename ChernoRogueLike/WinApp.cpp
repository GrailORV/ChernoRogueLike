//=============================================================================
//
// �E�C���h�E���� [WinApp.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "WinApp.h"
#include "manager.h"

//*****************************************************************************
// �ÓI�ϐ�
//*****************************************************************************
HWND CWinApp::m_hwnd = nullptr;
BOOL CWinApp::m_bInit = FALSE;
const LPSTR CWinApp::CLASS_NAME = "DX9Game";
const LPSTR CWinApp::WINDOW_NAME = "ChernoLogueLike";

//*********************************************************
// �E�C���h�E�N���X
//*********************************************************
int CWinApp::Run(CManager* pManager, HINSTANCE hInstance, int nCmdShow)
{
#ifdef _DEBUG
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// �E�C���h�E�N���X�̏�����
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

	// �E�C���h�E�T�C�Y�̏�����
	RECT wndRect = { 0,0,static_cast<LONG>(pManager->GetWindowWidth()),static_cast<LONG>(pManager->GetWindowHeight()) };
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, false);

	// �E�C���h�E�̏�����
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
	result = MessageBox(NULL, "�t���X�N���[���ɂ��܂����H", WINDOW_NAME, MB_YESNOCANCEL | MB_ICONQUESTION);
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
			// �E�C���h�E�N���X�̓o�^�̉���
			UnregisterClass(CLASS_NAME, wcex.hInstance);
			// �}�l�[�W���̔j��
			SafeDelete(pManager, &CManager::Uninit);
			return 0;
		}
	}
#endif

	// �}�l�[�W���̏�����
	pManager->Init(hInstance, m_hwnd, bWindow);
	m_bInit = TRUE;

	// ����\�̐ݒ�
	timeBeginPeriod(1);

	// �E�C���h�E�̕\��
	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);

	// ���b�Z�[�W���[�v
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// �E�C���h�E�N���X�̓o�^�̉���
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	// �}�l�[�W���̔j��
	SafeDelete(pManager, &CManager::Uninit);

	return static_cast<int>(msg.wParam);
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK CWinApp::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// �t���[���J�E���g�p�ϐ�
	static DWORD dwExecLastTime;
	static DWORD dwFPSLastTime;
	static DWORD dwCurrentTime;
	static DWORD dwFrameCount;
#ifdef _DEBUG
	static int nCountFPS = 0;		// FPS�J�E���^
#endif

	// �}�l�[�W���[�擾
	CManager* pManager = reinterpret_cast<CManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_CREATE:
	{
		// �t���[���J�E���g������
		dwExecLastTime =
			dwFPSLastTime = timeGetTime();
		dwCurrentTime =
			dwFrameCount = 0;

		// �}�l�[�W���̃p�X�ۑ�
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
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
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

				// �}�l�[�W���̍X�V����
				pManager->Update();

				// �}�l�[�W���̕`�揈��
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


