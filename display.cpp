#include "main.h"


static void display(void)
{

   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


   glColor3f (1.0, 1.0, 1.0);
   
   glLoadIdentity ();             /* clear the matrix */

   gluLookAt (LocX, LocY, LocZ, rotX, rotY, rotZ, 0.0, 1.0, 0.0); //this controls the nav.
   update();
   make_Maze(); // draw the maze
   /* draw gun */
   if( flame ) { // Draw gun shooting
	    //fuel = max_fuel;
		glClear(GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(0, -1.5, -2);
		glColor3f( 0.0, 0.0, 0.0 );
		make_Gun();
		flame = false;
   }
   else {
		glClear(GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glColor3f( 0.0, 0.0, 0.0 );
		glTranslatef(0, -1.2, -2);
		make_Gun();
		
   }

     /* draw fuel indicator */
   glClear(GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslatef(0, -2, -5);
   glScalef(10*((GLfloat)fuel/max_fuel), 0.2f, 0.2f);
   glRotatef(45, 1, 0, 0);
   make_Life();


   
   glFlush ();
   glutSwapBuffers();
   glFinish();
}

