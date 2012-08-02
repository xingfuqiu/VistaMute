#include "VistaMute.h"

template <class T>
inline void SafeRelease(T &p)
{
	if (p != NULL)
	{
		p->Release();
		p = NULL;
	}
}


CVistaVolume::CVistaVolume()
{
	m_pEnumerator = NULL;
	//m_pSAV = NULL;
	m_bMute = FALSE;
}

CVistaVolume::~CVistaVolume()
{
	Uninit();
}

HRESULT CVistaVolume::Init()
{
	HRESULT hr = E_FAIL;
	CoInitialize( NULL  );
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&m_pEnumerator);
	if (FAILED(hr))
		return hr;

	return hr;
}

HRESULT CVistaVolume::Uninit()
{
	SafeRelease(m_pEnumerator);
	//SafeRelease(m_pSAV);
	return S_OK;
}

HRESULT CVistaVolume::SetVolume(float fVolume)
{
	HRESULT hr = E_FAIL;

	//hr = m_pSAV->SetMasterVolume(fVolume, NULL);

	return hr;
}

HRESULT CVistaVolume::GetVolume(float *pfVolume)
{
	HRESULT hr = E_FAIL;

	//hr = m_pSAV->GetMasterVolume(pfVolume);

	return hr;
}

HRESULT CVistaVolume::SetMute(BOOL bMute)
{
	if (m_bMute == bMute)
		return S_OK;

	HRESULT hr = E_FAIL;

	IMMDevice* pDevice;
	hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	if (FAILED(hr))
		return hr;

	IAudioSessionManager2* pasm = NULL;
	hr = pDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pasm);
	if (FAILED(hr))
		return hr;

	IAudioSessionEnumerator* audio_session_enumerator;
	if(SUCCEEDED(pasm->GetSessionEnumerator(&audio_session_enumerator)))
	{
		int count;
		
		if(SUCCEEDED(audio_session_enumerator->GetCount(&count)))
		{
			for (int i = 0; i < count; i++)  
			{
				IAudioSessionControl* audio_session_control;
				IAudioSessionControl2* audio_session_control2;

				if(SUCCEEDED(audio_session_enumerator->GetSession(i, &audio_session_control)))
				{
					if(SUCCEEDED(audio_session_control->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&audio_session_control2)))
					{
						DWORD processid;
						
						if(SUCCEEDED(audio_session_control2->GetProcessId(&processid)))
						{
							if (processid == GetCurrentProcessId())
							{
								ISimpleAudioVolume* pSAV;
								hr = audio_session_control2->QueryInterface(__uuidof(ISimpleAudioVolume), (void**) &pSAV);
								if (SUCCEEDED(hr))
								{
									hr = pSAV->SetMute(bMute, NULL);
									if (SUCCEEDED(hr))
									{
										m_bMute = bMute;
									}
									pSAV->Release();
								}
							}
							audio_session_control->Release();
							audio_session_control2->Release();
						}
					}
				}
			}
			audio_session_enumerator->Release();
		}
	} 

	pasm->Release();

	return hr;
}

BOOL CVistaVolume::GetMute()
{
	return m_bMute;
}