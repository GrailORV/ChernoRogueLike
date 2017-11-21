//=============================================================================
//
// サウンドの処理 [sound.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "sound.h"
#include "winApp.h"
#include "stdafx.h"

//*****************************************************************************
// 静的変数
//*****************************************************************************
CSound::BGM_PARAM CSound::m_aBGMParam[CSound::BGM_LABEL_MAX] =
{
	{ L"data/BGM/bgm_NO_CURRY_NO_LIFE.mp3",1,0 }
};

CSound::SE_PARAM CSound::m_aSEParam[CSound::SE_LABEL_MAX] =
{
	{ "data/SE/se_decision.wav",0,1.0f }
};

//=============================================================================
// CSoundコンストラスタ
//=============================================================================
CSound::CSound() :
	m_dwRef(0)
{
	for (UINT i = 0; i < BGM_LABEL_MAX; i++)
	{
		m_apBGMPlayer[i] = NULL;
	}

	m_pXAudio2 = NULL;
	m_pMasteringVoice = NULL;
	for (UINT i = 0; i < MAX_VOICE; i++)
	{
		m_apSourceVoice[i] = NULL;
	}
	for (UINT i = 0; i < SE_LABEL_MAX; i++)
	{
		m_apDataAudio[i] = NULL;
		m_aSizeAudio[i] = 0;
	}
	m_nNextVoiceIndex = 0;
}

//=============================================================================
// CSoundデストラクタ
//=============================================================================
CSound::~CSound()
{
	Release();
}

//=============================================================================
// 参照コピー
//=============================================================================
HRESULT CSound::QueryInterface(REFIID riid, void**ppvObject)
{
	if (riid == IID_IUnknown)
	{
		*ppvObject = this;
		AddRef();
		return NO_ERROR;
	}
	return E_NOINTERFACE;
}

//=============================================================================
// 参照カウンタインクリメント
//=============================================================================
ULONG CSound::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// 終了処理
//=============================================================================
ULONG CSound::Release(void)
{
	if (--m_dwRef <= 0)
	{
		// サウンドの停止
		StopAll();

		// BGMの破棄
		for (UINT i = 0; i < BGM_LABEL_MAX; i++)
		{
			SafeRelease(m_apBGMPlayer[i]);
		}

		// SEの破棄
		for (UINT i = 0; i < MAX_VOICE; i++)
		{
			// ソースボイスの破棄
			if (m_apSourceVoice[i])
			{
				m_apSourceVoice[i]->DestroyVoice();
				m_apSourceVoice[i] = NULL;
			}
		}
		for (UINT i = 0; i < SE_LABEL_MAX; i++)
		{
			// オーディオデータの解放
			free(m_apDataAudio[i]);
			m_apDataAudio[i] = NULL;
		}

		// マスターボイスの破棄
		if (m_pMasteringVoice)
		{
			m_pMasteringVoice->DestroyVoice();
			m_pMasteringVoice = NULL;
		}

		// XAudio2オブジェクトの破棄
		SafeRelease(m_pXAudio2);

		// COMライブラリの終了処理
		CoUninitialize();

		delete this;
		return 0;
	}
	return m_dwRef;
}

//=============================================================================
// サウンドの初期化処理
//=============================================================================
HRESULT CSound::Init(void)
{
	HRESULT hr{};

	// BGMの初期化
	for (UINT i = 0; i < BGM_LABEL_MAX; i++)
	{
		hr = MFPCreateMediaPlayer(m_aBGMParam[i].pFileName, FALSE, 0, NULL, CWinApp::GetHwnd(), &m_apBGMPlayer[i]);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	// SEの初期化
	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		if (m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SE_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aSEParam[nCntSound].pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
	}

	WAVEFORMATEX format;
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 2;
	format.nSamplesPerSec = 22050;
	format.wBitsPerSample = 16;
	format.nBlockAlign = format.nChannels*format.wBitsPerSample / 8;
	format.nAvgBytesPerSec = format.nSamplesPerSec*format.nBlockAlign;
	format.cbSize = 0;
	for (u_int i = 0; i < MAX_VOICE; i++)
	{
		// ソースボイスの生成
		m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[i], &format);
	}

	return S_OK;

	return S_OK;
}


//=============================================================================
// サウンドの更新処理
//=============================================================================
void CSound::Update(void)
{
	// BGMループなら繰り返し処理
	for (u_int i = 0; i < BGM_LABEL_MAX; i++)
	{
		if (m_aBGMParam[i].bUse && m_aBGMParam[i].bLoop && m_apBGMPlayer[i])
		{
			MFP_MEDIAPLAYER_STATE state = MFP_MEDIAPLAYER_STATE_EMPTY;
			m_apBGMPlayer[i]->GetState(&state);
			if (state == MFP_MEDIAPLAYER_STATE_STOPPED)
			{
				m_apBGMPlayer[i]->Play();
			}
		}
	}
}

//=============================================================================
// BGMの再生
//=============================================================================
void CSound::Play(BGM_LABEL label)
{
	PROPVARIANT var;
	PropVariantInit(&var);

	if (m_apBGMPlayer[label])
	{
		var.vt = VT_I8;
		var.hVal.QuadPart = 0;

		// 再生位置を初めに移動
		m_apBGMPlayer[label]->SetPosition(
			MFP_POSITIONTYPE_100NS,
			&var
		);
		m_apBGMPlayer[label]->Play();
		m_aBGMParam[label].bUse = 1;
	}

	PropVariantClear(&var);
}

//=============================================================================
// SEの再生
//=============================================================================
void CSound::Play(SE_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aSEParam[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[m_nNextVoiceIndex]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_apSourceVoice[m_nNextVoiceIndex]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[m_nNextVoiceIndex]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[m_nNextVoiceIndex]->SubmitSourceBuffer(&buffer);

	// 音量の設定
	m_apSourceVoice[label]->SetVolume(m_aSEParam[label].fVolume);

	// 再生
	m_apSourceVoice[m_nNextVoiceIndex]->Start(0);

	// 次に使うソースボイス設定
	m_nNextVoiceIndex++;
	if (m_nNextVoiceIndex >= MAX_VOICE)
	{
		m_nNextVoiceIndex = 0;
	}
}

//=============================================================================
// BGMの停止
//=============================================================================
void CSound::Stop(BGM_LABEL label)
{
	if (m_aBGMParam[label].bUse && m_apBGMPlayer[label])
	{
		MFP_MEDIAPLAYER_STATE state = MFP_MEDIAPLAYER_STATE_EMPTY;
		m_apBGMPlayer[label]->GetState(&state);
		if (state == MFP_MEDIAPLAYER_STATE_PAUSED || state == MFP_MEDIAPLAYER_STATE_PLAYING)
		{
			m_apBGMPlayer[label]->Stop();
			m_aBGMParam[label].bUse = 0;
		}
	}
}

//=============================================================================
// SEの停止
//=============================================================================
void CSound::Stop(SE_LABEL label)
{

}

//=============================================================================
// 全てのサウンドの停止
//=============================================================================
void CSound::StopAll(void)
{
	StopAllBGM();
	StopAllSE();
}

//=============================================================================
// 全てのBGMの停止
//=============================================================================
void CSound::StopAllBGM(void)
{
	for (UINT i = 0; i < BGM_LABEL_MAX; i++)
	{
		if (m_aBGMParam[i].bUse && m_apBGMPlayer[i])
		{
			Stop((BGM_LABEL)i);
		}
	}
}

//=============================================================================
// 全てのSEの停止
//=============================================================================
void CSound::StopAllSE(void)
{
	for (UINT i = 0; i < MAX_VOICE; i++)
	{
		m_apSourceVoice[i]->Stop(0);
	}
}

//=============================================================================
// BGMの音量セット
//=============================================================================
void CSound::SetVolume(BGM_LABEL label, float value)
{
	if (m_apBGMPlayer[label])
	{
		m_apBGMPlayer[label]->SetVolume(value);
	}
}

//=============================================================================
// SEの音量セット
//=============================================================================
void CSound::SetVolume(SE_LABEL label, float value)
{
	m_aSEParam[label].fVolume = value;
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD dwFormat, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;

	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクタイプの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分進める
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == dwFormat)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}




