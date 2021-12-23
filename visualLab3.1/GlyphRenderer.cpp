#include "./include/GlyphRenderer.h"
#include "./include/ScalarAttributes.h"
#include "./include/VectorAttributes.h"
#include "./include/Grid.h"
#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>								//GLUT library
#include <algorithm>
#include <math.h>
#include <iostream>


using namespace std;


static float direction2angle(const Point3d& d)	//Ω´2D vector µƒ∑ΩœÚ£®Ω«∂»£©«Û≥ˆ¿¥£¨Ω«∂»‘⁄[0,360]÷Æº‰
{
    Point3d x = d; x.normalize();

    float cosa = x.x;
    float sina = x.y;

    float a;
    if (sina>=0)
        a = acos(cosa);
    else
        a = 2*3.14159 - acos(cosa);

    return 180*a/ 3.14159;
}




void GlyphRenderer::drawMagnitude(Grid& g)
{
    VectorAttributes& vectors = g.pointVectors();
    float vector_min = vectors.getMin();
    float vector_max = vectors.getMax();

    glPointSize(3);
    glBegin(GL_POINTS);
    for(int i=0;i<g.numPoints();++i)
    {
        float v[2];
        g.getPoint(i,v);

        Point3d vec = g.pointVectors().getC0Vector(i);
        float length = (vec.norm() - vector_min) / (vector_max - vector_min);

        float r,g,b;
        mapToColor(length, r, g, b);
        glColor3f(r,g,b);
        glVertex3f(v[0],v[1],0);
    }
    glEnd();
}


void GlyphRenderer::drawBalls(Grid& g)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    VectorAttributes& vectors = g.pointVectors();
    float vector_min = vectors.getMin();
    float vector_max = vectors.getMax();
    glMatrixMode(GL_MODELVIEW);
    for (int i = 0; i<g.numPoints(); ++i)
    {
        float v[2];
        g.getPoint(i, v);
        Point3d vec = g.pointVectors().getC0Vector(i);
        float length = (vec.norm()-vector_min)/(vector_max-vector_min);
        float r, g, b;
        mapToColor(length, r, g, b);
        glColor3f(r, g, b);
        float scale = (scaling_mode == SCALE_NORMALIZE) ? 0.5 : length;
        float radius = scale*scaling_factor / 2;
        glPushMatrix();
        glTranslatef(v[0], v[1], 0);
        glutSolidSphere(radius, 30, 30);
        glPopMatrix();
    }
    glDisable(GL_LIGHTING);
}




void GlyphRenderer::drawCones(Grid& g)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    VectorAttributes& vectors = g.pointVectors();
    float vector_min = vectors.getMin();
    float vector_max = vectors.getMax();
    glMatrixMode(GL_MODELVIEW);
    for (int i = 0; i<g.numPoints(); ++i)
    {
        float v[2];
        g.getPoint(i, v);
        Point3d vec = g.pointVectors().getC0Vector(i);
        float length = (vec.norm() - vector_min) / (vector_max - vector_min);
        float r, g, b;
        mapToColor(length, r, g, b);
        glColor3f(r, g, b);
        float scale = (scaling_mode == SCALE_NORMALIZE) ? 1 : length;
        float radius = scale*scaling_factor;
        float angle = direction2angle(vec);
        glPushMatrix();
        glTranslatef(v[0], v[1], 0);
        glRotatef(90 - angle, 0, 0, -1);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(radius / 4, radius, 30, 30);
        glPopMatrix();
    }
    glDisable(GL_LIGHTING);
}



void GlyphRenderer::drawArrows(Grid& g)
{
    VectorAttributes& vectors = g.pointVectors();
    float vector_min = vectors.getMin();
    float vector_max = vectors.getMax();
    glBegin(GL_LINES);
    for(int i=0;i<g.numPoints();++i)
    {
        Point3d v;
        g.getPoint(i,v.data);
        Point3d vec = g.pointVectors().getC0Vector(i);
        float length = (vec.norm()-vector_min)/(vector_max-vector_min);
        float r,g,b;
        mapToColor(length,r,g,b);
        glColor3f(r,g,b);
        vec.normalize();
        float scale = (scaling_mode==SCALE_NORMALIZE)? 1 : length;
        vec *= scale*scaling_factor;
        Point3d tip = v + vec;

        glVertex3f(v[0], v[1], 0);
        glVertex3f(tip[0], tip[1], 0);
    }
    glEnd();
}



void GlyphRenderer::draw(Grid& g)						//ªÊ÷∆Õ¯∏Ò'g'
{
    glClearColor(1,1,1,1);								//1. Clear the frame and depth buffers
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




