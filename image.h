#ifndef image_h
#define image_h

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

int ImageLoad(char *filename, Image *image);
void make_Textures( GLuint texture[4] );

#endif
