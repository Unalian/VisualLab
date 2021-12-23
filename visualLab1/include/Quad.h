#pragma once

#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>

class Quad
{
public:
		
		Quad()
		{  glBegin(GL_QUADS); }
	   ~Quad()
	    {}
		
void	addPoint(float x, float y, float z)
		{  glVertex3f(x,y,z); }

void	addNormal(float* n)
		{  glNormal3f(n[0],n[1],n[2]); }
			
void    draw()
		{ glEnd(); }						  
};			 	




