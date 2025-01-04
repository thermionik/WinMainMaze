//g_load.cpp

#include "g_util.h"


bool TEXTURE::LoadTGA(char* filename, GLfloat minFilter, GLfloat maxFilter)	
{    
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	//Uncompressed TGA header
	GLubyte		TGAcompare[12];					//Used to compare TGA header
	GLubyte		header[6];						//The first six useful bytes from the header
	GLuint		bytesPerPixel;					//Holds the bpp of the TGA
	GLuint		imageSize;						//Used to store image size while in RAM
	GLuint		temp;							//Temp variable
	GLuint		type=GL_RGBA;					//Set the default OpenGL mode to RBGA (32 BPP)

	FILE* file = fopen(filename, "rb");			// Open The TGA File

	if(file==NULL													   ||	// Does File Even Exist?
	   fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
	   memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0			   ||	// Does The Header Match What We Want?
	   fread(header,1,sizeof(header),file)!=sizeof(header))					// If So Read Next 6 Header Bytes
	{
		if(file==NULL)							// Did The File Even Exist? *Added Jim Strong*
		{
	//		errlog.glogOut("%s does not exist.", filename);
			return false;							
		}
		else
		{
			fclose(file);						// If anything failed, close the file
		//	errlog.glogOut("Could not load %s correctly, general failure.", filename);
			return false;						
		}
	}
	width = header[1] * 256 + header[0];		// Determine The TGA Width	(highbyte*256+lowbyte)
	height= header[3] * 256 + header[2];		// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(width	<=0	||							// Is The Width Less Than Or Equal To Zero
	   height<=0	||							// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))		// Is The TGA 24 or 32 Bit?
	{
		fclose(file);							// If Anything Failed, Close The File
	//	errlog.glogOut("%s's height or width is less than zero, or the TGA is not 24 or 32 bits.", filename);
		return false;							
	}

	bpp	 = header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel= bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize	 = width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	data= new GLubyte [imageSize];				// Reserve Memory To Hold The TGA Data

	if(data==NULL ||							// Does The Storage Memory Exist?
	   fread(data, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(data!=NULL)							// Was Image Data Loaded
			free(data);							// If So, Release The Image Data
		
	//	errlog.glogOut("Storate memory for %s does not exist or is corrupted.", filename);
		
		fclose(file);							// Close The File
		return false;							// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{										// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp	 =data[i];						// Temporarily Store The Value At Image Data 'i'
		data[i]	 = data[i + 2];					// Set The 1st Byte To The Value Of The 3rd Byte
		data[i+2]= temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);								//Close the file

	// Build A Texture From The Data
	glGenTextures(1, &ID);						//Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, ID);			//Bind the texture to a texture object 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);	//Filtering for if texture is bigger than should be
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);	//Filtering for if texture is smaller than it should be
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	if(bpp==24)									//Was the TGA 24 bpp?
		type=GL_RGB;							

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);

//	errlog.glogOut("Loaded %s correctly.", filename);
	return true;
}

bool TEXTURE::LoadBMP(char* filename, GLfloat minFilter, GLfloat maxFilter)
{
	FILE* file;									//The file pointer
	BITMAPFILEHEADER	file_header;			//The bitmap file header
	BITMAPINFOHEADER	info_header;
	int					imageIdx = 0;			//Image index counter
	unsigned char		tempRGB;				//Swap variable

	//Open filename in read only binary mode
	file= fopen(filename, "rb");
	if(file==NULL)
	{
	//	errlog.glogOut("Could not load %s", filename);
		return false;
	}

	//Read the bitmap file header
	fread(&file_header, sizeof(BITMAPFILEHEADER), 1, file);
	
	//Confirm that this is a bitmap by checking for the universal bitmap id
//	if(file_header.bfType != BITMAP_ID)
//		{
//		fclose(file);
//		errlog.glogOut("%s is not a .bmp", filename);
//		return false;
//		}

	//Read the bitmap information header in
	fread(&info_header, sizeof(BITMAPINFOHEADER), 1, file);

	//Advance the file pointer to the beginning of the bitmap data
	fseek(file, file_header.bfOffBits, SEEK_SET);

	//Allocate the bitmap image data
	data= new unsigned char [info_header.biSizeImage];

	//Confirm memory allocation
	if(!data)
	{
		free(data);
		fclose(file);
//		errlog.glogOut("Could not allocate memory for %s", filename);
		return false;
	}

	//Read in the bitmap image data
	fread(data, 1, info_header.biSizeImage, file);

	//Make sure bitmap image data was read
	if(data==NULL)
	{
		fclose(file);
	//	errlog.glogOut("Could not read %s", filename);
		return false;
	}
	
	width = info_header.biWidth;
	height= info_header.biWidth;
	bpp	  = info_header.biBitCount;

	//Swap the R and B values to get RGB since the bitmap color format is in BGR
	for(imageIdx = 0; imageIdx<(int)info_header.biSizeImage; imageIdx+=3)
	{
		tempRGB			= data[imageIdx];
		data[imageIdx]  = data[imageIdx + 2];
		data[imageIdx+2]= tempRGB;
	}

	//Close the file and return the bitmap image data
	fclose(file);

	//Build A Texture From The Data
	glGenTextures(1, &ID);						//Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, ID);			//Bind the texture to a texture object 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);	//Filtering for if texture is bigger than should be
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);	//Filtering for if texture is smaller than it should be
	
	if(bpp==32)
		bpp=GL_RGBA;

	if(bpp==24)									//Was the TGA 24 bpp?
		bpp=GL_RGB;							

	glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 0, bpp, GL_UNSIGNED_BYTE, data);

//	errlog.glogOut("Loaded %s correctly", filename);
	return true;
}


bool MD2::Load(char* filename)
{
	FILE* file;
	MD2_HEADER header;

	//Open the .md2 model file
	if((file= fopen(filename, "rb"))==NULL)
	{
//		S3Dlog.Output("Could not load %s correctly", filename);
		return false;
	}
	
	//Read in the header
	fread(&header, sizeof(MD2_HEADER), 1, file);

	//Create space for the frame information
	frames= new char[header.frameSize*header.numFrames];
	
	//Check to see if it allocated correctly
	if(frames==NULL)
		return false;

	//Zoom to the correct spot in the file
	fseek(file, header.offsetFrames, SEEK_SET);
	fread(frames, header.frameSize*header.numFrames, 1, file);

	//Create space for the GL command information (whether or not to use tri strips, or tri fans)
	glCommands= new long [header.numGlCommands];

	//Check to see if it allocated correctly
	if(glCommands==NULL)
		return false;

	//Zoom to the correct spot in the file
	fseek(file,		  header.offsetGlCommands, SEEK_SET);
	fread(glCommands, header.numGlCommands*sizeof(long), 1, file);

	//Move the important information from the header, to the permanent class info.
	numFrames	 = header.numFrames;
	numGlCommands= header.numGlCommands;
	frameSize	 = header.frameSize;
	numTriangles = header.numTriangles;

	//Close 'da friggin file mon
	fclose(file);	

	//Lets get outta here
//	S3Dlog.Output("Loaded %s correctly", filename);
	return true;
}

void MD2::Render(int numFrame)
{
	MD2_FRAME_PTR	currentFrame;
	MD2_MODELVERTEX vertList[100];
	VERTEX			v1;
	VERTEX			v2;
	VERTEX			v3;
	long*			command;
	float			texcoord[2];
	int				loop;
	int				vertIndex;
	int				type;
	int				numVertex;
	int				index;

	//Get the current frame and gl command information
	currentFrame= (MD2_FRAME*) ((char*)frames+frameSize*numFrame);
	command		= glCommands;

	//Make sure that the command doesn't equal 0, and if it doesn't lets start rendering!
	while((*command)!=0)
	{
		if(*command>0)	//This is a triangle strip
		{
			numVertex= *command; 
			command++; 
			type= 0;
		}
		else			//This is a triangle fan
		{
			numVertex= - *command; 
			command++; 
			type= 1;
		}

		if(numVertex<0)
			numVertex= -numVertex;

		index= 0;
		//Fill the vertex list information
		for(loop=0; loop<numVertex; loop++)
		{
			vertList[index].u= *((float*)command); 
			command++;
			vertList[index].v= *((float*)command); 
			command++;

			vertIndex= *command; 
			command++;

			vertList[index].x= ( (currentFrame->vertices[vertIndex].vertex[0]* 
								  currentFrame->scale[0])+ 
								  currentFrame->translate[0]);
			vertList[index].z= -((currentFrame->vertices[vertIndex].vertex[1]* 
								  currentFrame->scale[1])+ 
								  currentFrame->translate[1]);
			vertList[index].y= ( (currentFrame->vertices[vertIndex].vertex[2]* 
								  currentFrame->scale[2])+ 
								  currentFrame->translate[2]);
			index++;
		}

		//If the .md2 was optimized for use with triangle strips...
		if(type==0)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for(loop=0; loop<index; loop++)
			{
				v1.vertex[0]=(vertList[loop].x);
				v1.vertex[1]=(vertList[loop].y);
				v1.vertex[2]=(vertList[loop].z);
				texcoord[0]= vertList[loop].u;
				texcoord[1]= vertList[loop].v;
				glTexCoord2fv(texcoord);
				glVertex3fv(v1.vertex);
			}
			glEnd();		
		}

		//If the .md2 was made for use with triangle fans...
		else
		{
			glBegin(GL_TRIANGLE_FAN);
			for(loop=0; loop<index; loop++)
			{
				v1.vertex[0]=(vertList[loop].x);
				v1.vertex[1]=(vertList[loop].y);
				v1.vertex[2]=(vertList[loop].z);
				texcoord[0]= vertList[loop].u;
				texcoord[1]= vertList[loop].v;
				glTexCoord2fv(texcoord);
				v1.SendToOGL();
			}
			glEnd();	
		}
	}
}

//------------------------------------------------------------------//
//- void Animate(int, int, float) ----------------------------------//
//------------------------------------------------------------------//
//- Description: This function animates a model from the start     -//
//-				 frame, to the end frame, with the speed that you  -//
//-				 give (in percent).								   -//
//------------------------------------------------------------------//
//- I got the idea for this one from Dave Astle and Kevin Hawkins  -//
//- (two really cool guys), but I had to implement the code in it  -//
//- myself!														   -//
//------------------------------------------------------------------//
void MD2::Animate(int startFrame, int EndFrame, float percent)
{
	//Check to see if the current frame equals -1 (which means there was a 
	//sudden change of states)
	if(currentFrame==-1 || currentFrame>endFrame)
	{
		currentFrame= startFrame;
		nextFrame	= currentFrame+1;
		endFrame	= EndFrame;
	}

	//If the interpolation value is above one, then we advance to the next frame
	//of animation for the model
	if(interpolation>1.0f)
	{
		interpolation= 0.0f;
		currentFrame = nextFrame;
		nextFrame	 = currentFrame+1;
	}

	interpolation+=percent;
	Render(currentFrame);
}

//------------------------------------------------------------------//
//- void SetState(int) ---------------------------------------------//
//------------------------------------------------------------------//
//- Description: This sets the models state from one of the		   -//
//-				 predefined constants, the state will start		   -//
//-				 animating on the next frame (as long as you are   -//
//-				 using the Animate(...) function).				   -//
//------------------------------------------------------------------//
//- Ok, this one was all my idea, so no one to thank!!! HAHAHA! Ok -//
//- sorry, a little too much Mountain Dew for me! :P			   -//
//------------------------------------------------------------------//
void MD2::SetState(int state)
{	
	switch(state)
	{
		case IDLE1:
			stateStart	 = IDLE1_START;
			stateEnd	 = IDLE1_END;
			currentFrame =-1;
			break;
		case RUN:
			stateStart	 = RUN_START;
			stateEnd	 = RUN_END;
			currentFrame =-1;
			break;
		case SHOT_STAND:
			stateStart	 = SHOT_STAND_START;
			stateEnd	 = SHOT_STAND_END;
			currentFrame =-1;
			break;
		case SHOT_SHOULDER:
			stateStart	 = SHOT_SHOULDER_START;
			stateEnd	 = SHOT_SHOULDER_END;
			currentFrame =-1;
			break;
		case JUMP:
			stateStart	 = JUMP_START;
			stateEnd	 = JUMP_END;
			currentFrame =-1;
			break;
		case IDLE2:
			stateStart	 = IDLE2_START;
			stateEnd	 = IDLE2_END;
			currentFrame =-1;
			break;
		case SHOT_FALLDOWN:
			stateStart	 = SHOT_FALLDOWN_START;
			stateEnd	 = SHOT_FALLDOWN_END;
			currentFrame =-1;
			break;
		case IDLE3:
			stateStart	 = IDLE3_START;
			stateEnd	 = IDLE3_END;
			currentFrame =-1;
			break;
		case IDLE4:
			stateStart	 = IDLE4_START;
			stateEnd	 = IDLE4_END;
			currentFrame =-1;
			break;
		case CROUCH:
			stateStart	 = CROUCH_START;
			stateEnd	 = CROUCH_END;
			currentFrame =-1;
			break;
		case CROUCH_CRAWL:
			stateStart	 = CROUCH_CRAWL_START;
			stateEnd	 = CROUCH_CRAWL_END;
			currentFrame =-1;
			break;
		case CROUCH_IDLE:
			stateStart	 = CROUCH_IDLE_START;
			stateEnd	 = CROUCH_IDLE_END;
			currentFrame =-1;
			break;
		case CROUCH_DEATH:
			stateStart	 = CROUCH_DEATH_START;
			stateEnd	 = CROUCH_DEATH_END;
			currentFrame =-1;
			break;
		case DEATH_FALLBACK:
			stateStart	 = DEATH_FALLBACK_START;
			stateEnd	 = DEATH_FALLBACK_END;
			currentFrame =-1;
			break;
		case DEATH_FALLFORWARD:
			stateStart	 = DEATH_FALLFORWARD_START;
			stateEnd	 = DEATH_FALLFORWARD_END;
			currentFrame =-1;
			break;
		case DEATH_FALLBACKSLOW:
			stateStart	 = DEATH_FALLBACKSLOW_START;
			stateEnd	 = DEATH_FALLBACKSLOW_END;
			currentFrame =-1;
			break;
	}	
}


//------------------------------------------------------------------//
//------------------------------------------------------------------//
//- MS3D CODE ------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//

bool MS3D::Load(char* filename)
{
	FILE* file;
	MS3D_HEADER header;
	char texture[128];
	int loop;

	//Open the .ms3d model file
	if((file= fopen(filename, "rb"))==NULL)
	{
//		S3Dlog.Output("Could not load %s correctly", filename);
		return false;
	}

	//Read the header data in
	fread(&header.id, sizeof(char), 10, file);
	fread(&header.version, 1, sizeof(int), file);

	if(strncmp(header.id, "MS3D000000", 10)!=0)
		return false;
	if(header.version!=3 && header.version!=4)
		return false;

	//Read the vertex data in
	fread(&numVertices, sizeof(unsigned short), 1, file);
	vertices= new MS3D_VERTEX [numVertices];
	for(loop=0; loop<numVertices; loop++)
	{
		fread(&vertices[loop].flags,	sizeof(BYTE),  1, file);
		fread( vertices[loop].vertex,	sizeof(float), 3, file);
		fread(&vertices[loop].boneID,	sizeof(char),  1, file);
		fread(&vertices[loop].refCount, sizeof(BYTE),  1, file);
	}

	//Read the triangle data in
	fread(&numTriangles, sizeof(unsigned short), 1, file);
	triangles= new MS3D_TRIANGLE [numTriangles];
	for(loop=0; loop<numTriangles; loop++)
	{
		fread(&triangles[loop].flags,			 sizeof(unsigned short), 1, file);
		fread( triangles[loop].vertexIndices,	 sizeof(unsigned short), 3, file);
		fread( triangles[loop].vertexNormals[0], sizeof(float),			 3, file);
		fread( triangles[loop].vertexNormals[1], sizeof(float),		 	 3, file);
		fread( triangles[loop].vertexNormals[2], sizeof(float),			 3, file);
		fread( triangles[loop].u,				 sizeof(float),			 3, file);
		fread( triangles[loop].v,				 sizeof(float),			 3, file);
		fread(&triangles[loop].smoothingGroup,	 sizeof(unsigned char),  1, file);
		fread(&triangles[loop].groupIndex,		 sizeof(unsigned char),  1, file);
	}

	//Read the group data in
	fread(&numGroups, sizeof(unsigned short), 1, file);
	groups= new MS3D_GROUP [numGroups];
	for(loop=0; loop<numGroups; loop++)
	{
		fread(&groups[loop].flags,			 sizeof(unsigned char),  1,	 file);
		fread( groups[loop].name,			 sizeof(char),			 32, file);
		fread(&groups[loop].numTriangles,	 sizeof(unsigned short), 1,	 file);

		groups[loop].triangleIndices=new unsigned short [groups[loop].numTriangles];

		fread( groups[loop].triangleIndices, sizeof(unsigned short), groups[loop].numTriangles,file);
		fread(&groups[loop].materialIndex,	 sizeof(char), 1, file);
	}

	//Read the material information in
	fread(&textures, sizeof(unsigned short), 1, file);
	textures= new MS3D_TEXTURE[numTextures];
	for(loop=0; loop<numTextures; loop++)
	{
		fread( textures[loop].name,			sizeof(char),  32,  file);
		fread( textures[loop].ambient,		sizeof(float), 4,   file);
		fread( textures[loop].diffuse,		sizeof(float), 4,   file);
		fread( textures[loop].specular,		sizeof(float), 4,   file);
		fread( textures[loop].emissive,		sizeof(float), 4,   file);
		fread(&textures[loop].shininess,	sizeof(float), 1,   file);
		fread(&textures[loop].transparency, sizeof(float), 1,   file);
		fread(&textures[loop].mode,			sizeof(char),  1,   file);
		fread(&texture,						sizeof(char),  128, file);	//To keep things going (dummy variable)
		fread( textures[loop].alphaMap,		sizeof(char),  128, file);
	}

	//"Soooo loonnnnnggggg..."
//	S3Dlog.Output("Loaded %s correctly", filename);
	return true;
}

void MS3D::Render(bool texture_flag)
{
	int loop1;
	int loop2;
	int loop3;

	// Draw by group
	for(loop1=0; loop1<numGroups; loop1++ )
	{
		if(texture_flag)
		{
			int materialIndex= groups[loop1].materialIndex;
			if(materialIndex>=0)
			{
				glMaterialfv(GL_FRONT, GL_AMBIENT,  textures[materialIndex].ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE,  textures[materialIndex].diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, textures[materialIndex].specular);
				glMaterialfv(GL_FRONT, GL_EMISSION, textures[materialIndex].emissive);
				glMaterialf( GL_FRONT, GL_SHININESS,textures[materialIndex].shininess);
			}
			else
			{
				// Material properties?
				glDisable(GL_TEXTURE_2D);
			}
		}

		glBegin(GL_TRIANGLES);
		{
			for(loop2=0; loop2<groups[loop1].numTriangles; loop2++)
			{
				int triangleIndex		=  groups[loop1].triangleIndices[loop2];
				const MS3D_TRIANGLE* tri= &triangles[triangleIndex];

				for(loop3=0; loop3<3; loop3++)
				{
					int index= tri->vertexIndices[loop3];

					glNormal3fv( tri->vertexNormals[loop3]);
					if(texture_flag)
						glTexCoord2f(tri->u[loop3], tri->v[loop3]);
					glVertex3fv(vertices[index].vertex);
				}
			}
		}
		glEnd();
	}
}