//=============================================================================
//
// 入力処理 [input.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "main.h"

//*********************************************************
// 入力クラス
//*********************************************************
class CInput :public IUnknown
{
public:
	CInput();
	virtual ~CInput();

	STDMETHODIMP QueryInterface(REFIID riid, void FAR* FAR* ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	virtual HRESULT Init(HINSTANCE hInst, HWND hWnd);
	virtual HRESULT Update(void) = 0;		// ---> 純粋仮想関数化

protected:
	DWORD m_dwRef;

	ComPtr<IDirectInputDevice8>		m_pDIDevice;		// IDirectInputDevice8インターフェースへのポインタ
	static ComPtr<IDirectInput8>		m_pDInput;			// IDirectInput8インターフェースへのポインタ
};

//*********************************************************
// キーボード入力クラス
//*********************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	HRESULT Update(void);

	BOOL GetKeyPress(int nKey);
	BOOL GetKeyTrigger(int nKey);
	BOOL GetKeyRelease(int nKey);
	BOOL GetKeyRepeat(int nKey);
	void FlushKeyTrigger(int nKey);

	BOOL GetAnyKeyTrigger(void);

private:
	BYTE			m_aKeyState[256];			// キーボードの状態を受け取るワーク
	BYTE			m_aKeyStateTrigger[256];	// トリガーワーク
	BYTE			m_aKeyStateRelease[256];	// リリースワーク
	BYTE			m_aKeyStateRepeat[256];		// リピートワーク
	int				m_aKeyStateRepeatCnt[256];	// リピートカウンタ
};

//*********************************************************
// マウス入力クラス
//*********************************************************
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	HRESULT Update(void);

	BOOL GetLeftPress(void);
	BOOL GetLeftTrigger(void);
	BOOL GetRightPress(void);
	BOOL GetRightTrigger(void);
	BOOL GetCenterPress(void);
	BOOL GetCenterTrigger(void);
	LONG GetAxisX(void);
	LONG GetAxisY(void);
	LONG GetAxisZ(void);
	POINT *GetPosWorld(void) { return &m_posMouseWorld; }

private:
	DIMOUSESTATE2	m_mouseState;				// マウスの状態を受け取るワーク
	DIMOUSESTATE2	m_mouseStateTrigger;		// トリガーワーク
	POINT			m_posMouseWorld;			// マウスのスクリーン座標
};

//*********************************************************
// ジョイパッド入力クラス
//*********************************************************
class CInputJoypad : public CInput
{
public:
	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	HRESULT Update(void);

	// キーの種類
	typedef enum
	{
		KEY_LEFT = 0,
		KEY_RIGHT,
		KEY_UP,
		KEY_DOWN,
		KEY_JUMP,
		KEY_SHOT,
		KEY_TURN_LEFT,
		KEY_TURN_RIGHT,
		KEY_TURN_UP,
		KEY_TURN_DOWN,
		KEY_START,
		KEY_SELECT,
		KEY_MAX
	} KEY;

	bool GetKeyPress(KEY key);
	bool GetKeyTrigger(KEY key);
	bool GetKeyRelease(KEY key);
	bool GetKeyRepeat(KEY key);

private:
	static BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);
	static BOOL CALLBACK EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
	void SetKeyState(void);

	DIJOYSTATE2		m_joypadState;					// ジョイパッド状態を受け取るワーク
	bool			m_aKeyState[KEY_MAX];			// プレスワーク
	bool			m_aKeyStateTrigger[KEY_MAX];	// トリガーワーク
	bool			m_aKeyStateRelease[KEY_MAX];	// リリースワーク
	bool			m_aKeyStateRepeat[KEY_MAX];		// リピートワーク
	int				m_aKeyStateRepeatCnt[KEY_MAX];	// リピートカウンタ
};
