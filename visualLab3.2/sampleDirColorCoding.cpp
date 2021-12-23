#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>									//GLUT library
#include "./include/UniformGrid.h"
#include "./include/GlyphRenderer.h"
#include "./include/ColormapRenderer.h"
#include "./include/DirCodingRenderer.h"
#include <iostream>										
#include <math.h>										//For expf()


float Xmin	= -5, Xmax	= 5;							//x ���������Χ
float Ymin	= -5, Ymax	= 5;							//y ���������Χ
int   Nx	= 40, Ny	= 40;							//x y�����������

float f(float x, float y)								//���ӻ�һ���򵥺��� 
{														
	return 8*expf(-(x*x+y*y)/5);
}

UniformGrid			grid(Nx,Ny,Xmin,Ymin,Xmax,Ymax);
GlyphRenderer*		glyph_renderer = 0;
//ColormapRenderer*	dir_renderer   = 0;//��һ�е�һ������
DirCodingRenderer*	dir_renderer   = 0;//��һ�еڶ�������
int					colormap       = ColormapRenderer::COLORMAP_RAINBOW;  //��һ����colormap���õ���ʵ��2�е�ɫ��ͼ���ڶ����������Լ������
int					scaling_mode   = 0;
float				alpha          = 0;
int					draw_glyphs    = 1;

void viewing(int W, int H)								//�ص�������ͬʵ��1
{
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();										

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	gluOrtho2D(Xmin,Xmax,Ymin,Ymax);
	glDisable(GL_DEPTH_TEST);

	glViewport(0,0,W,H);								
}



void draw()												//��Ⱦ grid ���ص�����
{
    if (draw_glyphs)									//ȡ���� 'draw_glyphs', �Ƿ��� glyphs չʾʸ��
		glyph_renderer->draw(grid);						
	else												//��������directional color-coding����ɫ��ʾʸ���ķ���
		dir_renderer->draw(grid);
}	



float direction2angle(Point3d v)						//��2D vector �任Ϊ���򣨽Ƕȣ�
{														
	v.normalize();										

	float cosa = v.x;
	float sina = v.y;
	
	float a;
	if (sina>=0)
		a = acos(cosa);
	else
		a = 2*3.14159 - acos(cosa);
	
	if (a>3.14159) a = 3.14159 - fabs(a-3.14159);
		
	return a;
}


void createGrid(float alpha)//Ϊgrid��������������ֻ����������ϲ��� f(x,y)���ݶȲ�������ת
{														
	for(int i=0;i<grid.numPoints();++i)					
	{
	   float p[2];				
	   grid.getPoint(i,p);								//��ȡ��ǰ����������
	   
	   float scalar = f(p[0],p[1]);						//���㵱ǰ���f(x,y)
		
	   float delta    = 0.001*(Xmax-Xmin);	  //��4�д������f(x,y)���ݶȣ�ʹ�����޲��ģ��
	   float scalar_x = f(p[0]+delta,p[1]);	
	   float scalar_y = f(p[0],p[1]+delta);		   
	   Point3d gradient = Point3d((scalar_x-scalar)/delta,(scalar_y-scalar)/delta,0);

	   float alpha2 = sin(alpha)*gradient.norm();		//�����alpha2�ĺ���
	   
	   Point3d vec;										//���ݶ���תalpha2, ����ʸ��
	   vec.x = cos(alpha2)*gradient.x - sin(alpha2)*gradient.y;
	   vec.y = sin(alpha2)*gradient.x + cos(alpha2)*gradient.y;
	   	   
	   grid.pointVectors().setC0Vector(i,vec);			//�����ʸ���������ɵ�vec

	   float direction = direction2angle(vec);			//������ת�ݶȵķ���[0,PI]
	   grid.pointScalars().setC0Scalar(i,direction);	//�洢����������ֵΪ����ı���
														//�������ǿ��ӻ���ֻ��ʸ���ķ���
	}   
}



void keyboard(unsigned char c,int,int)					//�����¼��ص�����
{
	switch(c)
	{

	/*case 'c':
		{
		  colormap = (++colormap) % 3;					//directional color coding renderer ʹ�ú�һ��colormap
		  dir_renderer->setColormapType((ColormapRenderer::COLORMAP_TYPE)colormap);
		  break;
		}*/
		
	case 'v':											//�ı�ʸ�������п��ӻ�
		{
		  alpha = (alpha + 3.14159/50);
		  createGrid(alpha);
		  break;
		}
	case ' ':											//�Ƿ�ʹ�ñ�Ƿ����п��ӻ��Ŀ���
		{
		  draw_glyphs = !draw_glyphs;
		  break;
		}  		
	}
				
	glutPostRedisplay();    
}	



int main(int argc,char* argv[])								//Main program
{

   glutInit(&argc, argv);									//1.  ��ʼ�� GLUT toolkit
   glutInitDisplayMode(GLUT_RGB| GLUT_DOUBLE|GLUT_DEPTH);	//2.  ��֪ GLUT ������RGB framebuffer �� Z-buffer �Ĵ���
   glutInitWindowSize(500,500);								//3.  ��֪ GLUT ���ڵĳߴ�
   glutCreateWindow("Lab3.2");
															//4. ��������
   
   
   glyph_renderer = new GlyphRenderer();					//5. ���� glyph renderer, ��ʼ������
   float cell_size = (grid.getCellSize1()+grid.getCellSize2())/2.0;	
   glyph_renderer->setDrawingStyle(GlyphRenderer::DRAW_CONES);
   glyph_renderer->setScalingFactor(cell_size);
   glyph_renderer->setColormapType(ColormapRenderer::COLORMAP_SATURATION);
   glyph_renderer->setScalingMode(GlyphRenderer::SCALE_NORMALIZE);
  

//   dir_renderer   = new ColormapRenderer();					//6.  ����colormap renderer, ��ʼ������
//   dir_renderer->setDrawingStyle(ColormapRenderer::DRAW_C1_TEX_CELLS);
//   dir_renderer->setColormapType((ColormapRenderer::COLORMAP_TYPE)colormap);
    dir_renderer = new DirCodingRenderer();

   createGrid(alpha);										//7.  ����������ʸ��������grid �� vertices

   
   glutKeyboardFunc(keyboard);   
   glutDisplayFunc(draw);									//8.  Ϊ�������Ӽ����¼��ص�
   glutReshapeFunc(viewing);								//9.  Ϊ�������Ӹı�ߴ�ص�
   glutMainLoop();											//10. ��ʼ�¼�ѭ��
   
   return 0;
}




