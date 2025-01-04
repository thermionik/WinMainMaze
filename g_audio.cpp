//g_audio.cpp

#include "g_audio.h"

void SOUND::Set3DPos(float x, float y, float z)
{	
	ds3DBuffer->SetPosition(x, y, -z, DS3D_IMMEDIATE);	
}

void SOUND::Set3DDistances(float minDistance, float maxDistance)
{
	DS3DBUFFER dsBufferParams;

	dsBufferParams.flMinDistance= minDistance;	//Sets the minimum distance
	dsBufferParams.flMaxDistance= maxDistance;	//Sets the maximum distance

	//Now, set the parameters for our sound's 3D buffer
	if(ds3DBuffer)
		ds3DBuffer->SetAllParameters(&dsBufferParams, DS3D_IMMEDIATE);
}

void SOUND::Shutdown(void)
{
	if(dmSegment!=NULL)
	{
		dmSegment->Release();
		dmSegment= NULL;
	}

	if(ds3DBuffer!=NULL)
	{
		ds3DBuffer->Release(); 
		ds3DBuffer= NULL;
	}
}

bool AUDIO::Init(void)
{
	HRESULT hr;
	WCHAR wcharStr[MAX_PATH];
	char pathStr[MAX_PATH];


	//Create the loader object, load it... and load it good.
	hr= CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC,
						IID_IDirectMusicLoader8, (void**)&dmLoader);
	if(FAILED(hr))
	{
		MessageBox(hwnd, "Unable to create main DirectAudio file loader", "ERROR", MB_OK);
		// S3Dlog.Output("	Unable to create main DirectAudio file loader");
		return false;
	}

	//Create the performance object for that.... performance stuff. 
	hr= CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC,
						 IID_IDirectMusicPerformance8, (void**)&dmPerformance);
	if(FAILED(hr))
	{
		MessageBox(hwnd, "Unable to create main DirectAudio performance object", "ERROR", MB_OK);
		// S3Dlog.Output("	Unable to create main DirectAudio performance object");
		Shutdown();
		return false;
	}

	//Initialize the performance with the standard audio path information
	dmPerformance->InitAudio(NULL, NULL, hwnd, DMUS_APATH_SHARED_STEREOPLUSREVERB, 64,
							 DMUS_AUDIOF_ALL, NULL);

    //Create a simple 3D audiopath
	hr= dmPerformance->CreateStandardAudioPath(DMUS_APATH_DYNAMIC_3D,
												64, TRUE, &dm3DAudioPath);
    if(FAILED(hr))
	{
		MessageBox(hwnd, "Unable to create the DirectAudio audio path", "ERROR", MB_OK);
		// S3Dlog.Output("	Unable to create the DirectAudio audio path");
		Shutdown();
		return false;
	}

    //Retrieve the listener information from our audiopath
	hr= dm3DAudioPath->GetObjectInPath(0, DMUS_PATH_PRIMARY_BUFFER, 0, GUID_NULL, 0, 
											IID_IDirectSound3DListener8,
											(void**)&ds3DListener);
    if(FAILED(hr))
	{
		MessageBox(hwnd, "Unable to recieve listener from audio path", "ERROR", MB_OK);
		// S3Dlog.Output("	Unable to recieve listener from audio path");
		Shutdown();
		return false;
	}

	//Get the listener information
	dsListenerParams.dwSize= sizeof(DS3DLISTENER);
	ds3DListener->GetAllParameters(&dsListenerParams);

	//Set our default position of listener (since the listener shouldn't be moving all too much
	dsListenerParams.vPosition.x= 0.0f;
	dsListenerParams.vPosition.y= 0.0f;
	dsListenerParams.vPosition.z= 0.0f;
	ds3DListener->SetAllParameters(&dsListenerParams, DS3D_IMMEDIATE);

	//Retrieve the current directory
	GetCurrentDirectory(MAX_PATH, pathStr);

	//Convert the file name to a unicode string... as thats what COM needs
	MultiByteToWideChar(CP_ACP, 0, pathStr, -1, wcharStr, MAX_PATH);

	//Set the search directory
	dmLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wcharStr, FALSE);

	// S3Dlog.Output("	DirectAudio initialized successfully");
	return true;
}

void AUDIO::Shutdown(void)
{
	// S3Dlog.Output("DirectAudio shutdown:");
	//Shut down the direct music performance information
	if(dmPerformance!=NULL)
	{
		dmPerformance->Stop(NULL, NULL, 0, 0);
		dmPerformance->CloseDown();
		dmPerformance->Release();
		dmPerformance = NULL;
	}

	//Shut down the direct sound listener(player) information
//	if(ds3DListener!=NULL)
//	{
//		ds3DListener->Release();
//		ds3DListener = NULL;
//	}  // For some reason, this breaks everything... :(  fixme
	//Shut down the direct music audiopath
	if(dm3DAudioPath!=NULL)
	{
		dm3DAudioPath->Release();
		dm3DAudioPath = NULL;
	}
	//Shut down the direct music loader
	if(dmLoader!=NULL)
	{
		dmLoader->Release();
		dmLoader = NULL;
	}

	// S3Dlog.Output("	DirectAudio has been shut down successfully");
}

bool AUDIO::Create(SOUND* audio, char* filename, bool is3DSound)
{
	DS3DBUFFER dsBufferParams; 
	HRESULT hr;
	WCHAR wcharStr[MAX_PATH];

	//Convert the file name to the string that DirectAudio needs
	MultiByteToWideChar(CP_ACP, 0, filename, -1, wcharStr, MAX_PATH);

	//Load the audio segment from a file
	hr= dmLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,
									 IID_IDirectMusicSegment8,
									 wcharStr,
									 (void**)&audio->dmSegment);
	if(FAILED(hr))
	{
		// S3Dlog.Output("Could not load %s", filename);
		return false;
	}

	//Do code specific for 3D sounds
	if(is3DSound)
	{
		//Get the 3D buffer, and audio path
		hr= dm3DAudioPath->GetObjectInPath(DMUS_PCHANNEL_ALL, DMUS_PATH_BUFFER, 0, 
										   GUID_NULL, 0, IID_IDirectSound3DBuffer,
										   (void**)&audio->ds3DBuffer);
		if(FAILED(hr))
		{
			// S3Dlog.Output("Could not get 3D buffer for %s", filename);
			return false;
		}

		//Get the 3D buffer paramters
		dsBufferParams.dwSize= sizeof(DS3DBUFFER);
		audio->ds3DBuffer->GetAllParameters(&dsBufferParams);

		//Set some new parameters
		dsBufferParams.dwMode= DS3DMODE_HEADRELATIVE;    // relative to the listener
		audio->ds3DBuffer->SetAllParameters(&dsBufferParams, DS3D_IMMEDIATE);

		//Set the 3D sound flag to true
		audio->is3DSound = true;
	}
	//Do code specific to non-3D sounds (background music)
	else
	{
		//Set the 3D buffer to null since we don't need it
		audio->ds3DBuffer= NULL;
		//Set the 3D sound flag to false, since we don't need it either
		audio->is3DSound= false;
	}

	// S3Dlog.Output("Loaded %s correctly", filename);
	return true;
}

void AUDIO::Play(SOUND* audio, DWORD numRepeats)
{
	//Set the number of repeats that we want for the specific sound
	audio->dmSegment->SetRepeats(numRepeats);

	if(audio->is3DSound)
	{
		audio->dmSegment->Download(dm3DAudioPath);

		//Play the segment using the 3D audio path
		dmPerformance->PlaySegmentEx(audio->dmSegment, NULL, NULL, 
									 DMUS_SEGF_SECONDARY, 0,
									 NULL, NULL, dm3DAudioPath);
	}
	else
	{
		//Download the segment's performance object
		audio->dmSegment->Download(dmPerformance);
		//Play the non-3D sound segment
		dmPerformance->PlaySegmentEx(audio->dmSegment, NULL, NULL, 
									 DMUS_SEGF_DEFAULT, 0,
									 NULL, NULL, NULL);
		
	}
}

void AUDIO::Stop(SOUND* audio)
{	
	dmPerformance->StopEx(audio->dmSegment, 0, 0);	
}

void AUDIO::SetListenerPos(float x, float y, float z)
{
	//Get the listener parameters
	dsListenerParams.dwSize= sizeof(DS3DLISTENER);
	ds3DListener->GetAllParameters(&dsListenerParams);

	//Set the listener's position in 3D space
	dsListenerParams.vPosition.x=  x;
	dsListenerParams.vPosition.y=  y;
	dsListenerParams.vPosition.z= -z;

	//Set all of the new parameters
	ds3DListener->SetAllParameters(&dsListenerParams, DS3D_IMMEDIATE);
}

//------------------------------------------------------------------//
//- void AUDIO::SetListenerRolloff(float) -------------------//
//------------------------------------------------------------------//
//- Set the listener's sound volume rolloff with distance.		   -//
//------------------------------------------------------------------//
void AUDIO::SetListenerRolloff(float rolloff)
{
	if(ds3DListener)
		ds3DListener->SetRolloffFactor(rolloff, DS3D_IMMEDIATE);
}

//------------------------------------------------------------------//
//- void AUDIO::SetListenerOrientation(float, float, float, float, float, float) -//
//------------------------------------------------------------------//
//- Set the listener's orientation (you may need to implement a    -//
//- camera class to use this function correctly).				   -//
//------------------------------------------------------------------//
void AUDIO::SetListenerOrientation(float forwardX, float forwardY, float forwardZ,
						   float topX, float topY, float topZ)
{
	ds3DListener->SetOrientation(forwardX, forwardY, -forwardZ, 
								 topX, topY, topZ, DS3D_IMMEDIATE);
}