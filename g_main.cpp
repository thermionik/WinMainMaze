// g_main.cpp


#include "g_util.h"
#include "g_player.h"
#include "fmod.h"
#include "fmod_errors.h"    /* optional */

float xx, yy, zz, aa;


bool SphereInFrustum( float x, float y, float z, float radius );
void ExtractFrustum(void);
// Globals

bool	active		= true;	// Window Active Flag Set To TRUE By Default
bool	done		= false;// Bool Variable To Exit Loop	
bool	fullscreen	= true;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool	lights_on = false;  // Lighting ON / OFF
bool	tex_on		= true;
bool	music_on	= false;
bool n_key = false, b_key = true;
int flag = 0;
int quad = 0;
g_fuel FuelCan[MAX_FUEL];
g_framerate FrameRate;
FMUSIC_MODULE *mod = 0;

float frustum[6][4];

static FLOAT fps      = 0.0f;
static FLOAT last_time= 0.0f;
static int frames   = 0;

//int Level_Zero[60][60];


// RPG Level 0...
/*
int Level_Zero[60][60] = {
		 5, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5,77,77, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,77,77,77, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 5,77,77, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 4, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 5, 5, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 5, 5, 5, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 5, 5, 5, 0, 0, 5, 0, 5, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 5, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 5, 5, 0, 0, 5, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 0, 5, 5, 5, 5, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 5, 0, 0, 5, 5, 5, 5, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 5, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 5, 5, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 5, 0, 5, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 5, 5, 5, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
}; */

int Level_Zero[60][60] = {
		0, 0,10,10, 0, 0,10, 0,10,10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,10,10, 0, 0,10,10,10,10,10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		0, 0, 0,10,10,10,10, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0,10, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		0, 0, 0, 0, 0, 0, 0,10,10, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		0, 0, 0,10, 0, 0, 0, 0, 0,10, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 5, 0,10, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10, 0, 0, 0,10, 0, 0, 0,10, 0, 0, 0, 0, 0, 5,77,77,77, 5, 0, 0, 0, 5, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,77,77,77, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,77, 5,77,77,77, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10, 0,10, 0, 0, 0,10, 0, 0, 0, 5, 0, 0, 0, 5,77,77,77, 5, 0, 0, 0, 5, 0, 0, 0,10,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,77,77,77,77,77, 5, 4, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 5,
		10, 0, 0,10, 4, 0,10, 0, 0,10, 5, 0, 0, 0, 5,77, 5,77, 5, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,77, 5,77,77,77, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10, 0, 0,10, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5,77,77,77, 0, 0, 5, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 5, 5,77, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10,10, 0, 0, 0, 4, 0, 0, 0, 0, 5, 0, 5, 0, 0, 0, 5, 5, 5, 5, 5, 0, 5, 0,10, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0,10, 0, 5, 5, 5,77,77,77, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10, 0, 0,10,10, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,77,77, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0,77,77,77,77,77, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 5, 5, 5, 5, 5, 5, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 5,
		 0,10, 0, 0, 0, 0, 0,10, 0, 0,10,10,10, 5, 0, 0, 0, 0, 5,10,10,10,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 0,10, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 5,
		 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 0, 0,10, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10, 0,10,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 0, 0, 0,10, 0, 0, 4, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		10, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0,10, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 6, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 5, 5, 5, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,10, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 0,10, 5, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0,10, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0,10,10,10, 0, 0, 5,
		 5, 0, 0, 5, 5, 5, 5, 5,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0,10, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0,10, 0, 5,
		 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0,10, 5,
		 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0,10,
		 5, 5, 5, 5, 5, 0, 0, 0, 0, 5,10,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0,10,
		 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0,10, 0, 0, 0, 0,10,
		 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0,10, 0, 0, 0,10,
		 5, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0,10, 0, 0,10,
		 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,
		 5, 5, 0, 5, 5,10, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,
		 5, 0, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 5,
		 5, 0, 5, 5, 5, 5, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 5,
		 5, 0, 0, 0, 0, 5, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,10,10,10, 0, 0, 5,
		 5, 5, 5, 5, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 5, 5, 0, 0, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 5, 0, 0, 5, 0,10, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 5, 5, 5, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0,10, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 5, 0, 5, 5, 5, 5, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 5, 5, 5, 0, 5, 0, 0, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 5, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 5, 5, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 5, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
		 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
}; 

RECT	window;
BOOL	collision = FALSE;
BOOL	detected_yet = FALSE;
GLint	first_time = 1;
GLint   Skybox;				// The sky cube call list identifier

GLint	clouds;
GLint	life = 500;
GLint	max_maze_fuel = 5;
GLint	num_maze_fuel = 0;
GLint	speed = 80;			// Smaller is faster

GLint	sky_y = 0;
GLuint	filter;				// Which Filter To Use
GLuint	texture[5];

GLfloat pyramid_rotate = 0.0;
GLfloat cloud_move[2];

GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Light0Position[]=	{ 10.0f, 10.0f, -10.0f, 1.0f };
GLfloat Light1Position[]=	{ 10.0f, 10.0f, -10.0f, 1.0f };

GLfloat prevx, prevz;

CollisionModel3D* model_robot = newCollisionModel3D();
CollisionModel3D* model_player = newCollisionModel3D();
CollisionModel3D* fuel_can = newCollisionModel3D();
float mat1[16];

g_enemy			Enemy[MAX_ENEMIES];
g_player		Player;
g_util			Utility;
g_util_input	Input;
g_world			World;
//AUDIO			Music, Sound;
MD2				Epsilon;
MD2				EpsilonWeapon;
TEXTURE			EpsilonSkin;
TEXTURE			EpsilonWeaponSkin;
TEXTURE			skybox[6];
TEXTURE			cloud_plane;
/*
SOUND			music;
SOUND			fuelsound, shoot, death, gameover, pushsound; */
TEXTURE			flare; // for particles
PARTICLE_SYSTEM ps1;

struct
{
  __int64       frequency;								// Timer Frequency
  float         resolution;								// Timer Resolution
  unsigned long mm_timer_start;     
  
  // Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;						// Multimedia Timer Elapsed Time
  bool   performance_timer;    
  
  // Using The Performance Timer?
  __int64       performance_timer_start;				// Performance Timer Start Value
  __int64       performance_timer_elapsed;				// Performance Timer Elapsed Time
} timer;												// Structure Is Named 'timer'


// Prototypes:
bool	Game_Init(void);
bool	Game_Main(void);
bool	Game_Shutdown(void);
int		WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);


int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, 
				   int nCmdShow) 
{
	//Do all of the Initiation stuff
	if(Game_Init()==false)
		return 0;

	MSG		msg;									// Windows Message Structure

	while(!done)									// Loop That Runs While done=FALSE
	{
		if(Utility.eventHandler()==false)
			break;

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active && !Game_Main())	// Active?  Was There A Quit Received?
			{
				done=true;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hdc);					// Swap Buffers (Double Buffering)
				Player.Update_Mouse();
				Player.Update_Input( &World, fuel_can, &Enemy[0], &FuelCan[0], &FrameRate );
				for(int i = 0; i < MAX_ENEMIES; i++ )
					Enemy[i].Update_AI( &World, model_player, &FrameRate );
			}
		}
	}

	//Call Game_Shutdown, and do all the shutdown procedures
	Game_Shutdown();
	return(EXIT_SUCCESS);
}


bool Game_Init(void)
{
	/*
for (int ii=0; ii < 60 ; ii++ )
{
	for (int jj=0; jj < 60 ; jj++ )
	{ 
		Level_Zero[ii][jj] = 0;
	}
} */xx=0;
yy=0;
zz=0;
aa=0;
	cloud_move[0] = 0.0;
	cloud_move[1] = 0.0;

	FrameRate.Init(20);
	Utility.initAll(SCR_WIDTH,SCR_HEIGHT, SCR_BPP, ASK_FULLSCREEN);// Create a fullscreen/window with dimensions
	Input.initDI();									// Initiate DirectInput
	Utility.initFont();										// Initiate the font
	World.init_Block( 0 );							// Start level one;
	//S3Dlog.Output("Direct Initiation:");
	/*
	CoInitialize(NULL);

	Music.Init();
	Sound.Init();

	Music.Create(&music, "Sounds/sib.mid", false);
	Sound.Create(&fuelsound, "Sounds/fuel.wav", false);
	DLL_API FSOUND_STREAM * F_API FSOUND_Stream_OpenFile(const char *filename, unsigned int mode, int memlength);
	DLL_API FSOUND_STREAM * F_API FSOUND_Stream_Create(FSOUND_STREAMCALLBACK callback, int length, unsigned int mode, int samplerate, int userdata);
	DLL_API signed char     F_API FSOUND_Stream_Close(FSOUND_STREAM *stream);

	DLL_API int             F_API FSOUND_Stream_Play(int channel, FSOUND_STREAM *stream);

	Sound.Create(&pushsound, "Sounds/push.wav", false);
	Sound.Create(&shoot, "Sounds/shoot.wav", false);
	Sound.Create(&death, "Sounds/death.wav", false);
	Sound.Create(&gameover, "Sounds/gameover.wav", false);
	//Music.Play(&music, 50); */
	FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
    if (!FSOUND_Init(44100, 32, FSOUND_INIT_USEDEFAULTMIDISYNTH))
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
    }
	mod = FMUSIC_LoadSong("Sounds/ballad-x.xm");
    FMUSIC_SetMasterVolume(mod, 192);
    FMUSIC_PlaySong(mod);

	reshape();

	
//	glEnable(GL_CULL_FACE);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5);					// Full Brightness.  50% Alpha
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glCullFace(GL_BACK);
    glColorMaterial( GL_FRONT, GL_DIFFUSE );
	make_Textures( texture );
	flare.LoadTGA( "Art/flare.tga", GL_LINEAR, GL_LINEAR );

	// Particle System Initialization
	ps1.SetAlpha(1.0f, 0.0f);
	ps1.SetVelocity(1.0f, 1.0f, 1.0f);
	ps1.SetTexture( flare.ID );
	ps1.SetAngle(360);
	ps1.SetAttraction(0);
	ps1.SetColor(1.0f, 0.0f, 0.0f,   1.0f, 0.55f, 0.1f);
	ps1.SetGravity(0.0f, 0.0f, 0.0f);
	ps1.SetLife(4);
	ps1.SetLocation(0.0f, 0.0f, 0.0f);
	ps1.SetParticlesPerSec(0);
	ps1.SetSize(3, 2);
	ps1.SetSpeed(10);
	ps1.SetSpread(-100, 100, 200);
	ps1.SetSystemFlag(STOP, SK_OFF);
	ps1.SetSystemFlag(ATTRACTION, SK_OFF);
	
	/*
	// Coldet registration (collision detection stuff...)
	fuel_can->addTriangle( -2.0f, -2.0f, -2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f );
	fuel_can->addTriangle( -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f, -2.0f,  2.0f, -2.0f );

	fuel_can->addTriangle(  2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f,  2.0f,  2.0f );
	fuel_can->addTriangle(  2.0f, -2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f, -2.0f );

	fuel_can->addTriangle( -2.0f, -2.0f, -2.0f,  2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f );
	fuel_can->addTriangle(  2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f, -2.0f, -2.0f,  2.0f );

	fuel_can->addTriangle( -2.0f,  2.0f, -2.0f,  2.0f,  2.0f, -2.0f,  2.0f,  2.0f,  2.0f );
	fuel_can->addTriangle(  2.0f,  2.0f, -2.0f,  2.0f,  2.0f,  2.0f, -2.0f,  2.0f,  2.0f );

	fuel_can->addTriangle( -2.0f, -2.0f, -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f, -2.0f );
	fuel_can->addTriangle( -2.0f,  2.0f, -2.0f,  2.0f,  2.0f, -2.0f,  2.0f, -2.0f, -2.0f );

	fuel_can->addTriangle( -2.0f, -2.0f,  2.0f, -2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f );
	fuel_can->addTriangle( -2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f, -2.0f,  2.0f );

	fuel_can->finalize (); */

	model_player->addTriangle(0.0f, 1.0f, 0.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f);
	model_player->addTriangle(0.0f, 1.0f, 0.0f, 1.0f,-1.0f, 1.0f, 1.0f,-1.0f,-1.0f);
	model_player->addTriangle(0.0f, 1.0f, 0.0f, 1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f);
	model_player->addTriangle(0.0f, 1.0f, 0.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f);

	model_player->finalize ();

	// Fog stuff...
    /*
	glEnable(GL_FOG);
    {
	   GLfloat density = 0.01;
	   GLfloat fogColor[4] = {0.5, 0.5, 0.5, 0.3};
	   GLint fogMode;
	   fogMode = GL_EXP;
	   glFogi(GL_FOG_MODE, fogMode);
	   glFogfv(GL_FOG_COLOR, fogColor);
	   glFogf(GL_FOG_DENSITY, density);
	   glHint(GL_FOG_HINT, GL_DONT_CARE);
    }*/

	//glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT1);
    //glEnable(GL_LIGHT0);
//	glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
//	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_COLOR_MATERIAL);

	// CLOUDS
	cloud_plane.LoadBMP("clouds.bmp", GL_LINEAR, GL_LINEAR);	 // Clouds
	
	//SKY BOX
	
//	skybox[0].LoadTGA("Art/myghills_bk.tga", GL_LINEAR, GL_LINEAR);  // Back
//	skybox[1].LoadTGA("Art/myghills_ft.tga", GL_LINEAR, GL_LINEAR);  // Front
//	skybox[2].LoadTGA("Art/myghills_lf.tga", GL_LINEAR, GL_LINEAR);  // Left
//	skybox[3].LoadTGA("Art/myghills_rt.tga", GL_LINEAR, GL_LINEAR);  // Right
//	skybox[4].LoadTGA("Art/myghills_up.tga", GL_LINEAR, GL_LINEAR);  // Up
//	skybox[5].LoadTGA("Art/myghills_dn.tga", GL_LINEAR, GL_LINEAR);  // Down
	
	
	skybox[0].LoadBMP("skybox/sb1_right.bmp", GL_LINEAR, GL_LINEAR);  // Back
	skybox[1].LoadBMP("skybox/sb1_left.bmp", GL_LINEAR, GL_LINEAR);  // Front
	skybox[2].LoadBMP("skybox/sb1_back.bmp", GL_LINEAR, GL_LINEAR);  // Left
	skybox[3].LoadBMP("skybox/sb1_front.bmp", GL_LINEAR, GL_LINEAR);  // Right
	skybox[4].LoadBMP("skybox/sb1_top.bmp", GL_LINEAR, GL_LINEAR);  // Up
	skybox[5].LoadBMP("skybox/sb1_bottom.bmp", GL_LINEAR, GL_LINEAR);  // Down
	
	/* Quake2 model load */

	
//	Epsilon.Load("models/chastity/tris.md2");
//	EpsilonWeapon.Load("models/chastity/weapon.md2");
//	EpsilonSkin.LoadBMP("models/chastity/chastity1.bmp", GL_LINEAR, GL_LINEAR);
//	EpsilonWeaponSkin.LoadTGA("models/weapon.tga", GL_LINEAR, GL_LINEAR);
		

	
//	Epsilon.Load("models/sorcerer/tris.md2");
//	EpsilonWeapon.Load("models/sorcerer/weapon.md2");
//	EpsilonSkin.LoadBMP("models/sorcerer/sorcerer.bmp", GL_LINEAR, GL_LINEAR);
//	EpsilonWeaponSkin.LoadBMP("models/sorcerer/weapon.bmp", GL_LINEAR, GL_LINEAR);
	

	
//	Epsilon.Load("models/ch/tris.md2");
//	EpsilonWeapon.Load("models/ch/weapon.md2");
//	EpsilonSkin.LoadBMP("models/ch/ps.bmp", GL_LINEAR, GL_LINEAR);
//	EpsilonWeaponSkin.LoadTGA("models/sorcerer/weapon.tga", GL_LINEAR, GL_LINEAR);
	

	
//	Epsilon.Load("models/death/tris.md2");
//	EpsilonWeapon.Load("models/death/weapon.md2");
//	EpsilonSkin.LoadBMP("models/death/death1.bmp", GL_LINEAR, GL_LINEAR);
//	EpsilonWeaponSkin.LoadTGA("models/weapon.tga", GL_LINEAR, GL_LINEAR);
	

	/*
	Epsilon.Load("models/unit01/tris.md2");
	EpsilonWeapon.Load("models/unit01/weapon.md2");
	EpsilonSkin.LoadBMP("models/unit01/unit011.bmp", GL_LINEAR, GL_LINEAR);
	EpsilonWeaponSkin.LoadTGA("models/weapon.tga", GL_LINEAR, GL_LINEAR);
	*/

	/*
	Epsilon.Load("models/piccolo/tris.md2");
	EpsilonWeapon.Load("models/piccolo/weapon.md2");
	EpsilonSkin.LoadBMP("models/piccolo/piccolo1.bmp", GL_LINEAR, GL_LINEAR);
	EpsilonWeaponSkin.LoadTGA("models/weapon.tga", GL_LINEAR, GL_LINEAR);
	*/

	/*
	Epsilon.Load("models/zumlin/tris.md2");
	EpsilonWeapon.Load("models/zumlin/weapon.md2");
	EpsilonSkin.LoadBMP("models/zumlin/zumlin1.bmp", GL_LINEAR, GL_LINEAR);
	EpsilonWeaponSkin.LoadTGA("models/weapon.tga", GL_LINEAR, GL_LINEAR);
	*/

    tex_on = true;
	//lights_on = true;

	for(int i = 0; i < MAX_FUEL; i++ )
	{
		place_fuel(i);
	}
	return true;
}


bool Game_Main(void)
{

	FrameRate.SetSpeedFactor();

	Input.updateDI();		//Update DirectInput to make sure it's still alive
	if(KEY_DOWN(DIK_ESCAPE))	//Check to see if the user wants to quit
		PostQuitMessage(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear screen and depth buffer									
	

	//Reset the current modelview matrix
	glLoadIdentity();

	update();


	Player.Rotate();

	Light1Position[0] = -30;
	Light1Position[1] = -30;
	Light1Position[2] = -30;
	Light1Position[3] = 1;//21.0;
	glLightfv(GL_LIGHT0, GL_POSITION,Light0Position);	// Position The Light
	glLightfv(GL_LIGHT1, GL_POSITION,Light1Position);	// Position The Light

	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light

	
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	cloud_move[0] += 0.00002;
	cloud_move[1] += 0.00001;
	if ( cloud_move[0] >= 1.0 )
		cloud_move[0] = -1.0;
	if ( cloud_move[1] >= 1.0 )
		cloud_move[1] = -1.0;
//	glScalef(50.0, 50.0, 50.0);
	glColor4f(1.0, 0.0, 0.0, 0.0);
	if(keys['N'] || n_key)
	{
		n_key = true;
		b_key = false;
		glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
		World.make_Clouds( cloud_move, &cloud_plane );
	}
	glDepthMask(GL_TRUE);
	if(keys['B'] || b_key)
	{
		n_key = false;
		b_key = true;
		World.make_SkyBox( &skybox[0] );
	}/*
	if( lights_on )
	{
		glEnable(GL_LIGHTING);
	}*/




	glEnable(GL_COLOR_MATERIAL);
	glBindTexture(GL_TEXTURE_2D, texture[0]); 
//	glScalef(0.02, 0.02, 0.02);
	
	glGetFloatv(GL_MODELVIEW_MATRIX, mat1);
	model_player->setTransform (mat1); 
	Player.Translate();

	//ExtractFrustum();

	make_Maze(); // draw the maze

	//Render Enemies	
	for(int i = 0; i < MAX_ENEMIES; i++)
	{
		glPushMatrix();
		Enemy[i].Translate();
		Enemy[i].Rotate();
//		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glColor3f( 1.0, 1.0, 1.0 );
		glGetFloatv(GL_MODELVIEW_MATRIX, Enemy[i].mv_matrix);
		Enemy[i].SetColliderTransform();
		glScalef(0.2, 0.2, 0.2);
		glEnable(GL_TEXTURE_2D);

		// Quake2 model animate 
		glBindTexture(GL_TEXTURE_2D, EpsilonSkin.ID);
		if( !Enemy[i].IsDead() )
		{
			int h = 0;
			h++;
		//	Epsilon.SetState(RUN);
		//	Epsilon.Animate(CROUCH_START, CROUCH_END, 0.28f);
		//	Epsilon.Animate(RUN_START, RUN_END, 0.118f);
		//	Epsilon.Animate(IDLE1_START, IDLE1_END, 0.28f);
		//	Epsilon.Animate(IDLE1_START, CROUCH_DEATH_END, 0.28f);
		//	Epsilon.Animate(CROUCH_DEATH_START, CROUCH_DEATH_END, 0.28f);
		//	Epsilon.Animate(DEATH_FALLFORWARD_START, DEATH_FALLFORWARD_END, 0.28f);
		//	Epsilon.Animate(IDLE2_START, IDLE2_END, 0.28f);
		//	Epsilon.Animate(IDLE4_START, IDLE4_END, 0.28f);
		//	glBindTexture(GL_TEXTURE_2D, EpsilonWeaponSkin.ID);
		//	EpsilonWeapon.Animate(IDLE1_START, IDLE1_END, 0.28f);
		//	EpsilonWeapon.Animate(RUN_START, RUN_END, 0.018f);
		}
		else
		{
			if( flag < 65 )
			{
				flag++;
			//	Epsilon.SetState(CROUCH_DEATH);
				Epsilon.Animate(CROUCH_DEATH_START, CROUCH_DEATH_END, 0.18f);
			//	glBindTexture(GL_TEXTURE_2D, EpsilonWeaponSkin.ID);
			//	EpsilonWeapon.Animate(CROUCH_DEATH_START, CROUCH_DEATH_END, 0.18f);
			}
			else
			{
				Player.Fragged_Em();
				Enemy[i].Respawn( &World );
				flag = 0;
			}
		}
		glPopMatrix();
	} // Finished rendering enemies...

	first_time = 0;

	


	/* draw gun */
	glDisable(GL_BLEND);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f( 0.0, 0.0, 0.0 );
	glPushMatrix();
	if( Player.Is_Shooting() ) { // Draw gun shooting
	//	Sound.Play( &shoot, 0 );
		glTranslatef(0.3, -1.0, -3.5);
		Player.Shot();
	}
	else {
		glTranslatef(0.1, -1.2, -5);
	} 
	World.make_Gun();
	glPopMatrix();

		GetClientRect(hwnd,&window);					//Get the window's dimensions
		glMatrixMode(GL_PROJECTION);					//Select the projection matrix
		glPushMatrix();									//Store the projection matrix
		glLoadIdentity();								//Reset the projection matrix
		glOrtho(0,window.right,0,window.bottom,-1,1);	//Set up an ortho screen
		glMatrixMode(GL_MODELVIEW);						//Select the modelview matrix

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	
	glColor4f(1,0,0,1);
	/*
	GetClientRect(hwnd,&window);					//Get the window's dimensions
	glMatrixMode(GL_PROJECTION);					//Select the projection matrix
	glPushMatrix();									//Store the projection matrix
	glLoadIdentity();								//Reset the projection matrix
	glOrtho(0,window.right,0,window.bottom,-1,1);	//Set up an ortho screen
	glMatrixMode(GL_MODELVIEW);						//Select the modelview matrix

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	
	//Demo title
	glColor4d(1,1,1,1);
	s3d.glPrint(200,450,".md2/.ms3d Loading/Rendering Demo");
	s3d.glPrint(0,  435,"FPS: %f", fps);
*/
/*
	Utility.glPrint(200,100,"pxpos: %f", Player.Get_xpos());
	Utility.glPrint(200,150,"pzpos: %f", Player.Get_zpos());
	Utility.glPrint(200,0,"xpos: %f", Enemy.Get_xpos());
	Utility.glPrint(200,50,"zpos: %f", Enemy.Get_zpos());
*/
//	Utility.glPrint(200,200,"quadrants being rendered: %d", quad);
//	Utility.glPrint(200,350,"pcol: %d", Player.Get_Col());
//	Utility.glPrint(200,200,"row: %d", Enemy.Get_Row());
//	Utility.glPrint(200,250,"time: %f", time);

	glDisable(GL_DEPTH_TEST);
    Utility.glPrint(0,0,"Kills: %d", Player.Get_Kills());
	glColor4f(1,0,0,1);
	Utility.glPrint(400,300,"+" );

	glColor4f(1,1,1,1);
	Utility.glPrint(100,0,"Ammo: %d", Player.Get_Ammo()); 
	//Utility.glPrint(100,0,"Ammo: %s", glGetString(GL_VERSION)); 
	glColor4f(0,1,0,1);
	Utility.glPrint(200,0,"FPS: %d", (int) fps);
	glColor4f(0,0,1,1);
	Utility.glPrint(500,20,"World Keys: B, N, M");
	glColor4f(0,0,1,1);
	Utility.glPrint(500,0,"Move Keys: E, D, S, F, Space"); 
		glDisable(GL_BLEND);
		glMatrixMode(GL_PROJECTION);					//Select the projection matrix
		glPopMatrix();									//Restore the old projection matrix
		glMatrixMode(GL_MODELVIEW);						//Select the modelview matrix

	glFlush ();
	return(true);
}


bool Game_Shutdown(void)
{
	glFinish();
	
/*	music.Shutdown();
	fuelsound.Shutdown();
	pushsound.Shutdown();
	shoot.Shutdown();
	death.Shutdown();
	gameover.Shutdown();
	Music.Shutdown(); 
	Sound.Shutdown();

	//Uninitialize COM (*MUST* be done, or you're asking for trouble)
	CoUninitialize();
*/
	Utility.shutdownFont();
	Input.shutdownDI();
	Utility.shutdownAll();

    FMUSIC_StopSong(mod);
    FMUSIC_FreeSong(mod);
    FSOUND_Close();

	return(false);
}


/* fuel updating */
void update_fuel( int row, int col )
{
  if (World.block[col][row] == 66) {
	  int fuel_index = 0;
		/* update fuel status */
		//Sound.Play( &fuelsound, 0 );
	  for(int i=0; i < MAX_FUEL; i++ )
	  {
		  if(FuelCan[i].GetCol() == col && FuelCan[i].GetRow() == row)
		  {
			  fuel_index = i;
			  break;
		  }
	  }
	  Player.Reload();
		/* update city */
	  World.block[col][row] = 0;
		/* insert new fuel cell somewhere */	
	  place_fuel(fuel_index);
  }
}

void update( void )
{
  int col, row;
  col = Player.Get_Col();
  row = Player.Get_Row();
  switch (World.block[col][row]) {
  case 66:
    /* check fuel cells on ground */
    update_fuel(row, col);
    break;
  }
}


// Render the maze. ********************************************
// *************************************************************
void make_Maze( void )
{
	float time = timeGetTime() * 0.001f; // Get current time in seconds
	GLint i,j,k,l, e=0, x=0;
	++frames;


	// Update the frame rate once per second
	if(time - last_time > 1.0f)
	{
		fps      = frames/(time - last_time);
		last_time= time;
		frames   = 0;
	}

	World.make_Land( texture[3], tex_on, 1.0 );

	// Render the world blocks
	for(k=0; k< 2; k++ )
	{
		for(l=0; l<2; l++ )
		{// *2*30
			//if( SphereInFrustum( l*ground_radius*2*30 + 120, 0.0, 
			//					 k*ground_radius*2*30 + 120, 150.0 ) )
			if( 1 )
			{							
				for(i=0 +  k*30; i< ((max_block/2) + k*30); i++)
				{
					for(j=0 + l*30; j< ((max_block/2) + l*30); j++)
					{
						//if( SphereInFrustum( j*ground_radius*3, 0.0, 
						//					 i*ground_radius*3, 6.0 ) )
						if( 1 )
						{
							glPushMatrix();
							glTranslatef(j*ground_radius*2, 0, i*ground_radius*2);     
							// Create various maze objects.
							switch(World.block[i][j]){
							case 88:
								glPushMatrix();
								glRotatef(pyramid_rotate, 0.0, 1.0, 0.0 );
								pyramid_rotate+=10;
								if(pyramid_rotate >= 360) 
									pyramid_rotate = 0;
								World.make_Pyramid(); 
								glPopMatrix();
								break;
							case 4: 
								World.make_Cube( 585, texture[4], tex_on ); 
								break;			
							case 5:
								World.make_Cube(World.block[i][j], texture[0], tex_on ); 
								break;		
							
							case 7: 
								World.make_Cube(World.block[i][j], texture[0], tex_on ); 
								break;			
							case 8: 
								World.make_Cube(World.block[i][j], texture[0], tex_on ); 
								break;			
							case 9:
								World.make_Cube(World.block[i][j], texture[0], tex_on ); 
								break;
							case 77:
								glTranslatef(0.0, 15, 0.0);
								World.make_Quad( texture[0], tex_on , -1.0);
								break;
							case 12:
								glColor3f(1.0, 0.0, 0.0);
								glScalef(0.4, 0.4, 0.4);
								glutSolidDodecahedron(); 
								glScalef(1.0, 1.0, 1.0);
								break;
							case 13:
								glColor3f(0.4, 0.0, 0.4);
								glScalef(1.2, 1.2, 1.2);
								glutSolidTorus(0.25, 0.5, 20, 10); 
								glScalef(1.0, 1.0, 1.0);
								break;
							case 10:
								World.make_Tree( texture[3], tex_on ); break;
							case 1: 
								World.make_Cube(World.block[i][j], texture[3], tex_on ); 
								break;			
							case 3: 
								World.make_Cube(World.block[i][j], texture[3], tex_on ); 
								break;
							default: break;
							};
							glPopMatrix();
						}
					}
				}
			} 		
		}
	}
	// Find and place ammo cells.
	
	for (i = 0; i < max_block; i++) {
     for (j = 0; j < max_block; j++) {
//		if( SphereInFrustum( j*ground_radius*2, 4.0, i*ground_radius*2, 4.0 ) )
		if( 1 )
		{
		  glPushMatrix();
		  glTranslatef(j*ground_radius*2, 0, i*ground_radius*2);
		  
		  if (World.block[i][j] == 66) {	
			  glScalef(0.25, 0.75, 0.25);
			  World.make_Fuel(10, tex_on);
			  /*for(int k = 0; j < MAX_FUEL; k++ )
			  {
				//	if( (FuelCan[k].GetCol() == j) && (FuelCan[k].GetRow() == i) )
				//	{
					//	glGetFloatv(GL_MODELVIEW_MATRIX, FuelCan[k].mv_matrix);
					//	FuelCan[k].SetColliderTransform();
				//	}
			  }*/
		  }
		  if (World.block[i][j] == 6) { 
			  World.make_Cube(World.block[i][j], 999, tex_on ); 
		  }
		  if (World.block[i][j] == 2) { 
				// Particle System Stuff: //
//				glDisable(GL_CULL_FACE);
				glDisable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				ps1.Render();
				ps1.Update( time*2, UPDATE_AND_CREATE, 500 );
//				glEnable(GL_LIGHTING);
				glDisable(GL_BLEND);
		  }
		  
		  glPopMatrix();
		}
     }
	} 
	
}


void place_fuel( int fuel_index )
{
  static GLfloat random = 0;
  static GLint target_x = 0;
  static GLint target_z = 0;

  random = ((float)rand()/RAND_MAX);
  target_x = max_block*random;
  random = ((float)rand()/RAND_MAX);
  target_z = max_block*random;
  if (World.block[target_z][target_x] == 0)
  {
	 FuelCan[fuel_index].SetPosition( target_z, target_x );
     World.block[target_z][target_x] = 66;
  }
  else
    place_fuel(fuel_index);
}


void spawn_enemy( int x )
{
  static GLfloat random = 0;
  static GLint target_x = 0;
  static GLint target_z = 0;

  random = ((float)rand()/RAND_MAX);
  target_x = max_block*random;
  random = ((float)rand()/RAND_MAX);
  target_z = max_block*random;
}


void reshape ( void )
{
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum (-8.5, 8.5, -8.0, 8.0, 0.8, 30.0);
  // gluPerspective(50.0, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1, 5000);
  gluPerspective(45.0, 1.333, 0.1, 2000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void ExtractFrustum()
{
	float t;
	MATRIX4X4 proj, modl, clip;
	glGetFloatv( GL_PROJECTION_MATRIX, proj.matrix );
	glGetFloatv( GL_MODELVIEW_MATRIX, modl.matrix );

	clip = proj * modl;

	frustum[0][0] = clip.matrix[ 3] - clip.matrix[ 0];
	frustum[0][1] = clip.matrix[ 7] - clip.matrix[ 4];
	frustum[0][2] = clip.matrix[11] - clip.matrix[ 8];
	frustum[0][3] = clip.matrix[15] - clip.matrix[12];
 
	/* Normalize the result */
   t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
   frustum[0][0] /= t;
   frustum[0][1] /= t;
   frustum[0][2] /= t;
   frustum[0][3] /= t;

   /* Extract the numbers for the LEFT plane */
   frustum[1][0] = clip.matrix[ 3] + clip.matrix[ 0];
   frustum[1][1] = clip.matrix[ 7] + clip.matrix[ 4];
   frustum[1][2] = clip.matrix[11] + clip.matrix[ 8];
   frustum[1][3] = clip.matrix[15] + clip.matrix[12];

   /* Normalize the result */
   t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
   frustum[1][0] /= t;
   frustum[1][1] /= t;
   frustum[1][2] /= t;
   frustum[1][3] /= t;

   /* Extract the BOTTOM plane */
   frustum[2][0] = clip.matrix[ 3] + clip.matrix[ 1];
   frustum[2][1] = clip.matrix[ 7] + clip.matrix[ 5];
   frustum[2][2] = clip.matrix[11] + clip.matrix[ 9];
   frustum[2][3] = clip.matrix[15] + clip.matrix[13];

   /* Normalize the result */
   t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
   frustum[2][0] /= t;
   frustum[2][1] /= t;
   frustum[2][2] /= t;
   frustum[2][3] /= t;

   /* Extract the TOP plane */
   frustum[3][0] = clip.matrix[ 3] - clip.matrix[ 1];
   frustum[3][1] = clip.matrix[ 7] - clip.matrix[ 5];
   frustum[3][2] = clip.matrix[11] - clip.matrix[ 9];
   frustum[3][3] = clip.matrix[15] - clip.matrix[13];

   /* Normalize the result */
   t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
   frustum[3][0] /= t;
   frustum[3][1] /= t;
   frustum[3][2] /= t;
   frustum[3][3] /= t;

   /* Extract the FAR plane */
   frustum[4][0] = clip.matrix[ 3] - clip.matrix[ 2];
   frustum[4][1] = clip.matrix[ 7] - clip.matrix[ 6];
   frustum[4][2] = clip.matrix[11] - clip.matrix[10];
   frustum[4][3] = clip.matrix[15] - clip.matrix[14];

   /* Normalize the result */
   t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
   frustum[4][0] /= t;
   frustum[4][1] /= t;
   frustum[4][2] /= t;
   frustum[4][3] /= t;

   /* Extract the NEAR plane */
   frustum[5][0] = clip.matrix[ 3] + clip.matrix[ 2];
   frustum[5][1] = clip.matrix[ 7] + clip.matrix[ 6];
   frustum[5][2] = clip.matrix[11] + clip.matrix[10];
   frustum[5][3] = clip.matrix[15] + clip.matrix[14];

   /* Normalize the result */
   t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
   frustum[5][0] /= t;
   frustum[5][1] /= t;
   frustum[5][2] /= t;
   frustum[5][3] /= t;
}

bool SphereInFrustum( float x, float y, float z, float radius )
{
   int p;

   for( p = 0; p < 6; p++ )
      if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= -radius )
         return false;
   return true;
} 

/***************************EOF***********************************/
/*****************************************************************/



