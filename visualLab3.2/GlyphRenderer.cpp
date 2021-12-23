#include "./include/GlyphRenderer.h"
#include "./include/ScalarAttributes.h"
#include "./include/VectorAttributes.h"
#include "./include/Grid.h"
#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>										//GLUT library
#include <algorithm>
#include <math.h>
#include <iostream>


using namespace std;


static float direction2angle(const Point3d& d)			//��2D vector �ķ��򣨽Ƕȣ���������Ƕ���[0,360]֮��
{
	Point3d x = d; x.normalize();

	float cosa = x.x;
	float sina = x.y;

	float a;
	if (sina >= 0)
		a = acos(cosa);
	else
		a = 2 * 3.14159 - acos(cosa);

	return 180 * a / 3.14159;
}




void GlyphRenderer::drawMagnitude(Grid& g)				//ͨ����ʾ�����Ĵ�С����ɫ��ͼ������������g
{
	VectorAttributes& vectors = g.pointVectors();		//��ȡʸ���Ĵ�С��Χ�������һ���� [0,1]
	float vector_min = vectors.getMin();
	float vector_max = vectors.getMax();

	glPointSize(3);										//1.    �ô��������
	glBegin(GL_POINTS);									//2.    ������ΪOpenGL ��point-set	
	for (int i = 0; i<g.numPoints(); ++i)
	{
		float v[2];
		g.getPoint(i, v);								//2.1.  ��ȡ i-th vertex ������	

		Point3d vec = g.pointVectors().getC0Vector(i); 	//2.2.  ��ȡʸ����ֵ���Լ����һ����ĳ���
		float length = (vec.norm() - vector_min) / (vector_max - vector_min);// ���ﲹ���һ���Ĵ���

		float r, g, b;									//
		mapToColor(length, r, g, b);			        //2.3.  ����һ�����ֵӳ��Ϊ��ɫ�����ﲹ����� 
		glColor3f(r, g, b);								//2.4.  ����ɫ���ݸ�OpenGL
		glVertex3f(v[0], v[1], 0);						//2.5.  �����괫�ݸ�OpenGL
	}
	glEnd();											//3.    �������� point-set
}


void GlyphRenderer::drawBalls(Grid& g)					//����״��Ƿ����ӻ�'g', ��С��ӳ�����Ĵ�С
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	VectorAttributes& vectors = g.pointVectors();		//��ȡʸ���Ĵ�С��Χ�������һ���� [0,1]
	float vector_min = vectors.getMin();
	float vector_max = vectors.getMax();

	glMatrixMode(GL_MODELVIEW);

	for (int i = 0; i<g.numPoints(); ++i)
	{
		float v[2];
		g.getPoint(i, v);								//2.1. ��ȡ i-th vertex ������	

		Point3d vec = g.pointVectors().getC0Vector(i);
		float length = (vec.norm() - vector_min) / (vector_max - vector_min);//������룬ͬǰ

		float r, g, b;
		mapToColor(length, r, g, b);						//2.2.  ����ֵӳ��Ϊ��ɫ 
		glColor3f(r, g, b);								//2.3.  ����ɫ���ݸ�OpenGL

		float radius = length*scaling_factor / 2;      //2.4.  ���㵱ǰ����ı�Ƿ���С��������룬��������İ뾶

		glPushMatrix();
		glTranslatef(v[0], v[1], 0);
		glutSolidSphere(radius, 30, 30);					//2.5.  ������״��Ƿ�
		glPopMatrix();
	}

	glDisable(GL_LIGHTING);
}




void GlyphRenderer::drawCones(Grid& g)					//��׵����ӻ�'g', �ߴ缰��ת�ĽǶȷ�ӳ����
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	VectorAttributes& vectors = g.pointVectors();		// ��ȡʸ���Ĵ�С��Χ�������һ���� [0,1]
	float vector_min = vectors.getMin();
	float vector_max = vectors.getMax();

	glMatrixMode(GL_MODELVIEW);

	for (int i = 0; i<g.numPoints(); ++i)
	{
		float v[2];
		g.getPoint(i, v);								//2.1. ��ȡ i-th vertex ������	

		Point3d vec = g.pointVectors().getC0Vector(i);

		float length = (vec.norm() - vector_min) / (vector_max - vector_min); // ������룬ͬǰ

		float r, g, b;
		mapToColor(length, r, g, b);						//2.2.  ����ֵӳ��Ϊ��ɫ  
		glColor3f(r, g, b);								//2.3.  ����ɫ���ݸ�OpenGL

		float scale = (scaling_mode == SCALE_NORMALIZE) ? 1 : length; //��������ֵ
		float radius = scale*scaling_factor;			//2.4.  �����Ƿ��ĳߴ�, ���������ĳ��Ⱥ�����ģʽ��ѡ��
		float angle = direction2angle(vec);				//2.5.  �����Ƿ��ĽǶ�

		glPushMatrix();									//2.6.  ƽ�ƺ���ת׵�壬ʹ�䰮��ǰʸ�������ģ�����ʸ���ķ���
		glTranslatef(v[0], v[1], 0);                           // �������						
		glRotatef(90 - angle, 0, 0, -1);                            // �������
		glRotatef(-90, 1, 0, 0);                              // �������
		glutSolidCone(radius / 4, radius, 30, 30);			//2.7.  ����׵��
		glPopMatrix();
	}

	glDisable(GL_LIGHTING);
}



void GlyphRenderer::drawArrows(Grid& g)					//��������Ƿ����ӻ�'g' , �ߴ缰��ת�ĽǶȷ�ӳ����
{
	VectorAttributes& vectors = g.pointVectors();		// ��ȡʸ���Ĵ�С��Χ�������һ���� [0,1]
	float vector_min = vectors.getMin();
	float vector_max = vectors.getMax();


	glBegin(GL_LINES);									//1. ����������Ƿ���ʹ�� OpenGL line-set	
	for (int i = 0; i<g.numPoints(); ++i)
	{
		Point3d v;
		g.getPoint(i, v.data);							//2.1. ��ȡ i-th vertex ������	

		Point3d vec = g.pointVectors().getC0Vector(i);
		float length = (vec.norm() - vector_min) / (vector_max - vector_min);

		float r, g, b;
		mapToColor(length, r, g, b);						//2.2.  ����ֵӳ��Ϊ��ɫ 
		glColor3f(r, g, b);								//2.3.  ����ɫ���ݸ�OpenGL

		vec.normalize();
		float scale = (scaling_mode == SCALE_NORMALIZE) ? 1 : length;
		vec *= scale*scaling_factor;
		Point3d tip = v + vec;							//2.4.  ����line-glyph�Ķ˵�

		glVertex3f(v[0], v[1], 0);						//2.5.  �������Ͷ˵�����괫�ݸ�OpenGL
		glVertex3f(tip[0], tip[1], 0);					    //�������
	}
	glEnd();											//3. �������� line-set
}



void GlyphRenderer::draw(Grid& g)						//��������'g' 
{
	glClearColor(1, 1, 1, 1);								//1. Clear the frame and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (draw_style)
	{
	case DRAW_ARROWS:
		drawArrows(g);
		break;
	case DRAW_MAGNITUDE:
		drawMagnitude(g);
		break;
	case DRAW_BALLS:
		drawBalls(g);
		break;
	case DRAW_CONES:
		drawCones(g);
		break;
	}

	glutSwapBuffers();
}




