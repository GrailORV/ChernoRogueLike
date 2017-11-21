//=============================================================================
//
// �T�E���h�̏��� [sound.cpp]
// Author : SORA ENOMOTO
//
//=============================================================================
#include "sound.h"
#include "winApp.h"
#include "stdafx.h"

//*****************************************************************************
// �ÓI�ϐ�
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
// CSound�R���X�g���X�^
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
// CSound�f�X�g���N�^
//=============================================================================
CSound::~CSound()
{
	Release();
}

//=============================================================================
// �Q�ƃR�s�[
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
// �Q�ƃJ�E���^�C���N�������g
//=============================================================================
ULONG CSound::AddRef(void)
{
	return ++m_dwRef;
}

//=============================================================================
// �I������
//=============================================================================
ULONG CSound::Release(void)
{
	if (--m_dwRef <= 0)
	{
		// �T�E���h�̒�~
		StopAll();

		// BGM�̔j��
		for (UINT i = 0; i < BGM_LABEL_MAX; i++)
		{
			SafeRelease(m_apBGMPlayer[i]);
		}

		// SE�̔j��
		for (UINT i = 0; i < MAX_VOICE; i++)
		{
			// �\�[�X�{�C�X�̔j��
			if (m_apSourceVoice[i])
			{
				m_apSourceVoice[i]->DestroyVoice();
				m_apSourceVoice[i] = NULL;
			}
		}
		for (UINT i = 0; i < SE_LABEL_MAX; i++)
		{
			// �I�[�f�B�I�f�[�^�̉��
			free(m_apDataAudio[i]);
			m_apDataAudio[i] = NULL;
		}

		// �}�X�^�[�{�C�X�̔j��
		if (m_pMasteringVoice)
		{
			m_pMasteringVoice->DestroyVoice();
			m_pMasteringVoice = NULL;
		}

		// XAudio2�I�u�W�F�N�g�̔j��
		SafeRelease(m_pXAudio2);

		// COM���C�u�����̏I������
		CoUninitialize();

		delete this;
		return 0;
	}
	return m_dwRef;
}

//=============================================================================
// �T�E���h�̏���������
//=============================================================================
HRESULT CSound::Init(void)
{
	HRESULT hr{};

	// BGM�̏�����
	for (UINT i = 0; i < BGM_LABEL_MAX; i++)
	{
		hr = MFPCreateMediaPlayer(m_aBGMParam[i].pFileName, FALSE, 0, NULL, CWinApp::GetHwnd(), &m_apBGMPlayer[i]);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	// SE�̏�����
	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		// COM���C�u�����̏I������
		CoUninitialize();
		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		if (m_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SE_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(m_aSEParam[nCntSound].pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
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

		// �t�H�[�}�b�g�`�F�b�N
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

		// �I�[�f�B�I�f�[�^�ǂݍ���
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
		// �\�[�X�{�C�X�̐���
		m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[i], &format);
	}

	return S_OK;

	return S_OK;
}


//=============================================================================
// �T�E���h�̍X�V����
//=============================================================================
void CSound::Update(void)
{
	// BGM���[�v�Ȃ�J��Ԃ�����
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
// BGM�̍Đ�
//=============================================================================
void CSound::Play(BGM_LABEL label)
{
	PROPVARIANT var;
	PropVariantInit(&var);

	if (m_apBGMPlayer[label])
	{
		var.vt = VT_I8;
		var.hVal.QuadPart = 0;

		// �Đ��ʒu�����߂Ɉړ�
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
// SE�̍Đ�
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

	// ��Ԏ擾
	m_apSourceVoice[m_nNextVoiceIndex]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		m_apSourceVoice[m_nNextVoiceIndex]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[m_nNextVoiceIndex]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_apSourceVoice[m_nNextVoiceIndex]->SubmitSourceBuffer(&buffer);

	// ���ʂ̐ݒ�
	m_apSourceVoice[label]->SetVolume(m_aSEParam[label].fVolume);

	// �Đ�
	m_apSourceVoice[m_nNextVoiceIndex]->Start(0);

	// ���Ɏg���\�[�X�{�C�X�ݒ�
	m_nNextVoiceIndex++;
	if (m_nNextVoiceIndex >= MAX_VOICE)
	{
		m_nNextVoiceIndex = 0;
	}
}

//=============================================================================
// BGM�̒�~
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
// SE�̒�~
//=============================================================================
void CSound::Stop(SE_LABEL label)
{

}

//=============================================================================
// �S�ẴT�E���h�̒�~
//=============================================================================
void CSound::StopAll(void)
{
	StopAllBGM();
	StopAllSE();
}

//=============================================================================
// �S�Ă�BGM�̒�~
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
// �S�Ă�SE�̒�~
//=============================================================================
void CSound::StopAllSE(void)
{
	for (UINT i = 0; i < MAX_VOICE; i++)
	{
		m_apSourceVoice[i]->Stop(0);
	}
}

//=============================================================================
// BGM�̉��ʃZ�b�g
//=============================================================================
void CSound::SetVolume(BGM_LABEL label, float value)
{
	if (m_apBGMPlayer[label])
	{
		m_apBGMPlayer[label]->SetVolume(value);
	}
}

//=============================================================================
// SE�̉��ʃZ�b�g
//=============================================================================
void CSound::SetVolume(SE_LABEL label, float value)
{
	m_aSEParam[label].fVolume = value;
}

//=============================================================================
// �`�����N�̃`�F�b�N
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�^�C�v�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���i�߂�
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
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}




