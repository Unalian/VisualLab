#include "./include/ColormapRenderer.h"
#include "./include/ScalarAttributes.h"
#include "./include/Grid.h"
#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>											//GLUT library
#include <algorithm>
#include <math.h>

#include <iostream>
#define GL_CLAMP_TO_EDGE 0x812F

using namespace std;

void rainbow(float value,float& R,float& G,float& B)	//��[0,1] ��Χ�ڵı���ֵӳ��Ϊ�ʺ�ɫ��ͼ�е���ɫ
{
   const float dx=0.8f;

   value = (6-2*dx)*value+dx;
   R = max(0.0f,(3-(float)fabs(value-4)-(float)fabs(value-5))/2);
   G = max(0.0f,(4-(float)fabs(value-2)-(float)fabs(value-4))/2);
   B = max(0.0f,(3-(float)fabs(value-1)-(float)fabs(value-2))/2);
}

void grayscale(float value,float& R,float& G,float& B)	//��[0,1] ��Χ�ڵı���ֵӳ��Ϊ�Ҷ�ͼ�еĻҶ�ֵ
{
   R = G = B = value;
}

void saturation(float value,float& R,float& G,float& B)	//��[0,1] ��Χ�ڵı���ֵӳ��Ϊ����ͼ�е���ɫ
{
   float color_r = 0;									//��׼��ɫ���壬�˴�Ϊ��ɫ	
   float color_g = 1;									//�����Լ���Ϊ������ɫ������֮!	
   float color_b = 0;
   
   if (value<0.5)										//�� [0,0.5]��Χ�ڱ仯: ����ӳ�䣬�Ӻ�ɫ����׼��ɫ.
   {   
	   R = 2*value*color_r;
	   G = 2*value*color_g;
	   B = 2*value*color_b;
   }
   else													//��[0.5,1]��Χ�ڱ仯: ����ӳ�䣬�ӻ�׼ɫ����ɫ.
   {
	   value = 2*(value-0.5);	
	   R = (1-value)*color_r + 1*value;
	   G = (1-value)*color_g + 1*value;
	   B = (1-value)*color_b + 1*value;
   }
}

void ColormapRenderer::mapToColor(float value,float& R,float& G,float& B)
{														//��valueӳ��Ϊһ����ɫ������ȡ���ڵ�ǰ����ɫ��ͼ��Ч
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


void ColormapRenderer::createTextures()
{
    glGenTextures(3, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int i = COLORMAP_GRAYSCALE; i <= COLORMAP_SATURATION; ++i){
        glBindTexture(GL_TEXTURE_1D, textureID[i]);
        const int size = 512;
        float textureImage[3*size];

        colormap_type = (COLORMAP_TYPE)i;
        for(int j = 0; j<size; ++j){
            float v = float(j)/(size-1);
            float R, G, B;
            mapToColor(v, R, G, B);
            textureImage[3*j] = R;
            textureImage[3*j+1] = G;
            textureImage[3*j+2] = B;
        }
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, textureImage);
    }
    colormap_type = (COLORMAP_TYPE)0;
}

void ColormapRenderer::drawPoints(Grid& g)				//��g����㴦���ӻ���scalar values
{
	ScalarAttributes& scalars = g.pointScalars();		// ��ȡ����ֵ�ķ�Χ   
	float scalar_min = scalars.getMin();
	float scalar_max = scalars.getMax();

	glPointSize(3);										//1.   ���������еĶ��㣬�����ĳߴ�
	glBegin(GL_POINTS);									//2.   ��ʼ��OpenGL point-set��ʽ����������	
	for (int i = 0; i<g.numPoints(); ++i)
	{
		float v[2];
		g.getPoint(i, v);								//2.1. ��i-th vertex����ȡ����	

		float val = (g.pointScalars().getC0Scalar(i) - scalar_min) / (scalar_max - scalar_min);  //2.2. ��һ����[0,1]
		float r, g, b;
		mapToColor(val, r, g, b);							//2.3.  ����һ����ı���ֵӳ��Ϊĳ����ɫ 
		glColor3f(r, g, b);       							//2.4.  ����ɫ���� OpenGL����glColor3f����
		glVertex3f(v[0], v[1], 0);						    //2.5.  �����괫�� OpenGL����glVertex3f����
	}
	glEnd();											//3.    ��ɶԵ㼯����Ⱦ
}

void ColormapRenderer::drawC0Cells(Grid& g)
{														
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//1.�ƶ���Ⱦ�ķ�ʽ��Ҫ���cell
	glShadeModel(GL_FLAT);	//2.ʹ��ƽ����ɫ��ʽ������Ⱦ����һ��cell�ڲ�ֻ��һ����ɫ��

	ScalarAttributes& scalars = g.pointScalars();//3.��ȡscalar ��Χ
	float scalar_min = scalars.getMin();
	float scalar_max = scalars.getMax();

	for(int i=0;i<g.numCells();++i)
	{
		int   cell[10];
		
		int nvertices = g.getCell(i,cell);
		if (nvertices!=3 && nvertices!=4)


		if (nvertices==3)
			glBegin(GL_TRIANGLES);						
		else //nvertices==4	
			glBegin(GL_QUADS);
		

		float average_val = 0;
		for(int j=0;j<nvertices;++j)					
			average_val += g.pointScalars().getC0Scalar(cell[j]);
		average_val /= nvertices;
		average_val = (average_val-scalar_min)/(scalar_max-scalar_min);		

		float R,G,B;
		mapToColor(average_val,R,G,B);
		glColor3f(R,G,B);								

		for(int j=0;j<nvertices;++j)
		{
			float x[2];
			g.getPoint(cell[j],x);
			glVertex3f(x[0],x[1],0);
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

	for(int i=0;i<g.numCells();++i)
	{
		int   cell[10];
		
		int nvertices = g.getCell(i,cell);
		if (nvertices!=3 && nvertices!=4)				
		   continue;									

		if (nvertices==3)
			glBegin(GL_TRIANGLES);						
		else //nvertices==4	
			glBegin(GL_QUADS);
		
		for(int j=0;j<nvertices;++j)
		{
			float x[2];
			g.getPoint(cell[j],x);
						
			float value = (g.pointScalars().getC0Scalar(cell[j])-scalar_min)/(scalar_max-scalar_min);

			float R,G,B;
			mapToColor(value,R,G,B);
			glColor3f(R,G,B);
			glVertex3f(x[0],x[1],0);
		}	
		glEnd();
	}
}


void ColormapRenderer::drawC1TexCells(Grid& g)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_1D);

    glBindTexture(GL_TEXTURE_1D, textureID[colormap_type]);

    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    ScalarAttributes& scalars = g.pointScalars();
    float scalar_min = scalars.getMin();
    float scalar_max = scalars.getMax();

    for(int i = 0; i<g.numCells(); ++i){
        int cell[10];
        int nvertices = g.getCell(i, cell);
        if (nvertices != 3 && nvertices != 4)
            continue;
        if (nvertices == 3)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_QUADS);
        for(int j = 0; j<nvertices; ++j){
            float x[2];
            g.getPoint(cell[j], x);

            float value = (g.pointScalars().getC0Scalar(cell[j])-scalar_min)/(scalar_max-scalar_min);

            glTexCoord1f(value);
            glVertex3f(x[0], x[1], 0);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_1D);

}



void ColormapRenderer::draw(Grid& g)					//���ո�����style��������
{
	glClearColor(1,1,1,1);								//1. �����ɫ�����buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (draw_style)
	{
	case DRAW_POINTS:
		drawPoints(g);
		break;
	case DRAW_C0_CELLS:
		drawC0Cells(g);
		break;
	case DRAW_C1_CELLS:
		drawC1Cells(g);
		break;
	case DRAW_C1_TEX_CELLS: 
		drawC1TexCells(g);
		break;
	}			
	
	glutSwapBuffers();
}




