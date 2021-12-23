#include "ColormapRenderer.h"
#include "ScalarAttributes.h"
#include "Grid.h"
#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>								//GLUT library
#include <algorithm>
#include <math.h>


using namespace std;



void rainbow(float value,float& R,float& G,float& B)
{
    const float dx = 0.8f;
    value = (6-2*dx) * value + dx;
    R = max(0.0f, (3 - (float)fabs(value-4)- (float)fabs(value-5))/2);
    G = max(0.0f, (4 - (float)fabs(value-2)- (float)fabs(value-4))/2);
    B = max(0.0f, (3 - (float)fabs(value-1)- (float)fabs(value-2))/2);
}

void grayscale(float value,float& R,float& G,float& B)
{
  R = G = B = value;
}

void saturation(float value,float& R,float& G,float& B)
{
    float color_r = 0;
    float color_g = 1;
    float color_b = 0;
    if (value < 0.5){
        R = 2*value*color_r;
        G = 2*value*color_g;
        B = 2*value*color_b;
    }else{
        value = 2*(value-0.5);
        R = (1-value)*color_r + 1* value;
        G = (1-value)*color_g + 1* value;
        B = (1-value)*color_b + 1* value;
    }

}


void ColormapRenderer::mapToColor(float value,float& R,float& G,float& B)
{														
	switch (colormap_type)
	{
	case COLORMAP_GRAYSCALE: 
		grayscale(value,R,G,B);
		break;
	case COLORMAP_RAINBOW: 
		rainbow(value,R,G,B);
		break;
	case COLORMAP_SATURATION: 
		saturation(value,R,G,B);
		break;
	}
}		

void ColormapRenderer::drawGrid(Grid& g)
{
	glColor3f(0,0,0);									//1.   ʹ�ú�ɫ����cells
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);			//     ֻ����cell�����

	for(int i=0;i<g.numCells();++i)						//2. �������е�cell�����������߼����Լ��������ͼ
	{
		int   cell[10];									
		float x[2];
		
		int nvertices = g.getCell(i,cell);				
		if (nvertices!=3 && nvertices!=4)				
		   continue;									

		if (nvertices==3)								
			glBegin(GL_TRIANGLES);						
		else //nvertices==4	
			glBegin(GL_QUADS);							
		
		for(int j=0;j<nvertices;++j)					
		{
			g.getPoint(cell[j],x);						
			glVertex3f(x[0],x[1],0);					
		}	
		glEnd();                                       
	}

	glPointSize(3);										
	glColor3f(1,0,0);
	glBegin(GL_POINTS);									
	for(int i=0;i<g.numPoints();++i)						
	{
		float v[2];
		g.getPoint(i,v);										
		glVertex3f(v[0],v[1],0);						
	}
	glEnd();											
}



void ColormapRenderer::drawPoints(Grid& g)				//��g�Ķ���㴦���ӻ���scalar values
{
	ScalarAttributes& scalars = g.pointScalars();		// ��ȡ����ֵ�ķ�Χ   
	float scalar_min = scalars.getMin();
	float scalar_max = scalars.getMax();
	
	glPointSize(3);										//1.   ���������еĶ��㣬�����ĳߴ�
	glBegin(GL_POINTS);									//2.   ��ʼ��OpenGL point-set��ʽ����������	
	for(int i=0;i<g.numPoints();++i)						
	{
		float v[2];
		g.getPoint(i,v);								//2.1. ��i-th vertex����ȡ����	
		
		float val = (g.pointScalars().getC0Scalar(i)-scalar_min)/(scalar_max-scalar_min);//2.2. ��һ����[0,1]
		float r,g,b;									
		mapToColor(val,r,g,b);							//2.3.  ����һ����ı���ֵӳ��Ϊĳ����ɫ 
		glColor3f(r,g,b);								//2.4.  ����ɫ���� OpenGL
		glVertex3f(v[0],v[1],0);						//2.5.  �����괫�� OpenGL
	}
	glEnd();											//3.    ��ɶԵ㼯����Ⱦ
}


void ColormapRenderer::drawC0Cells(Grid& g)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);

    ScalarAttributes& scalars = g.pointScalars();
    float scalar_min = scalars.getMin();
    float scalar_max = scalars.getMax();

    for (int i = 0; i < g.numCells();++i){
        int cell[10];
        int nvertices = g.getCell(i, cell);
        if (nvertices != 3 && nvertices != 4)
            continue;
        if (nvertices == 3)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_QUADS);

        float average_val = 0;
        for (int j = 0; j < nvertices; ++j) {
            average_val += g.pointScalars().getC0Scalar(cell[j]);
        }
        average_val /= nvertices;
        average_val = (average_val - scalar_min)/(scalar_max - scalar_min);
        float R,G,B;
        mapToColor(average_val, R, G, B);
        glColor3f(R, G, B);

        for (int j = 0; j < nvertices; ++j) {
            float  x[2];
            g.getPoint(cell[j], x);
            glVertex3f(x[0], x[1], 0);
        }
        glEnd();
    }
 
}


void ColormapRenderer::drawC1Cells(Grid& g)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);

    ScalarAttributes& scalars = g.pointScalars();
    float scalar_min = scalars.getMin();
    float scalar_max = scalars.getMax();

    for (int i = 0; i < g.numCells();++i){
        int cell[10];
        int nvertices = g.getCell(i, cell);
        if (nvertices != 3 && nvertices != 4)
            continue;
        if (nvertices == 3)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_QUADS);

        for(int j = 0; j < nvertices; ++j){
           float x[2];
           g.getPoint(cell[j], x);
           float value = (g.pointScalars().getC0Scalar(cell[j])-scalar_min)/(scalar_max - scalar_min);

           float R,G,B;
           mapToColor(value, R, G, B);
           glColor3f(R, G, B);
           glVertex3f(x[0], x[1], 0);
        }
        glEnd();
    }
}



void ColormapRenderer::draw(Grid& g)					//����draw_style���� grid 'g' 
{
	glClearColor(1,1,1,1);								
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //1. ��� frame �� depth buffers

	switch (draw_style)
	{
	case DRAW_GRID: 
		drawGrid(g);
		break;
	case DRAW_POINTS:
		drawPoints(g);
		break;
	case DRAW_C0_CELLS:
		drawC0Cells(g);
		break;
	case DRAW_C1_CELLS:
		drawC1Cells(g);
		break;
	}			
	
	glutSwapBuffers();
}




