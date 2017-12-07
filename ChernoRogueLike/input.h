//=============================================================================
//
// ���͏��� [input.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

#include "main.h"

//*********************************************************
// ���̓N���X
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
	virtual HRESULT Update(void) = 0;		// ---> �������z�֐���

protected:
	DWORD m_dwRef;

	ComPtr<IDirectInputDevice8>		m_pDIDevice;		// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^
	static ComPtr<IDirectInput8>		m_pDInput;			// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
};

//*********************************************************
// �L�[�{�[�h���̓N���X
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
	BYTE			m_aKeyState[256];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE			m_aKeyStateTrigger[256];	// �g���K�[���[�N
	BYTE			m_aKeyStateRelease[256];	// �����[�X���[�N
	BYTE			m_aKeyStateRepeat[256];		// ���s�[�g���[�N
	int				m_aKeyStateRepeatCnt[256];	// ���s�[�g�J�E���^
};

//*********************************************************
// �}�E�X���̓N���X
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
	DIMOUSESTATE2	m_mouseState;				// �}�E�X�̏�Ԃ��󂯎�郏�[�N
	DIMOUSESTATE2	m_mouseStateTrigger;		// �g���K�[���[�N
	POINT			m_posMouseWorld;			// �}�E�X�̃X�N���[�����W
};

//*********************************************************
// �W���C�p�b�h���̓N���X
//*********************************************************
class CInputJoypad : public CInput
{
public:
	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	HRESULT Update(void);

	// �L�[�̎��
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

	DIJOYSTATE2		m_joypadState;					// �W���C�p�b�h��Ԃ��󂯎�郏�[�N
	bool			m_aKeyState[KEY_MAX];			// �v���X���[�N
	bool			m_aKeyStateTrigger[KEY_MAX];	// �g���K�[���[�N
	bool			m_aKeyStateRelease[KEY_MAX];	// �����[�X���[�N
	bool			m_aKeyStateRepeat[KEY_MAX];		// ���s�[�g���[�N
	int				m_aKeyStateRepeatCnt[KEY_MAX];	// ���s�[�g�J�E���^
};
