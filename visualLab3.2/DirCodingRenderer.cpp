#include "./include/DirCodingRenderer.h"
#include "./include/VectorAttributes.h"
#include "./include/Grid.h"
#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>									//GLUT library
#include <algorithm>
#include <math.h>
#include <iostream>

using namespace std;



static float direction2angle(Point3d v)					//��2D vector ת��Ϊһ������ֵ���÷���ֵΪ�Ƕȱ�ʾ������һ����[0,1]
{														
	v.normalize();										

	float cosa = v.x;
	float sina = v.y;
	
	float a;
	if (sina>=0)
		a = acos(cosa);
	else
		a = 2*3.14159 - acos(cosa);
	
	return a/(2*3.14159);
}




static void hsv2rgb(float h, float s, float v, float& r, float& g, float& b)
{
    float m, n ,f;
    int i;
    h = 6*h;
    i = floor(h);
    f = h-i;
    if (!(i&1))f = 1-f;
    m = v*(1-s);
    n = v*(1-s*f);

    switch (i) {
        case 6:
        case 0: r = v; g = n; b = m;break;
        case 1: r = n; g = v; b = m;break;
        case 2: r = m; g = v; b = n;break;
        case 3: r = m; g = n; b = v;break;
        case 4: r = n; g = m; b = v;break;
        case 5: r = v; g = m; b = n;break;
    }

}

//�ڶ�������ʵ�ֱ�����
void DirCodingRenderer::draw(Grid& g)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    VectorAttributes&vectors = g.pointVectors();

    for (int i = 0; i < g.numCells(); ++i) {
        int cell[10];

        int nvertices = g.getCell(i, cell);
        if (nvertices!= 3 && nvertices!=4)
            continue;

        if (nvertices == 3)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_QUADS);

        for (int j = 0; j < nvertices; ++j) {
            float x[2];
            g.getPoint(cell[j], x);

            const Point3d& v = g.pointVectors().getC0Vector(cell[j]);
            float direction = direction2angle(v);

            float R, G, B;
            hsv2rgb(direction, 1, 1, R, G, B);

            glColor3f(R, G, B);
            glVertex3f(x[0], x[1], 0);
        }
        glEnd();

    }
    glutSwapBuffers();

}


