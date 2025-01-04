/*
	Coded mostly by Lucas and Colby McLane, borrowed some code from Brian Pollack- The Shining Knight himself.
	Checkout http://nehe.gamedev.net for great OpenGL tips.
	C++ wrapper class for DirectInput, DirectSound, Windows API, and OpenGL
	g_util.h
*/

#ifndef g_util_h
#define g_util_h


#define WIN32_NON_PORTABLE	// Not a POSIX-compliant game... yet
#define INIT_GUID

//HEADERS:

#include <windows.h>		
#include <windowsx.h>		
#include <mmsystem.h>
#include <iostream>
#include <dinput.h>
#include <dmusicc.h>
//#include <dmusici.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include <conio.h>		
#include <math.h>
#include <time.h>
#include <io.h>	
#include <gl\gl.h>			
#include <gl\glu.h>	
#include <gl\glaux.h>
#include "glut.h"		
#include "image.h"
#include "coldet.h"


//PRAGMAS:
   
/* Cool tricks to search for libs at precompile:  */
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Dxguid.lib")	
#pragma comment(lib, "Dinput8.lib")	
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "Glu32.lib")	
#pragma comment(lib, "Coldet.lib")

/* Ways to hush noisy useless warnings... :)      */
#pragma warning (disable:4244)   
#pragma warning (disable:4305)  


//CONSTANTS:
#define GL_CLAMP_TO_EDGE	0x812F

#define WINDOWED		1
#define FULLSCREEN		0
#define FULLSCREEN_OFF	1
#define FULLSCREEN_ON	0
#define ASK_FULLSCREEN  2
#define TITLE			"g_util"
#define CLASSNAME		"g_util"
#define SCR_WIDTH		800
#define SCR_HEIGHT		600
#define SCR_BPP			32

#define MOUSE_LEFT		0
#define MOUSE_RIGHT		1
#define MOUSE_MIDDLE	2

#define PI_OVER_180		0.0174532925f
#define piover180		0.0174532925f
#define PI				3.141592654f
#define VECTOR			VECTOR
#define BITMAP_ID		0x4D42

//game specific constants
#define delta_fuel		 1
#define fuel_height		 0.5
#define max_fuel		 100
#define fuel_increment	 10
#define max_block		 60
#define max_life		 500

#define MAX_ENEMIES	5
#define MAX_FUEL	10

/* MD2 constants */
#define MAX_TRIANGLES 4096
#define MAX_VERTICES  2048
#define MAX_TEXCOORDS 2048
#define MAX_FRAMES    512

#define CHUNK1 0
#define CHUNK2 1
#define CHUNK3 2
#define CHUNK4 3

/* MD2 state constants */
#define ANIMATE_CURRENT -100

#define IDLE1				0
#define RUN					1
#define SHOT_STAND			2
#define SHOT_SHOULDER		3
#define JUMP				4
#define IDLE2				5
#define SHOT_FALLDOWN		6
#define IDLE3				7
#define IDLE4				8
#define CROUCH				9
#define CROUCH_CRAWL		10
#define CROUCH_IDLE			11
#define CROUCH_DEATH		12
#define DEATH_FALLBACK		13
#define DEATH_FALLFORWARD	14
#define DEATH_FALLBACKSLOW	15

#define IDLE1_START					0
#define IDLE1_END					39
#define RUN_START					40
#define RUN_END						45
#define SHOT_STAND_START			46
#define SHOT_STAND_END				60
#define SHOT_SHOULDER_START			61
#define SHOT_SHOULDER_END			66
#define JUMP_START					67
#define JUMP_END					73
#define IDLE2_START					74
#define IDLE2_END					95
#define SHOT_FALLDOWN_START			96
#define SHOT_FALLDOWN_END			112
#define IDLE3_START					113
#define IDLE3_END					122
#define IDLE4_START					123
#define IDLE4_END					135
#define CROUCH_START				136
#define CROUCH_END					154
#define CROUCH_CRAWL_START			155
#define CROUCH_CRAWL_END			161
#define CROUCH_IDLE_START			162
#define CROUCH_IDLE_END				169
#define CROUCH_DEATH_START			170
#define CROUCH_DEATH_END			177
#define DEATH_FALLBACK_START		178
#define DEATH_FALLBACK_END			185
#define DEATH_FALLFORWARD_START		186
#define DEATH_FALLFORWARD_END		190
#define DEATH_FALLBACKSLOW_START	191
#define DEATH_FALLBACKSLOW_END		198

/* MS3D constants */
#define MS3D_TEXTURE_OFF false
#define MS3D_TEXTURE_ON  true


/* Particle Constants */
#define MAX_PARTICLES 500

#define TYPE_BILLBOARD   0
#define TYPE_MESH	     1
#define TYPE_ONLY_UPDATE 2

#define ONLY_CREATE		  0
#define ONLY_UPDATE		  1
#define UPDATE_AND_CREATE 2

#define DEATH_AGE  0.0f

#define ATTRACTION 0
#define STOP       1

#define GET_X 0
#define GET_Y 1
#define GET_Z 2

#define SK_OFF false
#define SK_ON  true

#define MIN_SPEED	 0.0f
#define MIN_SIZE     0.1f
#define MIN_LIFETIME 0.01f
#define MIN_SPREAD	 0.01f
#define MIN_EMISSION 1.0f
#define MIN_GRAVITY -10.0f
#define MIN_ALPHA	 0.0f
#define MIN_COLOR	 0.0f

#define MAX_SPEED	 300.0f
#define MAX_SIZE     100.0f
#define MAX_LIFETIME 20.0f
#define MAX_SPREAD   180.0f
#define MAX_EMISSION 1000
#define MAX_GRAVITY  10.0f
#define MAX_ALPHA	 1.0f
#define MAX_COLOR	 1.0f

extern int block[20][20];



//STRUCTURES:

typedef struct g_tga_t
{
	GLuint	 ID;	
	GLuint	 Width;		
	GLuint	 Height;
  GLubyte*   Data;		// 32 bit image data
	GLuint	 BPP;		// bits per pixel						
} g_tga, *g_tga_ptr;	


typedef struct g_quad_t
{
	// Vertices in 3-Space
	GLfloat tl[3];
	GLfloat tr[3];
	GLfloat bl[3];
	GLfloat br[3];
}g_quad, *g_quad_ptr;	


/* MD2 Quake2 Model Structures */
typedef struct MD2_HEADER_TYP
{
	int magic;				//The 'magic' number (always 844121161)
	int version;			//The version number of the file (always 8)
	int skinWidth;			//The width of the model's skin
	int skinHeight;		//The height of the model's skin
	int frameSize;			//The size of each frame (in BYTEs)
	int numSkins;			//The model's number of skins
	int numBertices;		//The model's number of vertices
	int numTexcoords;		//The model's number of texture coordinates (most likely, its not the same number as the vertices)
	int numTriangles;		//The number of triangles that make up the model
	int numGlCommands;		//The number of DWORDs (4 bytes) in the GLcommand list (which decide to render the model with tri strips, or fans)
	int numFrames;			//The number of frames (of animation) that the model has
	int offsetSkins;		//Offset in the file of the model, to where the skin information is
	int offsetTexcoords;	//Offset in the file of the model, to where the texture coordinate information is
	int offsetTriangles;	//Offset in the file of the model, to where the traingle information is
	int offsetFrames;		//Offset in the file of the model, to where the first frame information is given
	int offsetGlCommands;	//Offset in the file of the model, to where the GLcommand information is
	int offsetEnd;			//Offset in the file of the model, to where the end of it is
} MD2_HEADER, *MD2_HEADER_PTR;

typedef struct MD2_VERTEX_TYP
{
	unsigned char vertex[3];		//Scaled version of the model's 'real' vertex coordinate
	unsigned char lightNormalIndex;	//An index into the table of normals, kept by Quake 2
} MD2_VERTEX, *MD2_VERTEX_PTR;

typedef struct MD2_FRAME_TYP
{
	float	   scale[3];		//The scale used by the model's 'fake' vertex structure
	float	   translate[3];	//The translation used by the model's 'fake' vertex structure
	char	   name[16];		//The name for the frame
	MD2_VERTEX vertices[1];		//An array of MD2_VERTEX structures
} MD2_FRAME, *MD2_FRAME_PTR;

typedef struct MD2_MODELVERTEX_TYP
{
	float x,y,z;				//The (x,y,z) location of the vertex
	float u,v;					//The (u,v) location of the texture coordinate
} MD2_MODELVERTEX, *MD2_MODELVERTEX_PTR;


/* MS3D model structures */
typedef struct MS3D_HEADER_TYP
{
    char id[10];				//The ID number of the model (always "MS3D000000")
	int  version;				//The version number of the model
} MS3D_HEADER, *MS3D_HEADER_PTR;

typedef struct MS3D_VERTEX_TYP
{
	unsigned char flags;
	unsigned char refCount;
	char boneID;				//The bone ID (used for skeletal animation)
	float vertex[3];
} MS3D_VERTEX, *MS3D_VERTEX_PTR;

typedef struct MS3D_TRIANGLE_TYP
{
	unsigned short flags;
	unsigned short vertexIndices[3];
	float vertexNormals[3][3];
	float u[3];
	float v[3];
	unsigned char smoothingGroup;
	unsigned char groupIndex;
} MS3D_TRIANGLE, *MS3D_TRIANGLE_PTR;

typedef struct MS3D_GROUP_TYP
{
    unsigned char	flags;
    char			name[32];
    unsigned short	numTriangles;
	unsigned short*	triangleIndices;
	char			materialIndex;
} MS3D_GROUP, *MS3D_GROUP_PTR;

typedef struct MS3D_TEXTURE_TYP
{
    char  name[32];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emissive[4];
    float shininess;
    float transparency;
    char  mode;
    char  alphaMap[128];
} MS3D_TEXTURE, *MS3D_TEXTURE_PTR;

typedef struct MS3D_JOINT_TYP
{
	unsigned char  flags;
	char		   name[32];
	char		   parentName[32];
	float		   rotation[3];
	float		   translation[3];
	unsigned short numRotationKeyframes;
	unsigned short numTranslationKeyframes;
} MS3D_JOINT, *MS3D_JOINT_PTR;


// CLASSES:

class g_util
{
	public: 
		 int scr_width;
		 int scr_height;
		 int scr_bpp;
		bool fullscreen;

		g_util(){	trigInit();	}
	   ~g_util(){				}
		bool eventHandler(void);
		void shutdownAll(void);
		bool initAll( int w, int h, int bpp, int scr_flag );
		GLvoid glPrint( GLint x, GLint y, const char *string, ... );	
		GLvoid initFont( GLvoid );
		GLvoid shutdownFont( GLvoid );

	private:
		g_tga font_tex;
		GLuint base;
		GLvoid trigInit( GLvoid );
};

class g_util_input
{
	public:
		bool initDI( void );
		void shutdownDI( void );
		void updateDI( void );

		g_util_input()
		{	}
	   ~g_util_input()
		{	}

	private:
		LPDIRECTINPUT8       lpdi;			//DirectInput parent
		LPDIRECTINPUTDEVICE8 lpdi_keyboard; //DirectInput for keyboard
		LPDIRECTINPUTDEVICE8 lpdiMouse;
};

class g_log
{
	public:
		bool gLogOut(char* text, ...);

	    g_log()
		{	
			Init();	
		}

	   ~g_log()
		{	
		   Shutdown();	
		}

	private:
		bool Init(void);
		bool Shutdown(void);
		
		FILE* logfile;
};

class VECTOR
{
	public:
		float vector[3];

	float ComputeLength(void);

	VECTOR inline operator+ (const VECTOR &v);
	VECTOR inline operator- (const VECTOR &v);
	VECTOR inline operator* (const float scalar);
	VECTOR inline operator* (const VECTOR &v);
	VECTOR inline operator/ (const VECTOR &v);

	VECTOR()
	{	
		memset(vector, 0, sizeof(float[3]));	
	}
	VECTOR(float x, float y, float z);
};

//------------------------------------------------------------------//
//- class VECTOR -------------------------------------------------//
//------------------------------------------------------------------//
//- A class for all three dimensional vectors.  This will have some-//
//- very nice features so that we can add/multiply/subtract/divide -//
//- vectors together now! (Also used for vertices)				   -//
//------------------------------------------------------------------//
class VERTEX
{
	public:
		float vertex[3];

	VERTEX inline operator+ (const VERTEX &v);
	VERTEX inline operator- (const VERTEX &v);
	VERTEX inline operator* (const float scalar);
	VERTEX inline operator* (const VERTEX &v);
	VERTEX inline operator/ (const VERTEX &v);

	void SendToOGL(void);

	VERTEX()
	{	
		memset(vertex, 0, sizeof(float[3]));	
	}
	VERTEX(float x, float y, float z);
};

//------------------------------------------------------------------//
//- class VECTOR -------------------------------------------------//
//------------------------------------------------------------------//
//- A class for 4x4 matrices.									   -//
//------------------------------------------------------------------//
class MATRIX4X4
{
	public:
		float matrix[16];

	void LoadZero(void);
	void LoadIdentity(void);

	friend MATRIX4X4 operator+ (const MATRIX4X4 &m1, const MATRIX4X4 &m2);
	friend MATRIX4X4 operator- (const MATRIX4X4 &m1, const MATRIX4X4 &m2);
	friend MATRIX4X4 operator* (const MATRIX4X4 &m1, const float scalar);
	friend MATRIX4X4 operator* (const MATRIX4X4 &m1, const MATRIX4X4 &m2);

	MATRIX4X4()
	{	
		memset(matrix, 0, sizeof(float[16]));
	}
};

class TEXTURE
{
	public:
		GLubyte* data;			//Image data (up to 32 bits)
		unsigned int bpp;		//Image color depth in bits per pixel
		unsigned int width;		//Image width
		unsigned int height;	//Image height
		unsigned int ID;		//Texture ID used to select a texture

	bool LoadTGA(char* filename, GLfloat minFilter, GLfloat maxFilter);
	bool LoadBMP(char* filename, GLfloat minFilter, GLfloat maxFilter);

	TEXTURE()
	{	}

	~TEXTURE()
	{	
		if(data)
		{
			glDeleteTextures(1, &ID);
			delete data;
		}	
	}
};


class MS3D
{
	public:
		unsigned short numVertices;
		MS3D_VERTEX*   vertices;
		unsigned short numTriangles;
		MS3D_TRIANGLE* triangles;
		unsigned short numGroups;
		MS3D_GROUP*	   groups;
		unsigned short numTextures;
		MS3D_TEXTURE*  textures;

	bool Load(char* filename);
	void Render(bool texture_flag);

	MS3D()
	{	}

	~MS3D()
	{	
		if(vertices)
			delete[] vertices;		
		if(triangles)
			delete[] triangles;
		if(groups)
			delete[] groups;
		if(textures)
			delete[] textures;
	}
};


//------------------------------------------------------------------//
//- class MD2 ------------------------------------------------------//
//------------------------------------------------------------------//
//- A class for the Quake 2 model mesh (includes key frames for    -//
//- animation).													   -//
//------------------------------------------------------------------//
class MD2
{
	private:
		int numGlCommands;
		long* glCommands;

		int numTriangles;

	public:
		int stateStart;
		int stateEnd;

		int frameSize;
		int numFrames;
		char* frames;

		int currentFrame;
		int nextFrame;
		int endFrame;
		float interpolation;
	
	bool Load(char* filename);
	void Render(int numFrame);
	void Animate(int startFrame, int EndFrame, float Interpolation);
	void SetState(int state);

	MD2() : stateStart(RUN_START), stateEnd(RUN_END),
	//stateStart(IDLE1_START), stateEnd(IDLE1_END),
			numGlCommands(0), frameSize(0), numFrames(0), 
			currentFrame(RUN_START), nextFrame(currentFrame+1),
			endFrame(RUN_END), interpolation(0.0f)
	//		currentFrame(RUN_START), nextFrame(currentFrame+1),
	//		endFrame(RUN_END), interpolation(0.0f)
	{	}

	~MD2()
	{	
		if(glCommands)
			delete[] glCommands;
		if(frames)
			delete[] frames;
	}
};

/* Particle classes */
class PARTICLE;
class PARTICLE_SYSTEM;

class PARTICLE
{
	private:
		PARTICLE_SYSTEM* Parent;

	public:
		VERTEX prevLocation;	//The particle's last position
		VERTEX location;		//The particle's current position
		VECTOR velocity;		//The particle's current velocity

		unsigned int texture;

		float color[4];			//The particle's color
		float colorCounter[4];	//The color counter!

		float alpha;			//The particle's current transparency
		float alphaCounter;		//Adds/subtracts transparency over time

		float size;				//The particle's current size
		float sizeCounter;		//Adds/subtracts transparency over time

		float age;				//The particle's current age
		float dyingAge;			//The age at which the particle DIES!

	void SetParentSystem(PARTICLE_SYSTEM* parent);
	void Create(PARTICLE_SYSTEM* parent, float timeCounter);
	bool Update(float timeCounter);

	PARTICLE();
	~PARTICLE();
};



//------------------------------------------------------------------//
//- class PARTICLE_SYSTEM ------------------------------------------//
//------------------------------------------------------------------//
//- A class that contains all of the information for the Particle  -//
//- Engine.  Defined in Particle.cpp.							   -//
//------------------------------------------------------------------//
class PARTICLE_SYSTEM
{
	private:
		bool attracting;			//Is the system attracting particle towards itself?
		bool stopped;				//Have the particles stopped emitting?

		unsigned int particlesPerSec;	//Particles emitted per second
		unsigned int particlesNumbAlive;//The number of particles currently alive
		
		float age;					//The system's current age (in seconds)
		
		float lastUpdate;			//The last time the system was updated

		float emissionResidue;		//Helps emit very precise amounts of particles


	public:
		PARTICLE particle[MAX_PARTICLES];//All of our particles

		VERTEX prevLocation;		//The last known location of the system
		VERTEX location;			//The current known position of the system
		VECTOR velocity;			//The current known velocity of the system

		unsigned int texture;		//The particle's texture

		float startSize;			//The starting size of the particles
		float sizeCounter;			//Adds/subtracts particle size over time
		float endSize;				//The particle's end size (used for a MAX boundry)

		float startAlpha;			//The starting transparency of the particle
		float alphaCounter;			//Adds/subtracts particle's transparency over time
		float endAlpha;				//The end transparency (used for a MAX boundry)

		VECTOR startColor;			//The starting color
		VECTOR colorCounter;		//The color that we interpolate over time
		VECTOR endColor;			//The ending color

		float speed;				//The system's speed
		float speedCounter;			//The system's speed counter

		float life;					//The system's life (in seconds)
		float lifeCounter;			//The system's life counter

		float angle;				//System's angle (90==1/2 sphere, 180==full sphere)

		int spreadMin;				//Used for random positioning around the emitter
		int spreadMax;
		float spreadFactor;			//Used to divide spread

		VECTOR gravity;				//Gravity for the X, Y, and Z axis
		float attractionPercent;

	/* Necessary functions */
	bool Update(float time, int flag, float numToCreate);
	void Render(void);

	
	/* Informational functions */
	unsigned int ActiveParticles(void);
	float GetLocation(int coordinate);
	bool IsAttracting(void)
	{	
		return attracting;	
	}
	bool IsStopped(void)
	{	
		return stopped;	
	}

	/* Customization functions */
	void SetLocation(float x, float y, float z);
	void SetTexture(unsigned int textureID);
	void SetParticlesPerSec(unsigned int number);
	void SetVelocity(float xv, float yv, float zv);
	void SetSize(float StartSize, float EndSize);
	void SetAlpha(float StartAlpha, float EndAlpha);
	void SetSpeed(float Speed);
	void SetAngle(float halfAngle);
	void SetSystemFlag(int flag, bool state);
	void SetColor(float startRed, float startGreen, float startBlue, float endRed, float endGreen, float endBlue);
	void SetLife(float seconds);
	void SetSpread(int SpreadMin, int SpreadMax, float SpreadFactor);
	void SetAttraction(unsigned int Attraction_Percent);
	void SetGravity(float xpull, float ypull, float zpull);

	PARTICLE_SYSTEM();
	~PARTICLE_SYSTEM();
};


// EXTERNS (rename these):

extern CollisionModel3D* model_robot;
extern CollisionModel3D* model_player;

extern HINSTANCE hinstance; //Handle to the window's instance
extern HWND  hwnd;			//Handle to the actual window
extern HDC   hdc;
extern HGLRC hrc;

extern UCHAR key_buffer[256];//The keyboard's key buffer
extern bool keys[256];
extern int first_time;

extern GLfloat COS[720];
extern GLfloat SIN[720];
extern DIMOUSESTATE  mouseState;	//The mouse's 'buffer'
extern g_log errlog;

#define KEY_DOWN(key) (key_buffer[key] & 0x80)	// Isssssssssssssssss key dddown??
#define MOUSE_DOWN(button) (mouseState.rgbButtons[button] & 0x80)

//Andrè's macros. Thanks a lot.  
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (b) : (a))
#define RANDOM_FLOAT (((float)rand()-(float)rand())/RAND_MAX)
#define CHECK_RANGE(x,min,max) (x = (x<min  ? min : x<max ? x : max))
#define DEG_TO_RAD(angle)  ((angle)*PI/180)
#define RAD_TO_DEG(radians) ((radians)*180/PI)
#define SQUARE(number) (number*number)


//PROTOTYPES:

extern LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
extern bool loadTGA( g_tga_ptr image, char* filename, GLfloat min_filter, GLfloat max_filter );
extern void reshapeFunc( GLsizei width, GLsizei height );
extern int pickaRandom( int min, int max );

extern void ComputeNormal(VECTOR* v1, VECTOR* v2, VECTOR* v3);
extern float ComputeDotProduct(VECTOR* v1, VECTOR* v2);
extern VECTOR ComputeCrossProduct(VECTOR* v1, VECTOR* v2);



//other prototypes
extern void update( void );
extern void update_fuel( int row, int col );
extern void update_Enemies( void );
extern void idle( void );									
extern void reshape ( void ) ;						
extern void keyboard( unsigned char key, int x, int y );	
extern void init( void );									
extern void display( void );
extern void spawn_enemy( int x );
extern void place_fuel( int fuel_index );
extern void make_Maze(void);
extern void make_Enemy( int x );


#endif // g_util.h