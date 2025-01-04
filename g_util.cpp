//g_util.cpp

#include "g_util.h"

HINSTANCE hinstance; //Handle to the window's instance
HWND  hwnd;		 	 //Handle to the actual window
HDC   hdc;			 //Handle to the window's graphics device context
HGLRC hrc;			 //Handle to the window's rendering context

UCHAR key_buffer[256];	//The keyboard's key buffer
bool keys[256];

GLfloat COS[720];	//Look-up tables for the sin/cosine of 0-720
GLfloat SIN[720];

g_log errlog;			//The error log


//------------------------------------------------------------------//
//- bool g_util:: OpenGL_Init(int, int, int, int) ---------------//
//------------------------------------------------------------------//
//- Description: This function takes care of all the necessary	   -//
//-              OpenGL initiation things.  You can provide the    -//
//-              dimensions (width, height, and bits per pixel),   -//
//-              and you can also make it a window, fullscreen or  -//
//-              give the user a choice, using these flags:	       -//
//-                  - FULLSCREEN (Just start with a fullscreen)   -//
//-                  - WINDOW (Just start as a window)             -//
//-                  - GIVE_CHOICE ("Would you like to run in fullscreen mode?") -//
//------------------------------------------------------------------//
bool g_util::initAll( int w, int h, int bpp, int scr_flag )
{
	WNDCLASS winclass;				//The windows class structure
	DEVMODE  dmScreenSettings;		//Device Mode
	GLuint	 PixelFormat;			//Hold the correct match for the pixel format
	DWORD	 dwExStyle;				//Window extended style
	DWORD	 dwStyle;				//Window style
	RECT     winrect;   

	scr_width =w;			//Set the class's constants to the 
	scr_height=h;			//parameter's provided by the 
	scr_bpp   =bpp;				//user.

	winrect.left  =(long)0;			//Upper left  x (x1)
	winrect.top   =(long)0;			//Upper left  y (y1)
	winrect.right =(long)w;		//Lower right x (x2)
	winrect.bottom=(long)h;	//Lower right y (y2)

	hinstance				=GetModuleHandle(NULL);				//Get the window's instance
	winclass.style			=CS_HREDRAW | CS_VREDRAW | CS_OWNDC;//Redraw the window on resize
	winclass.lpfnWndProc	=WindowProc;						//handle the WINPROC messages
	winclass.cbClsExtra		=0;									//No extra win data
	winclass.cbWndExtra		=0;									//No extra win data
	winclass.hInstance		=hinstance;							//Set the window's instance
	winclass.hIcon			=LoadIcon(NULL, IDI_WINLOGO);		//Load the default icon
	winclass.hCursor		=LoadCursor(NULL, IDC_ARROW);		//Load the mouse pointer
	winclass.hbrBackground	=NULL;								//No background required for OpenGL stuff
	winclass.lpszMenuName	=NULL;								//No menu
	winclass.lpszClassName	=CLASSNAME;							//Set the class name

	errlog.gLogOut("Window initiation:");

	if(!RegisterClass(&winclass))		//Register the window class
	{
		errlog.gLogOut("	Could not register window class");
		return false;		
	}

	else
		errlog.gLogOut("	Window class registered");
	
	if(scr_flag==ASK_FULLSCREEN)
	{
		//Ask the user which screen mode they prefer
		if(MessageBox(NULL,"Would you like to run in fullscreen mode?", TITLE, MB_YESNO|MB_ICONQUESTION)==IDNO)
			fullscreen=FULLSCREEN_OFF;					//Windowed mode
	}

	if( FULLSCREEN || fullscreen==FULLSCREEN_ON )			//Attempt fullscreen mode?
	{
		fullscreen=FULLSCREEN_ON;

		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes sure memory's cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size of the devmode structure
		dmScreenSettings.dmPelsWidth	= w;					// Selected screen width
		dmScreenSettings.dmPelsHeight	= h;					// Selected screen height
		dmScreenSettings.dmBitsPerPel	= bpp;					// Selected bits per pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		//Try to set selected mode and get results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if(ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			//If the mode fails, offer 2 options. Quit or use windowed mode.
			if(MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				errlog.gLogOut("Fullscreen mode not supported.");
				fullscreen=FALSE;		//Windowed mode selected. Turn fullscreen flag off
			}
			else
			{
				//Create a message box letting the user know the program is closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;
			}
		}
	}

	if(FULLSCREEN || fullscreen==FULLSCREEN_ON)					//Did the user want a fullscreen window?
	{
		dwExStyle=WS_EX_APPWINDOW;								//Window extended style
		dwStyle=WS_POPUP;										//Windows style
		ShowCursor(FALSE);										//Hide the mouse pointer
	}
	else
	{
		fullscreen=FULLSCREEN_OFF;
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			//Window extended style
		dwStyle=WS_OVERLAPPEDWINDOW;							//Windows style
	}

	//Adjust window to the actual requested size
	AdjustWindowRectEx(&winrect, dwStyle, FALSE, dwExStyle);

	//Create the window
	if(fullscreen)
	{
		if(!(hwnd=CreateWindowEx(dwExStyle,				//The extended style for the window
								 CLASSNAME,				//The class name
								 TITLE,					//The window's title
								 WS_SYSMENU | WS_BORDER | 
								 WS_CAPTION | WS_VISIBLE,		 
								 0, 0,					//The window's position
								 winrect.right -winrect.left,	//Calculate the window's width
								 winrect.bottom-winrect.top,	//Calculate the window's height
								 NULL,					//No parent window
								 NULL,					//No menu needed
								 hinstance,				//The window's instance
								 NULL)))				//Nothing to pass to WM_CREATE
		{
			//If the window could not be created
			shutdownAll();						//Kill the window				
			errlog.gLogOut("	Window could not be created.");
			return false;			
		}
	}
	else
	{
		if(!(hwnd=CreateWindowEx(dwExStyle,				//The extended style for the window
								 CLASSNAME,				//The class name
								 TITLE,					//The window's title
								 WS_POPUP | WS_VISIBLE, //No minimize/maximize commands
								 0, 0,					//The window's position
								 winrect.right -winrect.left,//Calculate the window's width
								 winrect.bottom-winrect.top,//Calculate the window's height
								 NULL,					//No parent window
								 NULL,					//No menu needed
								 hinstance,				//The window's instance
								 NULL)))				//Nothing to pass to WM_CREATE
		{
			//If the window could not be created
			shutdownAll();						//Kill the window				
			errlog.gLogOut("	Window could not be created.");
			return false;			
		}
	}

	static PIXELFORMATDESCRIPTOR pfd=	//Get the pixel format's description
	{
		sizeof(PIXELFORMATDESCRIPTOR),	//Get the size of the structure
		1,								//Version number
		PFD_DRAW_TO_WINDOW |			//Format must support Windows
		PFD_SUPPORT_OPENGL |			//Format must support OpenGL
		PFD_DOUBLEBUFFER,				//Must support Double Buffering
		PFD_TYPE_RGBA,					//Request a RGBA (red,green,blue,alpha) format
		bpp,							//Select the bits per pixel
		0, 0, 0, 0, 0, 0,				//Color bits ignored
		0,								//No alpha buffer
		0,								//shift bit ignored
		0,								//No accumulation buffer (advanced)
		0, 0, 0, 0,						//Accumulation bits ignored
		16,								//16 bit Z-Buffer (Depth Buffer)  
		0,								//No Stencil Buffer (advanced)
		0,								//No Auxiliary Buffer (advanced)
		PFD_MAIN_PLANE,					//The main drawing layer
		0,								//Reserved area
		0, 0, 0							//Layer masks ignored
		};
	
	//Could the device context be "grabbed?"
	if(!(hdc=GetDC(hwnd)))
	{
		shutdownAll();				//Kill the window
		errlog.gLogOut("	Could not create a GL device context");
		return false;								
	}
	else
		errlog.gLogOut("	OpenGL Device Context created.");

	//Did Windows find a matching pixel format?
	if(!(PixelFormat=ChoosePixelFormat(hdc,&pfd)))
	{
		shutdownAll();				//Kill the window
		errlog.gLogOut("	Could not find a suitable pixel format");
		return false;						
	}
	else
		errlog.gLogOut("	Found suitable pixel format");

	//Could the pixel format be set?
	if(!SetPixelFormat(hdc,PixelFormat,&pfd))
	{
		shutdownAll();				//Kill the window
		errlog.gLogOut("	Could not set pixel format");
		return false;						
	}
	else
		errlog.gLogOut("	Set pixel format.");
	
	//Could a rendering context be "grabbed?"
	if(!(hrc=wglCreateContext(hdc)))
	{
		shutdownAll();				//Kill the window
		errlog.gLogOut("	Could not create an OpenGL Rendering Context");
		return false;		
	}
	else
		errlog.gLogOut("	Created a OpenGL Rendering Context.");

	//Could the rendering context be activated?
	if(!wglMakeCurrent(hdc,hrc))
	{
		shutdownAll();				//Kill the Window
		errlog.gLogOut("	Could not activate an OpenGL Rendering Context");
		return false;			
	}
	else
		errlog.gLogOut("	Activated an OpenGL Rendering Context");

	glEnable(GL_TEXTURE_2D);							//Enable two dimensional texture mapping
	glShadeModel(GL_SMOOTH);							//Enable smooth shading (so you can't see the individual polygons of a primitive, best shown when drawing a sphere)
	glClearColor(0.0, 0.0, 0.0, 0.0);					//Completely black background
	glClearDepth(1.0);									//Depth buffer setup
	glEnable(GL_DEPTH_TEST);							//Enable depth testing
	glDepthFunc(GL_LEQUAL);								//The type of depth testing to do (LEQUAL==less than or equal to)
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//The nicest perspective look

	glColor4f(1.0, 1.0, 1.0, 1.0);						//Full brightness.
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					//Set the blending function for transparency

	ShowWindow(hwnd,SW_SHOW);			//Show the window
	SetForegroundWindow(hwnd);			//Give the window a high priority
	SetFocus(hwnd);						//Sets the keyboard's focus to the window
	reshapeFunc(w, h);		//Set up the OpenGL perspective view

	return true;
}

//------------------------------------------------------------------//
//- bool g_util:: OpenGL_Shutdown(void) -------------------------//
//------------------------------------------------------------------//
//- Description: This function takes care of all the necessary     -//
//-              OpenGL shutdown precautions.					   -//
//------------------------------------------------------------------//
void g_util::shutdownAll(void)
{
	if(fullscreen==FULLSCREEN_ON)			//Are we in fullscreen mode?
	{
		ChangeDisplaySettings(NULL,0);		//If so switch back to the desktop
		ShowCursor(TRUE);					//Show mouse pointer
	}

	errlog.gLogOut("Shutdown:");	

	if(hrc)			//Tru to release rendring context and device context
	{
		if(!wglMakeCurrent(NULL,NULL))	
			errlog.gLogOut("	Could not release RC and DC.");
		else
			errlog.gLogOut("	Released RC and DC.");

		if(!wglDeleteContext(hrc))		
			errlog.gLogOut("	Could not release rendering context.");
		else
			errlog.gLogOut("	Released rendering context.");
		
		hrc=NULL;		
	}

	if(hdc && !ReleaseDC(hwnd,hdc))	//Release the device context
	{
		errlog.gLogOut("	Could not release the graphics device context.");
		hdc=NULL;				
	}
	else
		errlog.gLogOut("	Released the Graphics Device Context.");

	if(hwnd && !DestroyWindow(hwnd))	//Release the main window handle
	{
		errlog.gLogOut("	Could not release main window handle.");
		hwnd=NULL;					
	}
	else
		errlog.gLogOut("	Released Main Window Handle.");

	if(!UnregisterClass(CLASSNAME,hinstance))	//Unregister the window's class
	{
		errlog.gLogOut("	Could not unregister window class.");	
		hinstance=NULL;			
	}
	else
		errlog.gLogOut("	Unregistered window class.");
}

//------------------------------------------------------------------//
//- GLvoid g_util:: Resize_GLScene(GLsizei, GLsizei) ------------//
//------------------------------------------------------------------//
//- Description: This function resizes the OpenGL SCENE (not the   -//
//-              window) so that the same proportions in the scene -//
//-              are the same.									   -//
//------------------------------------------------------------------//
GLvoid reshapeFunc(GLsizei width, GLsizei height)
{
	if(height==0)										//Prevent a divide by zero (bad)
		height=1;										//Making height equal one

	glViewport(0,0,width,height);						//Reset the current viewport

	glMatrixMode(GL_PROJECTION);						//Select the projection matrix
	glLoadIdentity();									//Reset the projection matrix

	//Calculate the aspect ratio of the window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							//Select the modelview matrix
	glLoadIdentity();									//Reset The modelview matrix
}

//------------------------------------------------------------------//
//- void g_util:: HandleMessages(void) --------------------------//
//------------------------------------------------------------------//
//- Description: This function handles all of the necessary        -//
//-              messages that your window will recieve. This      -//
//-              function is more of a 'space-saver' for to reduce -//
//-              the amount of stuff you have to look through in   -//
//-              your console.									   -//
//------------------------------------------------------------------//
bool g_util::eventHandler(void)
{
	MSG msg;

	//These are functions that are needed to process the messages
	//that the window is recieving.
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{ 
	    TranslateMessage(&msg);
	    DispatchMessage(&msg); 

		if(msg.message==WM_QUIT)
			return false;
	}
	return true;
}

//------------------------------------------------------------------//
//- LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) --------//
//------------------------------------------------------------------//
//- Description: This function handles all of the specific messages-//
//-              that the window will be recieving throughout it's -//
//-              life.											   -//
//------------------------------------------------------------------//
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{	
		case WM_DESTROY:		//If the user is pressing esc, then quit
			PostQuitMessage(0);
			return false;
			break;
	
		case WM_SYSCOMMAND:
			switch(lparam)
			{
				case SC_SCREENSAVE:		//Is the computer trying to get a screen saver going?
				case SC_MONITORPOWER:	//Does the computer want to go to sleep? (I know I do :) )
				default: break;
			}
			break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wparam] = TRUE;					// If So, Mark It As TRUE
			
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wparam] = FALSE;					// If So, Mark It As FALSE
			
			return 0;								// Jump Back
		}

		case WM_CLOSE:				//Does the user want to close?
			PostQuitMessage(0);		//Send a quit message to windows
			return false;
			break;

		case WM_SIZE:				//Resize the OpenGL window (and then resize the current scene to fit
			reshapeFunc(LOWORD(lparam),HIWORD(lparam));  // LOWORD=width, HIWORD=height
			return false;
			break;

		default:break; 
	} 

	return DefWindowProc(hwnd, msg, wparam, lparam);
}


//------------------------------------------------------------------//
//- GLvoid g_util::Font_Init(GLvoid) ----------------------------//
//------------------------------------------------------------------//
//- Description: This function initiates the font engine. Just put -//
//-              a call to this function in the initiation part of -//
//-              your program.									   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-             Font_Init();									   -//
//------------------------------------------------------------------//
GLvoid g_util::initFont(GLvoid)	
{
	int loop;
	float cx=0;
	float cy=0;

	loadTGA(&font_tex, "Art/Font.tga", 
			 GL_LINEAR, GL_LINEAR);

	base=glGenLists(95);						//Creating a display list that can hold 95
	glBindTexture(GL_TEXTURE_2D, font_tex.ID);//Bind our font texture
	for(loop=0; loop<95; loop++)				//Loop through the display list
	{
		cx=float(loop%16)/16.0f;				//X position of current character
		cy=float(loop/16)/8.0f;					//Y position of current character

		glNewList(base+loop,GL_COMPILE);		//Start building a list
			glBegin(GL_TRIANGLE_STRIP);			//Use a quad for each character
				//Top right (tex/vertex) coordinates
				glTexCoord2f((float)(cx+0.0815f), (float)(1.0-cy));		
				glVertex2i(16,16);
				//Top left (tex/vertex) coordinates
				glTexCoord2f((float)(cx),		  (float)(1.0-cy));	
				glVertex2i(0,16);
				//Bottom right (tex/vertex) coordinates
				glTexCoord2f((float)(cx+0.0815f), (float)(1.0-cy-0.120)); 
				glVertex2i(16,0);
				//Bottom left (tex/vertex) coordinates
				glTexCoord2f((float)(cx),		  (float)(1.0-cy-0.120)); 
				glVertex2i(0,0);
			glEnd();							//Done Building Our Quad (Character)
			glTranslated(10,0,0);				//Move To The Right Of The Character
		glEndList();							//Done Building The Display List
	}															// Loop Until All 256 Are Built
}


//------------------------------------------------------------------//
//- GLvoid g_util:: Font_Shutdown(GLvoid) -----------------------//
//------------------------------------------------------------------//
//- Description: This function shuts down the font engine.  Put a  -//
//-              call to this in the shutdown part of your program.-//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-             Font_Shutdown();								   -//
//------------------------------------------------------------------//
GLvoid g_util::shutdownFont(GLvoid)		
{ 
	glDeleteLists(base, 96); 
}

//------------------------------------------------------------------//
//- GLvoid Lucas3D::glPrint(GLint, GLint, const char*, ...) ------//
//------------------------------------------------------------------//
//- Description: This function prints a string of text onto the    -//
//-				 the screen.  You *must* be in ortho view to use   -//
//-              this function, but thats not a bad thing, it makes-//
//-              positioning the text easier.  Also, this function -//
//-              functions (hehehe) almost exactly like printf(...)-//
//-              in dos.										   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-             glPrint(120, 120, "Hi, I am %d years old", 16);	   -//
//------------------------------------------------------------------//
GLvoid g_util::glPrint( GLint x, GLint y, const char *string, ... )
{
	char		text[256];										// Holds Our String
	va_list		va;												// Pointer To List Of Arguments

	if(string==NULL)											// If There's No Text
		return;													// Do Nothing

	va_start(va, string);										// Parses The String For Variables
	    vsprintf(text, string, va);								// And Converts Symbols To Actual Numbers
	va_end(va);													// Results Are Stored In Text

	glBindTexture(GL_TEXTURE_2D, font_tex.ID);			// Select Our Font Texture

	glPushMatrix();												// Store The Modelview Matrix
	glLoadIdentity();											// Reset The Modelview Matrix
	glTranslated(x,y,0);										// Position The Text (0,0 - Bottom Left)
	glListBase(base-32);										// Choose The Font Set
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);			// Draws The Display List Text
	glPopMatrix();												// Restore The Old Projection Matrix
}

//------------------------------------------------------------------//
//- GLvoid LOG::Init() ---------------------------------------------//
//------------------------------------------------------------------//
//- Description: Don't even bother using this function at all, it  -//
//-              initiates the output log, but I put this function -//
//-              in the log's constructor, so it's automatically   -//
//-              called.										   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-             Don't even use it.								   -//
//------------------------------------------------------------------//
bool g_log::Init(void)	
{
	//Clear the log contents
	if((logfile=fopen("Error Log.txt", "wb"))==NULL)
		return false;

	//Close the file, and return a success!
	fclose(logfile);
	return true;
}
//------------------------------------------------------------------//
//- GLvoid LOG::Init() ---------------------------------------------//
//------------------------------------------------------------------//
//- Description: Don't even bother using this function at all, it  -//
//-              shuts down the output log, but I put this function-//
//-              in the log's destructor, so it's automatically    -//
//-              called.										   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-             Don't even use it.								   -//
//------------------------------------------------------------------//
bool g_log::Shutdown(void)
{
	if(logfile)
		fclose(logfile);

	return true;
}

//------------------------------------------------------------------//
//- GLvoid LOG::gLogOut() -------------------------------------------//
//------------------------------------------------------------------//
//- Description: This function outputs text the error log, the     -//
//-              function functions (I won't even say "hehehe" this-//
//-              time) exactly like printf(...).				   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-             errlog.gLogOut("Hi bob");						   -//
//------------------------------------------------------------------//
bool g_log::gLogOut(char* text, ...)
{
	va_list arg_list;

	//Initialize variable argument list
	va_start(arg_list, text);

	//Open the log file for append
	if((logfile = fopen("3d_log.txt", "a+"))==NULL)
		return false;

	//Write the text and a newline
	vfprintf(logfile, text, arg_list);
	putc('\n', logfile);

	//Close the file
	fclose(logfile);
	va_end(arg_list);

	return true;
}


//------------------------------------------------------------------//
//- BOOL TGA_Load(TGA_PTR, char*) ----------------------------------//
//------------------------------------------------------------------//
//- Description: This function loads a TGA image.  You must first  -//
//-              create a S3DTGA object, and then pass it's address-//
//-              as this function's first argument.  Then you must -//
//-              provide the tga's filename.  Last, but not least, -//
//-              you need to provide the texture's filter (using   -//
//-              the regular OpenGL filter constants). The function-//
//-              returns true if everything went ok. The function  -//
//-              also writes a status to the error log to tell you -//
//-              exactly what went wrong, or if nothing did go     -//
//-              wrong, exactly what file was loaded.			   -//
//------------------------------------------------------------------//
//- Sample use:													   -//
//-              S3DTGA tga;									   -//
//-              TGA_Load(&tga, "Art/mytga.tga",				   -//
//-                       GL_LINEAR, GL_LINEAR);                   -//
//------------------------------------------------------------------//
bool loadTGA(g_tga_t* image, char* filename,
			  GLfloat minfilter, GLfloat maxfilter)	
{    
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	//Uncompressed TGA header
	GLubyte		TGAcompare[12];								//Used to compare TGA header
	GLubyte		header[6];									//The first six useful bytes from the header
	GLuint		bytesPerPixel;								//Holds the bpp of the TGA
	GLuint		imageSize;									//Used to store image size while in RAM
	GLuint		temp;										//Temp variable
	GLuint		type=GL_RGBA;								//Set the default OpenGL mode to RBGA (32 BPP)

	FILE* file = fopen(filename, "rb");						// Open The TGA File

	if(file==NULL													   ||	// Does File Even Exist?
	   fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
	   memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0			   ||	// Does The Header Match What We Want?
	   fread(header,1,sizeof(header),file)!=sizeof(header))					// If So Read Next 6 Header Bytes
	{
		if(file==NULL)									// Did The File Even Exist? *Added Jim Strong*
		{
			errlog.gLogOut("%s does not exist.", filename);
			return false;							
		}
		else
		{
			fclose(file);						// If anything failed, close the file
			errlog.gLogOut("Could not load %s correctly, general failure.", filename);
			return false;						
		}
	}
	image->Width  = header[1] * 256 + header[0];		// Determine The TGA Width	(highbyte*256+lowbyte)
	image->Height = header[3] * 256 + header[2];		// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(image->Width	<=0	||								// Is The Width Less Than Or Equal To Zero
	   image->Height<=0	||								// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))				// Is The TGA 24 or 32 Bit?
	{
		fclose(file);									// If Anything Failed, Close The File
		errlog.gLogOut("%s's height or width is less than zero, or the TGA is not 24 or 32 bits.", filename);
		return false;							
	}

	image->BPP		= header[4];						// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= image->BPP/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= image->Width * image->Height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	image->Data=(GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if(image->Data==NULL ||							// Does The Storage Memory Exist?
	   fread(image->Data, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(image->Data!=NULL)						// Was Image Data Loaded
			free(image->Data);						// If So, Release The Image Data
		
		errlog.gLogOut("Storate memory for %s does not exist or is corrupted.", filename);
		
		fclose(file);								// Close The File
		return false;								// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{										// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=image->Data[i];					// Temporarily Store The Value At Image Data 'i'
		image->Data[i] = image->Data[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		image->Data[i + 2] = temp;				// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);								//Close the file

	// Build A Texture From The Data
	glGenTextures(1, &image->ID);				//Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, image->ID);			//Bind the texture to a texture object 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);	//Filtering for if texture is bigger than should be
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxfilter);	//Filtering for if texture is smaller than it should be
	
	if(image->BPP==24)							//Was the TGA 24 bpp?
		type=GL_RGB;							

	glTexImage2D(GL_TEXTURE_2D, 0, type, image->Width, image->Height, 0, type, GL_UNSIGNED_BYTE, image->Data);

	errlog.gLogOut("Loaded %s correctly.", filename);
	return true;
}




