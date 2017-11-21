//=============================================================================
//
// サウンドの処理 [sound.h]
// Author : SORA ENOMOTO
//
//=============================================================================
#pragma once

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <mfplay.h>
#include <xaudio2.h>

interface CSound :public IUnknown
{
public:
	// BGMファイル
	enum BGM_LABEL
	{
		BGM_LABEL_NO_CURRY,
		BGM_LABEL_MAX
	};

	// SEファイル
	enum SE_LABEL
	{
		SE_LABEL_DICISION,
		SE_LABEL_MAX
	};



public:
	CSound();
	~CSound();

	STDMETHODIMP QueryInterface(REFIID riid, void**ppvObject);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	HRESULT Init(void);
	void Update(void);

	void Play(BGM_LABEL label);
	void Play(SE_LABEL label);
	void Stop(BGM_LABEL label);
	void Stop(SE_LABEL label);
	void StopAll(void);
	void StopAllBGM(void);
	void StopAllSE(void);
	void SetVolume(BGM_LABEL label, float value);
	void SetVolume(SE_LABEL label, float value);

private:
	unsigned long m_dwRef;

	// BGM用(MFC)
	struct BGM_PARAM
	{
		const wchar_t* pFileName;
		BYTE bLoop;
		BYTE bUse;
	};

	IMFPMediaPlayer* m_apBGMPlayer[BGM_LABEL_MAX];

	static BGM_PARAM m_aBGMParam[BGM_LABEL_MAX];

	// SE用(XAudio2)
	struct SE_PARAM
	{
		const char* pFileName;
		int nCntLoop;
		float fVolume;
	};

	static const UINT MAX_VOICE = 16;

	HRESULT CheckChunk(HANDLE hFile, DWORD dwFormat, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[MAX_VOICE];	// ソースボイス
	BYTE *m_apDataAudio[SE_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SE_LABEL_MAX];					// オーディオデータサイズ

	UINT m_nNextVoiceIndex;

	static SE_PARAM m_aSEParam[SE_LABEL_MAX];

};


