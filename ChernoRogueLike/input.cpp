//=============================================================================
//
// 入力処理 [input.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		JOY_MIN_X		(-256)		// Ｘ軸最小値
#define		JOY_MAX_X		(256)		// Ｘ軸最大値
#define		JOY_MIN_Y		(-256)		// Ｙ軸最小値
#define		JOY_MAX_Y		(256)		// Ｙ軸最大値

//*****************************************************************************
// 静的変数
//*****************************************************************************
ComPtr<IDirectInput8>	CInput::m_pDInput;			// IDirectInput8インターフェースへのポインタ

														//=============================================================================
														// CInputコンストラスタ
														//=============================================================================
CInput::CInput()
{
}

//=============================================================================
// CInputデストラスタ
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// 入力処理の初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	// DirectInputオブジェクトの作成
	if (!m_pDInput)
	{
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pDInput, NULL);
	}

	return hr;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void CInput::Uninit(void)
{
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
#if 0	// ---> 純粋仮想関数化
HRESULT CInput::Update(void)
{
	return S_OK;
}
#endif

//=============================================================================
// CInputKeyboardコンストラスタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	// 各ワークのクリア
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

//=============================================================================
// CInputKeyboardデストラスタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// キーボードの初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init(hInst, hWnd);

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, m_pDIDevice.GetAddressOf(), NULL);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDeviceアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}

//=============================================================================
// キーボード入力処理の開放
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// 入力処理の開放
	CInput::Uninit();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
HRESULT CInputKeyboard::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[256];

	if (!m_pDIDevice)
	{
		return S_FALSE;
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);
	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < 256; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if (aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;
				if (m_aKeyStateRepeatCnt[nCntKey] >= 20)
				{
					m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;
				m_aKeyStateRepeat[nCntKey] = 0;
			}

			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

//=============================================================================
// キーボードデータ取得(プレス)
//=============================================================================
BOOL CInputKeyboard::GetKeyPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? TRUE : FALSE;
}

BOOL CInputKeyboard::GetAnyKeyTrigger(void)
{
	for (int i = 0; i < 256; i++)
	{
		if (m_aKeyStateTrigger[i] & 0x80)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// キーボードデータ取得(トリガー)
//=============================================================================
BOOL CInputKeyboard::GetKeyTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードデータ取得(リリース)
//=============================================================================
BOOL CInputKeyboard::GetKeyRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードデータ取得(リピート)
//=============================================================================
BOOL CInputKeyboard::GetKeyRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードデータ削除(トリガー)
//=============================================================================
void CInputKeyboard::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[nKey] = 0;
}

//=============================================================================
// CInputMouseコンストラスタ
//=============================================================================
CInputMouse::CInputMouse()
{
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));
	ZeroMemory(&m_mouseStateTrigger, sizeof(m_mouseStateTrigger));
}

//=============================================================================
// CInputMouseデストラスタ
//=============================================================================
CInputMouse::~CInputMouse()
{
}

//=============================================================================
// マウスの初期化
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init(hInst, hWnd);

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスの設定
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = m_pDIDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}

//=============================================================================
// マウス入力処理の開放
//=============================================================================
void CInputMouse::Uninit(void)
{
	// 入力処理の開放
	CInput::Uninit();
}

//=============================================================================
// マウス入力処理更新
//=============================================================================
HRESULT CInputMouse::Update(void)
{
	HRESULT hr;
	DIMOUSESTATE2 mouseState;

	if (!m_pDIDevice)
	{
		return S_FALSE;
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(hr))
	{
		m_mouseStateTrigger.lX = ((m_mouseState.lX ^ mouseState.lX) & mouseState.lX);
		m_mouseStateTrigger.lY = ((m_mouseState.lY ^ mouseState.lY) & mouseState.lY);
		m_mouseStateTrigger.lZ = ((m_mouseState.lZ ^ mouseState.lZ) & mouseState.lZ);
		for (int nCntKey = 0; nCntKey < 8; nCntKey++)
		{
			m_mouseStateTrigger.rgbButtons[nCntKey]
				= ((m_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey]) & mouseState.rgbButtons[nCntKey]);
		}

		m_mouseState = mouseState;

		// スクリーン座標を取得
		GetCursorPos(&m_posMouseWorld);
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

//=============================================================================
// マウスデータ取得(左プレス)
//=============================================================================
BOOL CInputMouse::GetLeftPress(void)
{
	return (m_mouseState.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(左トリガー)
//=============================================================================
BOOL CInputMouse::GetLeftTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(右プレス)
//=============================================================================
BOOL CInputMouse::GetRightPress(void)
{
	return (m_mouseState.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(右トリガー)
//=============================================================================
BOOL CInputMouse::GetRightTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(中央プレス)
//=============================================================================
BOOL CInputMouse::GetCenterPress(void)
{
	return (m_mouseState.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(中央トリガー)
//=============================================================================
BOOL CInputMouse::GetCenterTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// マウスデータ取得(Ｘ軸移動)
//=============================================================================
LONG CInputMouse::GetAxisX(void)
{
	return m_mouseState.lX;
}

//=============================================================================
// マウスデータ取得(Ｙ軸移動)
//=============================================================================
LONG CInputMouse::GetAxisY(void)
{
	return m_mouseState.lY;
}

//=============================================================================
// マウスデータ取得(Ｚ軸移動)
//=============================================================================
LONG CInputMouse::GetAxisZ(void)
{
	return m_mouseState.lZ;
}

//=============================================================================
// CInputJoypadコンストラスタ
//=============================================================================
CInputJoypad::CInputJoypad()
{
}

//=============================================================================
// CInputJoypadデストラスタ
//=============================================================================
CInputJoypad::~CInputJoypad()
{
}

//=============================================================================
// ジョイパッドの初期化
//=============================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init(hInst, hWnd);

	hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, CInputJoypad::EnumJoyCallback, this, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		//		MessageBox(hWnd, "ジョイパッドがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ジョイパッドのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ジョイパッドの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	hr = m_pDIDevice->EnumObjects(CInputJoypad::EnumAxesCallback, this, DIDFT_AXIS);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ジョイパッドの・・・", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスの設定
#if 0
	{
		DIPROPRANGE diprg;

		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);

		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = JOY_MIN_X;
		diprg.lMax = JOY_MAX_X;
		m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｘ方向の範囲を指定

		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = JOY_MIN_Y;
		diprg.lMax = JOY_MAX_Y;
		m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｙ方向の範囲を指定

		diprg.diph.dwObj = DIJOFS_RY;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = JOY_MIN_Y;
		diprg.lMax = JOY_MAX_Y;
		m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｙ方向の範囲を指定
	}
#endif

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}

//=============================================================================
// ジョイスティック問い合わせ用コールバック関数
//=============================================================================
BOOL CALLBACK CInputJoypad::EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	DIDEVCAPS	diDevCaps;			// デバイス情報
	HRESULT		hRes;
	CInputJoypad *pThis = reinterpret_cast<CInputJoypad*>(pvRef);

	// ジョイスティック用デバイスオブジェクトを作成
	hRes = pThis->m_pDInput->CreateDevice(lpddi->guidInstance, &pThis->m_pDIDevice, NULL);
	if (FAILED(hRes))
	{
		return DIENUM_CONTINUE;		// 列挙を続ける
	}

	// ジョイスティックの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = pThis->m_pDIDevice->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		pThis->m_pDIDevice.Reset();

		return DIENUM_CONTINUE;		// 列挙を続ける
	}

	return DIENUM_STOP;		// このデバイスを使うので列挙を終了する
}

BOOL CALLBACK CInputJoypad::EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	HRESULT hr;
	CInputJoypad *pThis = reinterpret_cast<CInputJoypad*>(pvRef);

	// 軸の値の範囲を設定（-1000～1000）
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -1000;
	diprg.lMax = +1000;
	hr = pThis->m_pDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

//=============================================================================
// ジョイパッド入力処理の開放
//=============================================================================
void CInputJoypad::Uninit(void)
{
	// 入力処理の開放
	CInput::Uninit();
}

//=============================================================================
// ジョイパッド入力処理更新
//=============================================================================
HRESULT CInputJoypad::Update(void)
{
	HRESULT hr;
	bool aKeyStateOld[KEY_MAX];

	if (!m_pDIDevice.Get())
	{
		return E_FAIL;
	}

	// 前回のデータを保存
	for (int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
	{
		aKeyStateOld[nCntKey] = m_aKeyState[nCntKey];
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(m_joypadState), &m_joypadState);
	if (SUCCEEDED(hr))
	{
		// キー情報設定
		SetKeyState();

		for (int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyState[nCntKey]) & m_aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyState[nCntKey]) & !m_aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if (m_aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;
				if (m_aKeyStateRepeatCnt[nCntKey] >= 20)
				{
					m_aKeyStateRepeat[nCntKey] = m_aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;
				m_aKeyStateRepeat[nCntKey] = 0;
			}
		}
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

//=============================================================================
// ジョイパッドのキー情報設定
//=============================================================================
void CInputJoypad::SetKeyState(void)
{
	if (m_joypadState.lX <= -8)
	{
		m_aKeyState[KEY_LEFT] = true;
	}
	else
	{
		m_aKeyState[KEY_LEFT] = false;
	}

	if (m_joypadState.lX >= 8)
	{
		m_aKeyState[KEY_RIGHT] = true;
	}
	else
	{
		m_aKeyState[KEY_RIGHT] = false;
	}

	if (m_joypadState.lY <= -8)
	{
		m_aKeyState[KEY_UP] = true;
	}
	else
	{
		m_aKeyState[KEY_UP] = false;
	}

	if (m_joypadState.lY >= 8)
	{
		m_aKeyState[KEY_DOWN] = true;
	}
	else
	{
		m_aKeyState[KEY_DOWN] = false;
	}

	if (m_joypadState.rgbButtons[0])
	{
		m_aKeyState[KEY_JUMP] = true;
	}
	else
	{
		m_aKeyState[KEY_JUMP] = false;
	}

	if (m_joypadState.rgbButtons[1])
	{
		m_aKeyState[KEY_SHOT] = true;
	}
	else
	{
		m_aKeyState[KEY_SHOT] = false;
	}

	if (m_joypadState.rgbButtons[6])
	{
		m_aKeyState[KEY_TURN_LEFT] = true;
	}
	else
	{
		m_aKeyState[KEY_TURN_LEFT] = false;
	}

	if (m_joypadState.rgbButtons[8])
	{
		m_aKeyState[KEY_TURN_RIGHT] = true;
	}
	else
	{
		m_aKeyState[KEY_TURN_RIGHT] = false;
	}

	if (m_joypadState.rgbButtons[7])
	{
		m_aKeyState[KEY_TURN_UP] = true;
	}
	else
	{
		m_aKeyState[KEY_TURN_UP] = false;
	}

	if (m_joypadState.rgbButtons[9])
	{
		m_aKeyState[KEY_TURN_DOWN] = true;
	}
	else
	{
		m_aKeyState[KEY_TURN_DOWN] = false;
	}

	if (m_joypadState.rgbButtons[11])
	{
		m_aKeyState[KEY_START] = true;
	}
	else
	{
		m_aKeyState[KEY_START] = false;
	}

	if (m_joypadState.rgbButtons[10])
	{
		m_aKeyState[KEY_SELECT] = true;
	}
	else
	{
		m_aKeyState[KEY_SELECT] = false;
	}
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputJoypad::GetKeyPress(KEY key)
{
	return m_aKeyState[key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputJoypad::GetKeyTrigger(KEY key)
{
	return m_aKeyStateTrigger[key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputJoypad::GetKeyRelease(KEY key)
{
	return m_aKeyStateRelease[key];
}

//=============================================================================
// ジョイパッド情報取得
//=============================================================================
bool CInputJoypad::GetKeyRepeat(KEY key)
{
	return m_aKeyStateRepeat[key];
}

