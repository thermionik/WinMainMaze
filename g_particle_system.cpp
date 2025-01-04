// g_particle_system.cpp

#include "g_util.h"

PARTICLE_SYSTEM::PARTICLE_SYSTEM() : attracting(false),	   stopped(false),
						texture(0),			   particlesPerSec(0),
						particlesNumbAlive(0), startSize(0.0f),
						sizeCounter(0.0f),	   endSize(0.0f), 
						startAlpha(0.0f),	   alphaCounter(0.0f),
						endAlpha(0.0f),		   speed(0.0f),			  speedCounter(0.0f),
						life(0.0f),			   lifeCounter(0.0f),	  angle(0.0f),		  age(0.0f),
						lastUpdate(0.0f),	   emissionResidue(0.0f), 
						spreadMin(0),		   spreadMax(0),	      spreadFactor(1.0f),
						attractionPercent(0.0f)
	{	}

//------------------------------------------------------------------//
//- PARTICLE_SYSTEM:: ~PARTICLE_SYSTEM() ---------------------------//
//------------------------------------------------------------------//
//- Description: No, let me take that back.  *THIS* is the most	   -//
//-				 boring function in the system.  Its an empty	   -// 
//-				 destructor.  Yahoo.							   -//
//------------------------------------------------------------------//
PARTICLE_SYSTEM::~PARTICLE_SYSTEM()
{	/* Echo! Echooo... Echooooooo... */	}

//------------------------------------------------------------------//
//- bool PARTICLE_SYSTEM:: Update(float) ---------------------------//
//------------------------------------------------------------------//
//- Description: This is the update function that will update all  -//
//-				 of the particles in the system.  This is one	   -//
//-				 *LONG* function, and they don't get much more	   -//
//-			     boring than this, because it doesn't even generate-// 
//-				 any effects. *sigh*  Well, lets get to it.		   -//
//------------------------------------------------------------------//
bool PARTICLE_SYSTEM::Update(float time, int flag, float numToCreate)
{
	int loop;
	unsigned int particlesCreated;
	float timeCounter;
	float particlesNeeded=numToCreate;

	//We need the calculate the elapsed time
	timeCounter= (time-lastUpdate);

	if(flag== ONLY_UPDATE || flag== UPDATE_AND_CREATE)
	{
		//Set the time of the last update to now.  Its sort of wierd
		//doing all this stuff that is in the past, now... Its starting
		//to mess with my brain, oh darn, I guess I need another Mountain
		//Dew, well isn't that a shame. ^_^
		lastUpdate=time;

		//Clear the particle counter variable before counting.
		particlesNumbAlive=0;

		//Update all particles
		for(loop=0; loop<MAX_PARTICLES; loop++)
		{
			if(particle[loop].age>=DEATH_AGE)
			{
				if(particle[loop].Update(timeCounter))
					particlesNumbAlive++;
			}
		}
	}

	if(flag== ONLY_CREATE || flag== UPDATE_AND_CREATE)
	{
		//Now calculate how many particles we should create based on 
		//time and taking the previous frame's emission residue into
		//account.  This is where things start to get kewl.  
		//Of course, when you're dealing with particles, everything is kewl.
		particlesNeeded+= particlesPerSec*timeCounter+emissionResidue;
		
		//Now, taking the previous line into account, we now cast 
		//particles_needed into an unsigned int, so that we aren't
		//going to try to create half of a particle or something.
		particlesCreated= (unsigned int)particlesNeeded;

		if(!stopped)
		{
			//This will remember the difference between how many we wanted
			//to create, and how many we actually did create.  Doing it
			//this way, we aren't going to lose any accuracy.
			emissionResidue= particlesNeeded-particlesCreated;
		}
		else
		{
			emissionResidue= particlesNeeded;
			particlesCreated=0;
		}

		//Lets make sure that we actually have a particle, or two, or
		//thousand to create
		if(particlesCreated<1)
		{
			prevLocation.vertex[0]=location.vertex[0];
			prevLocation.vertex[1]=location.vertex[1];
			prevLocation.vertex[2]=location.vertex[2];
			return true;
		}

		for(loop=0; loop<MAX_PARTICLES; loop++)
		{
			//If we have created enough particles to satisfy the needed
			//amount, then this value will be zero, and will quit this 
			//loop faster than Nixon resigned from office.
			if(!particlesCreated)
				break;

			//If the age of this particles is -1.0, then this particle
			//is not in use.  The poor guy died probably.  Damn shame.
			if(particle[loop].age<DEATH_AGE)
			{
				particle[loop].Create(this, timeCounter);

				//Now we decrease the amount of particles we need to create
				//by one.  
				particlesCreated--;
			}
		}
	}

	return true;
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: Render(float) --------------------------------//
//------------------------------------------------------------------//
//- Descriptions: This function renders all of the particles that  -//
//-				  are alive in the current particle system.		   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::Render(void)
{
	VERTEX vert;
	float size;
	int loop;

	float modelview[16];
	int i,j;

	// save the current modelview matrix

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	glPushMatrix();
	// The only difference now is that
	// the i variable will jump over the
	// up vector, 2nd column in OpenGL convention

	for( i=0; i<3; i+=3 ) 
		for( j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview matrix with the 
	// up vector unchanged
	glLoadMatrixf(modelview);

	// restores the modelview matrix

	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_PIXEL_MODE_BIT);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);


	glEnable(GL_BLEND);

	for(loop=0; loop<MAX_PARTICLES; loop++)
	{
		size=particle[loop].size/2;
		particle[loop].color[4]=particle[loop].alpha;

		if(particle[loop].age>=DEATH_AGE)
		{
			glBindTexture(GL_TEXTURE_2D, particle[loop].texture);
			glColor4fv(particle[loop].color);
				
			glBegin(GL_TRIANGLE_STRIP);

				vert.vertex[0]=particle[loop].location.vertex[0]-size;
				vert.vertex[1]=particle[loop].location.vertex[1]+size;
				vert.vertex[2]=particle[loop].location.vertex[2];
				glTexCoord2d(1,1); glVertex3fv(vert.vertex);

				vert.vertex[0]=particle[loop].location.vertex[0]+size;
				vert.vertex[1]=particle[loop].location.vertex[1]+size;	
				vert.vertex[2]=particle[loop].location.vertex[2];
				glTexCoord2d(0,1); glVertex3fv(vert.vertex);

				vert.vertex[0]=particle[loop].location.vertex[0]-size;
				vert.vertex[1]=particle[loop].location.vertex[1]-size;
				vert.vertex[2]=particle[loop].location.vertex[2];
				glTexCoord2d(1,0); glVertex3fv(vert.vertex);

				vert.vertex[0]=particle[loop].location.vertex[0]+size;
				vert.vertex[1]=particle[loop].location.vertex[1]-size;
				vert.vertex[2]=particle[loop].location.vertex[2]; 
				glTexCoord2d(0,0); glVertex3fv(vert.vertex);
				
			glEnd();
		}
	}
	glDepthMask(GL_TRUE);
	glPopAttrib();
	glPopMatrix();

}

//------------------------------------------------------------------//
//- GLfloat PARTICLE:: GetLocation(GLint) -------------------------//
//------------------------------------------------------------------//
//- Descriptions: This function returns the location of whatever   -//
//-               coordinate of the emitter that you want.  Here   -//
//-               are the possible flags that could be used:	   -//
//-						- GET_X: Returns the x coordinate		   -//
//-						- GET_Y: Returns the y coordinate		   -//
//-						- GET_Z: Returns the z coordinate          -//
//------------------------------------------------------------------//
float PARTICLE_SYSTEM::GetLocation(int coordinate)
{
	switch(coordinate)
	{
		case GET_X:
			return location.vertex[0];
			break;

		case GET_Y:
			return location.vertex[1];
			break;

		case GET_Z:
			return location.vertex[2];
			break;
	}
	return 0;
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetLocation(GLfloat, GLfloat, GLfloat) ------//
//------------------------------------------------------------------//
//- Descriptions: This function sets the particle system's emitter -//
//-			      location.  All particle will be started here     -//
//-               (with a little randomness from the spread        -//
//-               variables).									   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetLocation(float x, float y, float z)
{
	location.vertex[0]=x;
	location.vertex[1]=y;
	location.vertex[2]=z;
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetTexture(S3DTGA_PTR) ----------------------//
//------------------------------------------------------------------//
//- Descriptions: This function sets the particle system's texture -//
//-               to be used for all the particles.  Only one      -//
//-               texture per system. Sowwy.  Anyway, the texture  -//
//-               *MUST* be already loaded before you do this, or  -//
//-               else you won't get the desired result.		   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetTexture(unsigned int textureID)
{	
	texture= textureID;	
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetParticlesPerSec(GLuint) ------------------//
//------------------------------------------------------------------//
//- Descriptions: Sets the number of particles that will be created-//
//-               per second (plus or minus a few).				   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetParticlesPerSec(unsigned int number)
{	
	particlesPerSec=number;	
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetVelocity(float) --------------------------//
//------------------------------------------------------------------//
//- Descriptions: This sets the system's velocity for each         -//
//-				  particle.  Thats really all there is to it.	   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetVelocity(float xv, float yv, float zv)
{
	velocity.vector[0]=xv;
	velocity.vector[1]=yv;
	velocity.vector[2]=zv;
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetSize(GLfloat, GLfloat) -------------------//
//------------------------------------------------------------------//
//- Descriptions: This function sets the size that a particle      -//
//-               starts at, and the size that the particle will be-//
//-               when it's time comes.							   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetSize(float StartSize, float EndSize)
{
	startSize= StartSize;
	endSize  = EndSize;
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetAlpha(GLfloat, GLfloat) ------------------//
//------------------------------------------------------------------//
//- Descriptions: This function sets the alpha value (transparency)-//
//-				  that the particle starts at, and the alpha value -//
//-               that it will be when it dies.					   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetAlpha(float StartAlpha, float EndAlpha)
{
	startAlpha= StartAlpha;
	endAlpha  = EndAlpha;
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetSpeed(GLfloat) ---------------------------//
//------------------------------------------------------------------//
//- Descriptions: This function sets the speed of the particles.   -//
//------------------------------------------------------------------//	
void PARTICLE_SYSTEM::SetSpeed(float Speed)
{	
	speed=Speed;	
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetAngle(GLfloat) ---------------------------//
//------------------------------------------------------------------//
//- Descriptions: This function sets the angle at which the		   -//
//-               particles are emitted.  You only send HALF of the-//
//-               angle that you want the particle to be emitted.  -//
//-               So if you wanted to emit them in a sphere like   -//
//-               pattern, go with 180.							   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetAngle(float halfAngle)
{	
	angle=halfAngle;	
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetSystemFlag(GLint, bool) ------------------//
//------------------------------------------------------------------//
//- Descriptions: This function sets a flag of the particle system -//
//-               either on or off.  First you pass a constant to  -//
//-               the flag argument, here are your valid choice:   -//
//-						-MOVEMENT: Moves the emitter around the	   -//
//-								   screen						   -//
//-						-ATTRACTION: Attracts the particles back   -//
//-									 towards the emitter.		   -//
//-						-STOP: Suspends the particles from moving  -//
//-                            and builds the amount that is going -//
//-                            to be created up (Builds up the	   -//
//-							   the amount to be created).		   -//
//-				   Then for the second flag, you need to pass      -//
//-                either SK_OFF, or SK_ON, depending on if you    -//
//-                want the selected state on or off.			   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetSystemFlag(int flag, bool state)
{
	switch(flag)
	{
		case ATTRACTION:
			attracting=state;
			break;

		case STOP:
			stopped=state;
			break;
	}
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetColor(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat)-//
//------------------------------------------------------------------//
//- Descriptions: This function sets the starting color (RGB), and -//
//-				  the ending color that the particle should be at  -//
//-               when it dies.									   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetColor(float startRed, float startGreen, float startBlue, 
			 float endRed,   float endGreen,   float endBlue)
{
	startColor.vector[0]= startRed;
	startColor.vector[1]= startGreen;
	startColor.vector[2]= startBlue;

	endColor.vector[0]= endRed;
	endColor.vector[1]= endGreen;
	endColor.vector[2]= endBlue;
}

//------------------------------------------------------------------//
//- GLuint PARTICLE:: ActiveParticles(float) ----------------------//
//------------------------------------------------------------------//
//- Descriptions: Returns the number of active particles.		   -//
//------------------------------------------------------------------//
unsigned int PARTICLE_SYSTEM::ActiveParticles(void)
{	
	return particlesNumbAlive;	
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetLife(float) ------------------------------//
//------------------------------------------------------------------//
//- Descriptions: Set each particle's lifespan time, in seconds.   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetLife(float seconds)
{	
	life=seconds;	
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetSpread(GLint, GLint, GLfloat) ------------//
//------------------------------------------------------------------//
//- Descriptions: Set the particle's spread (random spread around  -//
//-				  the emitter's position).  You can set the min    -//
//-               max, and the answer of that is divided by the    -//
//-               spread_factor.								   -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetSpread(int SpreadMin, int SpreadMax, 
			   float SpreadFactor)
{
	spreadMin=SpreadMin;
	spreadMax=SpreadMax;

	if(SpreadFactor==0)
		SpreadFactor=1;
	spreadFactor=SpreadFactor;
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: SetAttraction(GLuint) -----------------------//
//------------------------------------------------------------------//
//- Descriptions: Set the particle's attraction to the emitter, in -//
//-               a percentage ranging from 0% to 100%.		       -//
//------------------------------------------------------------------//
void PARTICLE_SYSTEM::SetAttraction(unsigned int AttractionPercent)
{
	if(AttractionPercent>100)
		AttractionPercent=100;
		
	attractionPercent= (AttractionPercent/100.0f);
}

void PARTICLE_SYSTEM::SetGravity(float xpull, float ypull, float zpull)
{
	gravity.vector[0]=xpull;
	gravity.vector[1]=ypull;
	gravity.vector[2]=zpull;
}