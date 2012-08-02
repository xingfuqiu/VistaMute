// VistaVolume.h
#ifndef _VISTAVOLUME_H_
#define _VISTAVOLUME_H_

#include <Windows.h>
#include <Mmdeviceapi.h> // for IMMDeviceEnumerator and IMMDevice
#include <Endpointvolume.h> // for IAudioEndpointVolume, must #define NTDDI_VERSION NTDDI_WINXPSP1 in stdafx.h
#include <Audiopolicy.h>
#include <Audioclient.h>

class CVistaVolume
{
public:
	CVistaVolume();
	~CVistaVolume();

public:
	HRESULT Init();
	HRESULT Uninit();
	HRESULT SetVolume(float fVolume);
	HRESULT GetVolume(float *pfVolume);
	HRESULT SetMute(BOOL bMute);
	BOOL GetMute();

private:
	IMMDeviceEnumerator *m_pEnumerator;
	BOOL m_bMute;
};

#endif //_VISTAVOLUME_H_

