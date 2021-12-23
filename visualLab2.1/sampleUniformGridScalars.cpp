

#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>//GLUT library
#include "UniformGrid.h"
#include "ColormapRenderer.h"
#include <iostream>										
#include <math.h>										//For expf()



float Xmin	= -5, Xmax	= 5;							//����֮��Χ���䶨��ͬʵ��1
float Ymin	= -5, Ymax	= 5;							
int   Nx	= 30, Ny	= 20;							//x��y�����������


float fov	 = 60;										//͸��ͶӰ������ͬʵ��1
float aspect = 1; 
float z_near = 0.1; 
float z_far  = 30;

float eye_x = 8, eye_y = 8, eye_z = 8;				   //������������
float c_x   = 0,  c_y   = 0,  c_z   = 0;
float up_x  = 0,  up_y  = 0,  up_z  = 1;


float f(float x, float y)								//�����ӻ��ĺ�����ͬʵ��1 
{														
	float ret = 8*expf(-(x*x+y*y)/5);
	return ret;
}


UniformGrid			grid(Nx,Ny,Xmin,Ymin,Xmax,Ymax);
ColormapRenderer	renderer;
int					drawing_style = 0;//ȫ�ֱ�����������ʾ������
int					colormap      = 0;//ȫ�ֱ���������ɫ��ͼ������


void viewing(int W, int H)								//�ص�������ͬʵ��1
{
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity ();										
	gluLookAt(eye_x,eye_y,eye_z,c_x,c_y,c_z,up_x,up_y,up_z);

	glMatrixMode (GL_PROJECTION);						
	glLoadIdentity ();
	gluPerspective(fov,float(W)/H,z_near,z_far);

	glViewport(0,0,W,H);								
}



void draw()												//�ص�����������grid
{
	renderer.draw(grid);
}


void keyboard(unsigned char c,int,int)
{
    switch (c) {
        case ' ':{
            drawing_style = (++drawing_style) % 4;
            renderer.setDrawingStyle((ColormapRenderer::DRAW_STYLE)drawing_style);
            break;
        }
        case 'c':{
            colormap = (++colormap) % 3;
            renderer.setColormapType((ColormapRenderer::COLORMAP_TYPE)colormap);
            break;
        }
    }
    glutPostRedisplay();
}	


void createGrid()										//����������Ӧ�ı�����ֵ
{														
	for(int i=0;i<grid.numPoints();++i)
	{
	   float p[2];				
	   grid.getPoint(i,p);								//��ȡ��ǰgrid point������
	   
	   float value = f(p[0],p[1]);						
	   
	   grid.pointScalars().setC0Scalar(i,value);		//�洢������ֵ
	}   
}


int main(int argc,char* argv[])							//Main program
{

   glutInit(&argc, argv);								
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);			
   glutInitWindowSize(500,500);							
   glutCreateWindow("exp2");
														
   createGrid();
   
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(draw);									
   glutReshapeFunc(viewing);							
   glutMainLoop();										
   return 0;
}




