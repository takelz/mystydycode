#ifndef __SOUND_H__
#define __SOUND_H__

#include <xaudio2.h>

#pragma comment(lib, "xaudio2.lib")

struct SoundData
{
	WAVEFORMATEX format;
	BYTE *pBuffer;
	DWORD bufSize;
	IXAudio2SourceVoice* pSource;
};


HRESULT InitSound(void);
void UninitSound(void);

HRESULT CreateSound(const char *file, SoundData *pData);
void ReleaseSound(SoundData *pData);

void PlaySound(SoundData *pData);
void StopSound(SoundData *pData);


#endif // __SOUND_H__