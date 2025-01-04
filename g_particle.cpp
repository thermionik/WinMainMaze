// g_particle.cpp

#include "g_util.h"

PARTICLE::PARTICLE() : age(0.0f), dyingAge(0.0f),
				 size(0.0f), sizeCounter(0.0f),
				 alpha(0.0f), alphaCounter(0.0f)
{
	color[0]=0.0f;		//Set all of the vectors to 0.0 
	color[1]=0.0f; 
	color[2]=0.0f; 
	color[3]=0.0f;

	colorCounter[0]=0.0f;
	colorCounter[1]=0.0f;
	colorCounter[2]=0.0f;
	colorCounter[3]=0.0f;
}

//------------------------------------------------------------------//
//- PARTICLE:: ~PARTICLE() -----------------------------------------//
//------------------------------------------------------------------//
//- Description: Default deconstructor that only shuts down all of -//
//-				 the Particle's values to 0.					   -//	   -//
//------------------------------------------------------------------//	
PARTICLE::~PARTICLE()
{	} 

//------------------------------------------------------------------//
//- GLvoid PARTICLE:: Set_ParentSystem(PARTICLE_SYSTEM*) -----------//
//------------------------------------------------------------------//
//- Description: Gives the class's particle a parent.  This parent -//
//-				 will be used for various updating stuff. Is it a  -//
//-				 mommy or a daddy? No one may ever know. o_O	   -//
//------------------------------------------------------------------//
GLvoid PARTICLE::SetParentSystem(PARTICLE_SYSTEM* parent)
{	
	Parent= parent;	
}

//------------------------------------------------------------------//
//- GLvoid PARTICLE::Create(PARTICLE_SYSTEM*,float) ----------------//
//------------------------------------------------------------------//
//- Description: Creates a particle.  Its private, so you don't    -//
//-				 get to use it at all... HAHAHA. :)                -//
//------------------------------------------------------------------//
GLvoid PARTICLE::Create(PARTICLE_SYSTEM* parent, float timeCounter)
{
	VECTOR tempVelocity;
	float randomYaw;
	float randomPitch;
	float newSpeed;

	//This particle is dead, so its free to mess around with.
	//Now, this is where the fun starts.  Kick butt baby.
	age=0.0;
	dyingAge= (parent->life)+(RANDOM_FLOAT*(parent->lifeCounter));
	CHECK_RANGE(dyingAge, MIN_LIFETIME, MAX_LIFETIME);

	texture= parent->texture;

	//Now, we are going to set the particle's color.  The color
	//is going to be the system's start color*color counter
	color[0]= (parent->startColor.vector[0])+RANDOM_FLOAT*(parent->colorCounter.vector[0]);
	color[1]= (parent->startColor.vector[1])+RANDOM_FLOAT*(parent->colorCounter.vector[1]);
	color[2]= (parent->startColor.vector[2])+RANDOM_FLOAT*(parent->colorCounter.vector[2]);
	color[3]= 1.0f;

	//Lets make sure that the color is legal
	CHECK_RANGE(color[0], MIN_COLOR, MAX_COLOR);
	CHECK_RANGE(color[1], MIN_COLOR, MAX_COLOR);
	CHECK_RANGE(color[2], MIN_COLOR, MAX_COLOR);

	//Now, lets calculate the color's counter, so that by the
	//time the particle is ready to die (poor guy), it will
	//have reached the system's end color.
	colorCounter[0]= ((parent->endColor.vector[0])-color[0])/dyingAge;
	colorCounter[1]= ((parent->endColor.vector[1])-color[1])/dyingAge;
	colorCounter[2]= ((parent->endColor.vector[2])-color[2])/dyingAge;

	//Calculate the particle's alpha from the system's.
	alpha= (parent->startAlpha)+(RANDOM_FLOAT*(parent->alphaCounter));
	//Make sure the result of the above line is legal
	CHECK_RANGE(alpha, MIN_ALPHA, MAX_ALPHA);
	//Calculate the particle's alpha counter so that by the
	//time the particle is ready to die, it will have reached 
	//the system's end alpha
	alphaCounter=((parent->endAlpha)-alpha)/dyingAge;

	//Now, same routine as above, except with size
	size= (parent->startSize)+(RANDOM_FLOAT*(parent->sizeCounter));
	CHECK_RANGE(size, MIN_SIZE, MAX_SIZE);
	sizeCounter= ((parent->endSize)-size)/dyingAge;

	//Now, we calculate the velocity that the particle would 
	//have to move to from prev_location to current_location
	//in time_counter seconds.
	tempVelocity.vector[0]= ((parent->location.vertex[0])-(parent->prevLocation.vertex[0]))/timeCounter;
	tempVelocity.vector[1]= ((parent->location.vertex[1])-(parent->prevLocation.vertex[1]))/timeCounter;
	tempVelocity.vector[2]= ((parent->location.vertex[2])-(parent->prevLocation.vertex[2]))/timeCounter;

	//Now emit the particle from a location between the last
	//known location, and the current location.  And don't
	//worry, this function is almost done.  Its mostly comments
	//if you look at it. Nice to know I have wasted a lot of
	//time typing these comments for you.  Blah. :)
	location.vertex[0]= (parent->prevLocation.vertex[0])+tempVelocity.vector[0]*RANDOM_FLOAT*timeCounter;
	location.vertex[1]= (parent->prevLocation.vertex[1])+tempVelocity.vector[1]*RANDOM_FLOAT*timeCounter;
	location.vertex[2]= (parent->prevLocation.vertex[2])+tempVelocity.vector[2]*RANDOM_FLOAT*timeCounter;

	//Now a simple randomization of the point that the particle
	//is emitted from.
	location.vertex[0]+=(pickaRandom((parent->spreadMin), (parent->spreadMax)))/(parent->spreadFactor);
	location.vertex[1]+=(pickaRandom((parent->spreadMin), (parent->spreadMax)))/(parent->spreadFactor);
	location.vertex[2]+=(pickaRandom((parent->spreadMin), (parent->spreadMax)))/(parent->spreadFactor);

	//Update the previous location so the next update can 
	//remember where we were
	(parent->prevLocation.vertex[0])=(parent->location.vertex[0]);
	(parent->prevLocation.vertex[1])=(parent->location.vertex[1]);
	(parent->prevLocation.vertex[2])=(parent->location.vertex[2]);

	//The emitter has a direction.  This is where we find it:
	randomYaw  = (float)(RANDOM_FLOAT*PI*2.0f);
	randomPitch= (float)(DEG_TO_RAD(RANDOM_FLOAT*((parent->angle))));

	//The following code uses spherical coordinates to randomize
	//the velocity vector of the particle
	velocity.vector[0]=(cosf(randomPitch))*(parent->velocity.vector[0]);
	velocity.vector[1]=(sinf(randomPitch)*cosf(randomYaw))*(parent->velocity.vector[1]);
	velocity.vector[2]=(sinf(randomPitch)*sinf(randomYaw))*(parent->velocity.vector[2]);

	//Velocity at this point is just a direction (normalized
	//vector) and needs to be multiplied by the speed 
	//component to be legit.
	newSpeed= ((parent->speed)+(RANDOM_FLOAT*(parent->speedCounter)));
	CHECK_RANGE(newSpeed, MIN_SPEED, MAX_SPEED);
	velocity.vector[0]*= newSpeed;
	velocity.vector[1]*= newSpeed;
	velocity.vector[2]*= newSpeed;

	SetParentSystem(parent);
}

//------------------------------------------------------------------//
//- bool PARTICLE:: Update(float) ----------------------------------//
//------------------------------------------------------------------//
//- Description: This function updates the class's particle.	   -//
//-				 Nothing too hard about that, eh?				   -//
//------------------------------------------------------------------//
bool PARTICLE::Update(float timeCounter)
{
	static VERTEX attractLocation;
	static VECTOR attractNormal;

	//Age the particle by the time counter
	age+= timeCounter;

	if(age>=dyingAge)
	{
		//Kill the particle. NOOOOOO... Ah, well, we have enough of
		//them, I probably won't even be able to tell the difference
		age=-1.0f;
		return false;
	}
	
	//Set the particle's previous location with the location that
	//will be the old one by the time we get through this function
	prevLocation.vertex[0]=location.vertex[0];
	prevLocation.vertex[1]=location.vertex[1];
	prevLocation.vertex[2]=location.vertex[2];

	//Move the particle's current location
	location.vertex[0]+= velocity.vector[0]*timeCounter;
	location.vertex[1]+= velocity.vector[1]*timeCounter;
	location.vertex[2]+= velocity.vector[2]*timeCounter;

	//Update the particle's velocity by the gravity vector by time.
	velocity.vector[0]+= (Parent->gravity.vector[0]*timeCounter);
	velocity.vector[1]+= (Parent->gravity.vector[1]*timeCounter);
	velocity.vector[2]+= (Parent->gravity.vector[2]*timeCounter);

	//Hehehe, sounds kewl.  If parent is attracting....  Well, I have
	//*NEVER* seen an attracting parent... Thats just wrong
	if(Parent->IsAttracting())	
	{
		//Find out where our Parent is located so we can track it
		attractLocation.vertex[0]=Parent->GetLocation(GET_X);
		attractLocation.vertex[1]=Parent->GetLocation(GET_Y);
		attractLocation.vertex[2]=Parent->GetLocation(GET_Z);

		//Calculate the vector between the particle and the attractor
		attractNormal.vector[0]= attractLocation.vertex[0]-location.vertex[0]; 
		attractNormal.vector[1]= attractLocation.vertex[1]-location.vertex[1]; 
		attractNormal.vector[2]= attractLocation.vertex[2]-location.vertex[2]; 

		//We can turn off attraction for certain axes to create 
		//some kewl effects (such as a tornado!)
		glNormal3fv(attractNormal.vector);

		//If you decide to use this simple method you really should use a variable multiplier
		//instead of a hardcoded value like 25.0f
		velocity.vector[0]+= attractNormal.vector[0]*5.0f*timeCounter;
		velocity.vector[1]+= attractNormal.vector[1]*5.0f*timeCounter;
		velocity.vector[2]+= attractNormal.vector[2]*5.0f*timeCounter;
	}

	//Adjust the current color (if you didn't realize that, 
	//then I wouldn't be surprised if you got done with your first
	//"Hello World!" program this morning... Sick and twisted)
	color[0]+= colorCounter[0] *timeCounter;
	color[1]+= colorCounter[1] *timeCounter;
	color[2]+= colorCounter[2] *timeCounter;

	//Adjust the alpha values (for transparency)
	alpha+= alphaCounter*timeCounter;

	//Adjust current size 
	size+= sizeCounter*timeCounter;

	//Annnnnddddd finally, set our color vector's spot #4 to the alpha,
	//because, things are a lot quick if we can use OpenGL's vector
	//color function.
	color[3]=alpha;

	return true;	//Yeee-aahhhhh buddy!
}