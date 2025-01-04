// g_world.h

#include "g_util.h"

#ifndef g_world_h
#define g_world_h

#define	ground_radius 4

extern int Level_Zero[60][60];
//extern int Level_One[20][20];
//extern int Level_Two[20][20];


class g_world
{

public:
	g_world( void );
	int  get_Level( void );
	void make_Clouds( GLfloat cloud_move[], TEXTURE *cloud_plane );
	void make_Pyramid( void );
	void make_Cube(  int color, int texture_ID, bool textures_on );
	void make_Quad(  int texture_ID, bool textures_on, float normal );
	void make_Land(  int texture_ID, bool textures_on, float normal );
	void make_Tree(  int texture_ID, bool textures_on );
	void make_Floor( int texture_ID, bool textures_on );
	void make_Fuel(  int detail    , bool textures_on );								
	void make_Life( float life_radius, bool textures_on );
	void make_Gun( void );
	void make_SkyBox( TEXTURE *skybox );
	void init_Block( int level );

	int block[60][60]; 
	bool tex_on;
	bool music_on;
	bool lights_on;
private:
	int  world_level;

};


class g_fuel
{

public:
	g_fuel( void );
	void SetPosition( int col, int row );
	int GetCol( void );
	int GetRow( void );
	float mv_matrix[16];
	CollisionModel3D *collider;
	void SetColliderTransform( void );

private:
	int col;
	int row;
};
#endif