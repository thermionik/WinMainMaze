// g_audio.h

//------------------------------------------------------------------//
//- class SOUND ----------------------------------------------------//
//------------------------------------------------------------------//
//- A class to hold the information for a single sound.  You use   -//
//- AUDIO class to manage these.								   -//
//------------------------------------------------------------------//


#include <windows.h>		
#include <windowsx.h>		
#include <mmsystem.h>
#include <iostream>
#include <dinput.h>
#include <dmusicc.h>
//#include <dmusici.h>

#ifndef g_audio_h
#define g_audio_h

#pragma comment(lib, "Dsound.lib")

extern HWND hwnd;
/* DirectAudio constants */
#define LPDIRECTMUSICLOADER8	  IDirectMusicLoader8*
#define LPDIRECTMUSICPERFORMANCE8 IDirectMusicPerformance8*
#define LPDIRECTMUSICSEGMENT8	  IDirectMusicSegment8*
#define LPDIRECTMUSICAUDIOPATH	  IDirectMusicAudioPath*
#define LPDIRECTSOUND3DBUFFER	  IDirectSound3DBuffer*
#define LPDIRECTSOUND3DLISTENER	  IDirectSound3DListener*

#if 0
class SOUND
{
	public:
		LPDIRECTMUSICSEGMENT8 dmSegment;
		LPDIRECTSOUND3DBUFFER ds3DBuffer;
		bool is3DSound;

	void Set3DPos(float x, float y, float z);
	void Set3DDistances(float minDistance, float maxDistance);	
	void Shutdown(void);

	SOUND() : dmSegment(NULL),  ds3DBuffer(NULL), is3DSound(false) 
	{	}

	~SOUND() 
	{	}
};

//------------------------------------------------------------------//
//- class AUDIO ---------------------------------------------//
//------------------------------------------------------------------//
//- The DirectAudio class... It plays sounds, duh.				   -//
//------------------------------------------------------------------//
class AUDIO
{
	private:

	public:
		LPDIRECTMUSICLOADER8	  dmLoader;			// the loader
		LPDIRECTMUSICPERFORMANCE8 dmPerformance;	// the performance
		LPDIRECTMUSICAUDIOPATH    dm3DAudioPath;	// the audiopath
		LPDIRECTSOUND3DLISTENER	  ds3DListener;		// 3d listener

		DS3DLISTENER dsListenerParams;				// 3d listener properties


	bool Init(void);	
	void Shutdown(void);

	bool Create(SOUND* audio, char* filename, bool is3DSound);

	void Play(SOUND* audio, DWORD numRepeats);
	void Stop(SOUND* audio);

	void SetListenerPos(float x, float y, float z);
	void SetListenerRolloff(float rolloff);
	void SetListenerOrientation(float forwardX, float forwardY, float forwardZ, float topX, float topY, float topZ);


	AUDIO() : dm3DAudioPath(NULL), dmLoader(NULL),
					 dmPerformance(NULL), ds3DListener(NULL)
	{	}

	~AUDIO()
	{	}
};
#endif


#endif g_audio_h
