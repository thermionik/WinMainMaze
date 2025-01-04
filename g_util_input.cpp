//g_util_input.cpp

#include "g_util.h"


DIMOUSESTATE  mouseState;

// DirectInput initialization for keyboard and mouse
bool g_util_input::initDI(void)
{
	HRESULT hr;	//DirectX result variable

	//Create the main DirectInput object
	hr= (DirectInput8Create(hinstance, DIRECTINPUT_VERSION,
						   IID_IDirectInput8, (void**)&lpdi, NULL));
	if(FAILED(hr))
	{
		MessageBox(hwnd, "Could not create main DInput object", TITLE, MB_OK);
		return false;
	}

	//Create the keyboard's device object
	hr= (lpdi->CreateDevice(GUID_SysKeyboard, &lpdi_keyboard, NULL));
	if(FAILED(hr))
	{
		MessageBox(hwnd, "Could not create keyboard's object",TITLE,MB_OK);
		shutdownDI();
		return false;
	}

	//Set the keyboard's data format 
	hr= (lpdi_keyboard->SetDataFormat(&c_dfDIKeyboard));
	if(FAILED(hr))
	{
		MessageBox(hwnd, "Could not set keyboard's data format",TITLE,MB_OK);
		shutdownDI();
		return false;
	}

	//Set the keyboard's cooperation level with your computer's
	//environment.
	hr= (lpdi_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND |
												  DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hwnd, "Could not set keyboard's cooperation level",
				   TITLE, MB_OK);
		shutdownDI();
		return false;
	}

	//And finally, acquire the keyboard for use.
	lpdi_keyboard->Acquire();


	//Its time to get mousey
	hr= lpdi->CreateDevice(GUID_SysMouse, &lpdiMouse, NULL);
	if(FAILED(hr))
		{
		MessageBox(hwnd, "Could not set create the mouse device", "ERROR", MB_OK);
	//	S3Dlog.Output("	Could not set create the mouse device");
		shutdownDI();
		return FALSE;
		}

	//Set the mouse's data format
	hr= lpdiMouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(hr)) 
		{
		MessageBox(hwnd, "Could not set the mouse's data format", "ERROR", MB_OK);
	//	S3Dlog.Output("	Could not set the mouse's data format");
		shutdownDI();
		return FALSE;
		}

	//Set the mouse's cooperative level with that of windows
	hr= lpdiMouse->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hr)) 
		{
		MessageBox(hwnd, "Could not set the mouse's behavior", "ERROR", MB_OK);
	//	S3Dlog.Output("	Could not set the mouse's behavior");
		shutdownDI();
		return FALSE;
		}

	//Acquire the mouse
	hr= lpdiMouse->Acquire();
	if(FAILED(hr))
		{
		MessageBox(hwnd, "Could not acquire the mouse", "ERROR", MB_OK);
	//	S3Dlog.Output("	Could not acquire the mouse");
		shutdownDI();
		}

	//Set the mouse's device state
	hr= lpdiMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	if(FAILED(hr))
		{
		MessageBox(hwnd, "The mouse has been lost in initialization", "ERROR", MB_OK);
	//	S3Dlog.Output("	The mouse has been lost in initialization");
		shutdownDI();
		}

	return true;
}
		
void g_util_input::shutdownDI(void)
{

		if(lpdiMouse!=NULL)
		{
		//Unacquire the mouse
		lpdiMouse->Unacquire();

		//Throw the mouse away
		lpdiMouse->Release();
		lpdiMouse= NULL;
		}
	//First, since we create the keyboard last, we need to 'destroy'
	//it first.
	if(lpdi_keyboard!=NULL)
	{
		//Unacquire the keyboard
		lpdi_keyboard->Unacquire();
		
		//Let the keyboard go faster than your ex-girlfriend's phone number
		lpdi_keyboard->Release();
		lpdi_keyboard=NULL;
	}
	
	//Now the main DirectInput object
	if(lpdi!=NULL)
	{
		//Kill, burn, and destroy the main object
		lpdi->Release();
		lpdi=NULL;
	}
}

void g_util_input::updateDI(void)
{
	HRESULT hr;
	
	//First, check to see if the keyboard is still working/functioning
    hr= (lpdi_keyboard->GetDeviceState(sizeof(UCHAR[256]),(LPVOID)&key_buffer));
    if(FAILED(hr))
	{ 
        if(hr==DIERR_INPUTLOST) 
		{
			//Try to re-acquire the keyboard
			hr= (lpdi_keyboard->Acquire());
			if(FAILED(hr))
			{
				MessageBox(hwnd, "Keyboard has been lost", TITLE, MB_OK);
				shutdownDI();
			}
		}
	}

		//Now check the mouse
    hr= (lpdiMouse->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&mouseState));
    if(FAILED(hr))
		{ 
        if(hr==DIERR_INPUTLOST) 
			{
			//Try to re-acquire the keyboard
			hr= (lpdiMouse->Acquire());
			if(FAILED(hr))
				{
				MessageBox(hwnd, "The mouse has been lost", "ERROR", MB_OK);
				shutdownDI();
				}
			}
		}
}