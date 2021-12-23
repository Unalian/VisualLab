#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>									//GLUT
#include "./include/UniformGrid.h"
#include "./include/GlyphRenderer.h"
#include <iostream>
#include <math.h>



float Xmin	= -5, Xmax	= 5;
float Ymin	= -5, Ymax	= 5;
int   Nx	= 20, Ny	= 20;



float f(float x, float y)								//ø… ”ªØ“ª∏ˆºÚµ•∫Ø ˝
{
    float ret = 8*expf(-(x*x+y*y)/5);
    return ret;
}


UniformGrid			grid(Nx,Ny,Xmin,Ymin,Xmax,Ymax);
GlyphRenderer		renderer;
int					drawing_style = 0;
int					colormap      = 0;
int					scaling_mode  = 0;
float				alpha         = 0;


void viewing(int W, int H)								//ªÿµ˜∫Ø ˝£¨Õ¨ µ—È1
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D(Xmin,Xmax,Ymin,Ymax);
    glDisable(GL_DEPTH_TEST);

    glViewport(0,0,W,H);
}



void draw()												//‰÷»æ grid
{
    renderer.draw(grid);
}



void createGrid(float alpha)
{
    for(int i=0;i<grid.numPoints();++i)
    {
        float p[2];
        grid.getPoint(i,p);
        float scalar = f(p[0],p[1]);
        float delta    = 0.001*(Xmax-Xmin);
        float scalar_x = f(p[0]+delta,p[1]);
        float scalar_y = f(p[0],p[1]+delta);
        Point3d gradient = Point3d((scalar_x-scalar)/delta,(scalar_y-scalar)/delta,0);
        Point3d vec;
        vec.x = cos(alpha)*gradient.x - sin(alpha)*gradient.y;
        vec.y = sin(alpha)*gradient.x + cos(alpha)*gradient.y;
        grid.pointVectors().setC0Vector(i,vec);
    }
}



void keyboard(unsigned char c,int,int)					//º¸≈Ãªÿµ˜∫Ø ˝
{
    switch(c)
    {
        case ' ':
        {
            drawing_style = (++drawing_style) % 4;		//Õ®÷™renderer≤…”√∫Œ÷÷∑Ω ΩΩ¯––ªÊ÷∆
            renderer.setDrawingStyle((GlyphRenderer::DRAW_STYLE)drawing_style);
            break;
        }
        case 'c':
        {
            colormap = (++colormap) % 3;					//Õ®÷™renderer π”√∫Œ÷÷…´≤ Õº£®colormap£©
            renderer.setColormapType((GlyphRenderer::COLORMAP_TYPE)colormap);
            break;
        }
        case '+':											//‘ˆº”±Íº«∑˚µƒ≥ﬂ¥Á
        case '=':
        {
            renderer.setScalingFactor(1.05*renderer.getScalingFactor());
            break;
        }
        case '-':											//ºı…Ÿ±Íº«∑˚µƒ≥ﬂ¥Á
        case '_':
        {
            renderer.setScalingFactor(renderer.getScalingFactor()/1.05);
            break;
        }
        case 's':											// «∑Ò∞¥ ∏¡ø¥Û–°Ω¯––Àı∑≈
        {
            scaling_mode = (++scaling_mode) % 2;
            renderer.setScalingMode((GlyphRenderer::SCALING_MODE)scaling_mode);
            break;
        }
        case 'v':											//∏ƒ±‰Ã›∂»–˝◊™µƒΩ«∂»£¨¥”∂¯∏ƒ±‰ ∏¡ø
        {
            alpha = (alpha + 3.14159/15);
            createGrid(alpha);
        }
    }

    glutPostRedisplay();
}



int main(int argc,char* argv[])								//÷˜≥Ã–Ú
{


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB| GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("lab3");


    createGrid(alpha);										//Œ™∏˜ ∏¡ø≥°÷–µƒÕ¯∏Ò∂•µ„¥¥Ω® ∏¡ø÷µ£¨≥ı º÷µalphaŒ™0£¨’‚¿Ôµƒalpha”√”⁄øÿ÷∆ ∏¡øµƒ–˝◊™Ω«∂»£®œ¬¥Œ µ—ÈÕÍ≥…£©

    glutKeyboardFunc(keyboard);                              // ‘ˆº”÷˜“™µƒªÿµ˜∫Ø ˝
    glutDisplayFunc(draw);
    glutReshapeFunc(viewing);
    glutMainLoop();											// ∆Ù∂Ø ¬º˛—≠ª∑£¨¥¶¿Ìº¸≈Ã£¨¥∞ø⁄∏ƒ±‰µ» ¬º˛

    return 0;
}




