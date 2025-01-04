// g_tex.cpp
// texture loading and manipulations

#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
/*
 * getint and getshort arehelp functions to load the bitmap byte by byte on 
 * SPARC platform.
 * I've got them from xv bitmap load routinebecause the original bmp loader didn't work
 * I've tried to change as less code as possible.
 */

static unsigned int getint(FILE *fp)
{
  int c, c1, c2, c3;

  /* get 4 bytes*/
  c = getc(fp);  
  c1 = getc(fp);  
  c2 = getc(fp);  
  c3 = getc(fp);
  
  return ((unsigned int) c) +   
    (((unsigned int) c1) << 8) + 
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp)
{
  int c, c1;
  
  /*get 2 bytes*/
  c = getc(fp);  
  c1 = getc(fp);
 
  return ((unsigned int) c) + (((unsigned int) c1) << 8);
 
}
    

/* quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  */
/* See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.*/

int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 /* size of the image in bytes.*/
    unsigned long i;                    /* standard counter.*/
    unsigned short int planes;          /* number of planes in image (must be 1) */
    unsigned short int bpp;             /* number of bits per pixel (must be 24)*/
    char temp;                          /* temporary color storage for bgr-rgb conversion.*/

    /* make sure the file is there.*/
    if ((file = fopen(filename, "rb"))==NULL) {
      printf("File Not Found : %s\n",filename);
      return 0;
    }
    
    /* seek through the bmp header, up to the width/height:*/
    fseek(file, 18, SEEK_CUR);

    /* No 100% errorchecking anymore!!!*/

    /* read the width*/
    image->sizeX = getint (file);
    printf("Width of %s: %lu\n", filename, image->sizeX);
    
    /* read the height */
    image->sizeY = getint (file);
    printf("Height of %s: %lu\n", filename, image->sizeY);
    
    /* calculate the size (assuming 24 bits or 3 bytes per pixel).*/
    size = image->sizeX * image->sizeY * 3;

    /* read the planes*/
    planes = getshort(file);
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    /* read the bpp*/
    bpp = getshort(file);
    if (bpp != 24) {
	printf("Bpp from %s is not 24: %u\n", filename, bpp);
	return 0;
    }
	
    /* seek past the rest of the bitmap header.*/
    fseek(file, 24, SEEK_CUR);

    /* read the data. */
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 0;	
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
	printf("Error reading image data from %s.\n", filename);
	return 0;
    }

    for (i=0;i<size;i+=3) { /* reverse all of the colors. (bgr -> rgb)*/
	temp = image->data[i];
	image->data[i] = image->data[i+2];
	image->data[i+2] = temp;
    }
    
    /* we're done.*/
    return 1;
}


void make_Textures( GLuint texture[5] ) {	
    /* Load Texture*/
    Image *image1, *image2, *image3, 
		  *image4, *image5, *image6;//,
		  //*image7, *image8, *image9,
		  //*image10, *image11, *image12,
		  //*image13, *image14, *image15,
		  //*image16;
    
    /* allocate space for texture*/
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image2 = (Image *) malloc(sizeof(Image));
    if (image2 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image3 = (Image *) malloc(sizeof(Image));
    if (image3 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image4 = (Image *) malloc(sizeof(Image));
    if (image4 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

	image5 = (Image *) malloc(sizeof(Image));
    if (image5 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

	image6 = (Image *) malloc(sizeof(Image));
    if (image6 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
/*
	image7 = (Image *) malloc(sizeof(Image));
    if (image7 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image8 = (Image *) malloc(sizeof(Image));
    if (image8 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	
	image9 = (Image *) malloc(sizeof(Image));
    if (image9 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image10 = (Image *) malloc(sizeof(Image));
    if (image10 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image11 = (Image *) malloc(sizeof(Image));
    if (image11 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image12 = (Image *) malloc(sizeof(Image));
    if (image12 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image13 = (Image *) malloc(sizeof(Image));
    if (image13 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image14 = (Image *) malloc(sizeof(Image));
    if (image14 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image15 = (Image *) malloc(sizeof(Image));
    if (image15 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image16 = (Image *) malloc(sizeof(Image));
    if (image16 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
*/

    if (!ImageLoad("tile1.bmp", image1)) {
	exit(1);
    } 
	if (!ImageLoad("robot.bmp", image2)) {
	exit(1);
    }
	if (!ImageLoad("gun.bmp", image3)) {
	exit(1);
    } 
	if (!ImageLoad("grass.bmp", image4)) {
	exit(1);
    }
	if (!ImageLoad("crate.bmp", image5)) {
	exit(1);
    }

	if (!ImageLoad("brick.bmp", image6)) {
	exit(1);
    }
	/*
	if (!ImageLoad("brick.bmp", image7)) {
	exit(1);
    }
	if (!ImageLoad("wierdbrick.bmp", image8)) {
	exit(1);
    }
	if (!ImageLoad("crate.bmp", image9)) {
	exit(1);
    }

	if (!ImageLoad("tile3.bmp", image10)) {
	exit(1);
    }
	if (!ImageLoad("tile3.bmp", image11)) {
	exit(1);
    }
	if (!ImageLoad("tile3.bmp", image12)) {
	exit(1);
    }
	if (!ImageLoad("tile3.bmp", image13)) {
	exit(1);
    }
	if (!ImageLoad("tile3.bmp", image14)) {
	exit(1);
    }
	if (!ImageLoad("tile3.bmp", image15)) {
	exit(1);
    }
	if (!ImageLoad("tile3.bmp", image16)) {
	exit(1);
    }
*/
    glGenTextures(5, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);   
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    glBindTexture(GL_TEXTURE_2D, texture[1]);   
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image2->data);

    glBindTexture(GL_TEXTURE_2D, texture[2]);  
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image3->data);

    glBindTexture(GL_TEXTURE_2D, texture[3]);   
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image4->sizeX, image4->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image4->data); 

    glBindTexture(GL_TEXTURE_2D, texture[4]);   
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image5->sizeX, image5->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image5->data);

    glBindTexture(GL_TEXTURE_2D, texture[5]);   
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image6->sizeX, image6->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image6->data);

	free(image1->data);
	free(image1);
	free(image2->data);
	free(image2);
	free(image3->data);
	free(image3);
	free(image4->data);
	free(image4);
	free(image5->data);
	free(image5);
	free(image6->data);
	free(image6);
/*
	free(image7->data);
	free(image7);
	free(image8->data);
	free(image8);
	free(image9->data);
	free(image9);

	free(image10->data);
	free(image10);
	free(image11->data);
	free(image11);
	free(image12->data);
	free(image12);
	free(image13->data);
	free(image13);
	free(image14->data);
	free(image14);
	free(image15->data);
	free(image15);
	free(image16->data);
	free(image16);
	*/
}