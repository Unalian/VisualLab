#include <iostream>

#include <math.h>
#include "Quad.h"
#include "Point3d.h"

#include <stdio.h>
#include <stdlib.h>

float Xmin	= -5, Xmax	= 5;
float Ymin	= -5, Ymax	= 5;
int   Nx	= 30, Ny	= 30;

float dx = (Xmax-Xmin)/(Nx-1);
float dy = (Ymax-Ymin)/(Ny-1);


float fov	 = 60;
// float aspect = 1;
float z_near = 0.1;
float z_far  = 30;

float eye_x = 10, eye_y = 10, eye_z = 10;
float c_x   = 0,  c_y   = 0,  c_z   = 0;
float up_x  = 0,  up_y  = 0,  up_z  = 1;

float alpha = 0.7;

float f(float x, float y)
{
    float ret = 8*expf(-(x*x+y*y)/5);
    return ret;
}

void viewing(int W, int H)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x,eye_y,eye_z,c_x,c_y,c_z,up_x,up_y,up_z);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, float(W)/H, z_near, z_far);

    glViewport(0, 0, W, H);
}

void draw()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(float x = Xmin; x <= Xmax-dx; x+=dx)
        for (float y = Ymin; y <= Ymax-dy; y+=dy){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glColor3f(0,1,0);
            Quad q1;
            q1.addPoint(x, y, f(x,y));
            q1.addPoint(x+dx, y, f(x+dx, y));
            q1.addPoint(x+dx, y+dy, f(x+dx, y+dy));
            q1.addPoint(x, y+dy, f(x, y+dy));
            q1.draw();

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3f(0,0,0);
            Quad q2;
            q2.addPoint(x, y, f(x,y));
            q2.addPoint(x+dx, y, f(x+dx, y));
            q2.addPoint(x+dx, y+dy, f(x+dx, y+dy));
            q2.addPoint(x, y+dy, f(x, y+dy));
            q2.draw();
        }
    glutSwapBuffers();
}

void drawFlatShading()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_FLAT);
    float light[4] = {1,1,1,0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION,light);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0,1,0);

    for(float x = Xmin; x <= Xmax-dx; x+=dx)
        for (float y = Ymin; y <= Ymax-dy; y+=dy){

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            Point3d p1(x, y, f(x,y));
            Point3d p2(x+dx, y, f(x+dx, y));
            Point3d p3(x+dx, y+dy, f(x+dx, y+dy));
            Point3d p4(x, y+dy, f(x, y+dy));

            Point3d n = (p2 - p1).cross(p4 - p1);
            n.normalize();

            Quad q1;
            q1.addNormal(n.data);
            q1.addPoint(x, y, f(x,y));
            q1.addPoint(x+dx, y, f(x+dx, y));
            q1.addPoint(x+dx, y+dy, f(x+dx, y+dy));
            q1.addPoint(x, y+dy, f(x, y+dy));
            q1.draw();


        }
    glutSwapBuffers();
}

Point3d computeNormal(const Point3d& p){
    Point3d pr(p.x+dx, p.y, f(p.x+dx,p.y));
    Point3d pl(p.x-dx, p.y, f(p.x-dx,p.y));
    Point3d pu(p.x, p.y+dy, f(p.x,p.y+dy));
    Point3d pd(p.x, p.y-dy,f(p.x,p.y-dy));
    Point3d n1 = (pr-p).cross(pu-p);
    n1.normalize();
    Point3d n2 = (pu-p).cross(pl-p);
    n2.normalize();
    Point3d n3 = (pl-p).cross(pd-p);
    n3.normalize();
    Point3d n4 = (pd-p).cross(pr-p);
    n4.normalize();
    Point3d n = n1+n2+n3+n4;
    n.normalize();
    return n;
}
void drawSmooth(){
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);
    float light[4] = {1,1,1,0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION,light);
    glEnable(GL_COLOR_MATERIAL);

    glColor3f(0,1,0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for(float x = Xmin; x <= Xmax-dx; x+=dx)
        for (float y = Ymin; y <= Ymax-dy; y+=dy){

            Point3d p1(x, y, f(x,y));
            Point3d p2(x+dx, y, f(x+dx, y));
            Point3d p3(x+dx, y+dy, f(x+dx, y+dy));
            Point3d p4(x, y+dy, f(x, y+dy));

            Point3d n1 = computeNormal(p1);
            Point3d n2 = computeNormal(p2);
            Point3d n3 = computeNormal(p3);
            Point3d n4 = computeNormal(p4);

            Quad q1;
            q1.addNormal(n1.data);
            q1.addPoint(p1.x, p1.y, p1.z);
            q1.addNormal(n2.data);
            q1.addPoint(p2.x, p2.y, p2.z);
            q1.addNormal(n3.data);
            q1.addPoint(p3.x, p3.y, p3.z);
            q1.addNormal(n4.data);
            q1.addPoint(p4.x, p4.y, p4.z);
            q1.draw();
        }
    glutSwapBuffers();
}


void drawSmooth_definition(){
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    float light[4] = {1,1,1,0};
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION,light);
    glEnable(GL_COLOR_MATERIAL);
    for(float x = Xmin; x <= Xmax-dx; x+=dx)
        for (float y = Ymin; y <= Ymax-dy; y+=dy){
            Point3d p1(x, y, f(x,y));
            Point3d p2(x+dx, y, f(x+dx, y));
            Point3d p3(x+dx, y+dy, f(x+dx, y+dy));
            Point3d p4(x, y+dy, f(x, y+dy));
            Point3d n1 = computeNormal(p1);
            Point3d n2 = computeNormal(p2);
            Point3d n3 = computeNormal(p3);
            Point3d n4 = computeNormal(p4);
            glColor3f(0,1,0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            Quad q1;
            q1.addNormal(n1.data);
            q1.addPoint(p1.x, p1.y, p1.z);
            q1.addNormal(n2.data);
            q1.addPoint(p2.x, p2.y, p2.z);
            q1.addNormal(n3.data);
            q1.addPoint(p3.x, p3.y, p3.z);
            q1.addNormal(n4.data);
            q1.addPoint(p4.x, p4.y, p4.z);
            q1.draw();
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glColor3f(0,0,0);
            Quad q2;
            q2.addPoint(p1.x,p1.y,-1);
            q2.addPoint(p2.x, p2.y, -1);
            q2.addPoint(p3.x,p3.y,-1);
            q2.addPoint(p4.x, p4.y, -1);
            q2.draw();
        }
    glutSwapBuffers();
}

void drawSmooth_alpha(){
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    float light[4] = {1,1,1,0};
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION,light);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for(float x = Xmin; x <= Xmax-dx; x+=dx)
        for (float y = Ymin; y <= Ymax-dy; y+=dy){
            Point3d p1(x, y, f(x,y));
            Point3d p2(x+dx, y, f(x+dx, y));
            Point3d p3(x+dx, y+dy, f(x+dx, y+dy));
            Point3d p4(x, y+dy, f(x, y+dy));
            Point3d n1 = computeNormal(p1);
            Point3d n2 = computeNormal(p2);
            Point3d n3 = computeNormal(p3);
            Point3d n4 = computeNormal(p4);
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glColor3f(0,0,0);
            Quad q2;
            q2.addPoint(p1.x,p1.y,-1);
            q2.addPoint(p2.x, p2.y, -1);
            q2.addPoint(p3.x,p3.y,-1);
            q2.addPoint(p4.x, p4.y, -1);
            q2.draw();
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glColor4f(0,1,0,alpha);
            Quad q1;
            q1.addNormal(n1.data);
            q1.addPoint(p1.x, p1.y, p1.z);
            q1.addNormal(n2.data);
            q1.addPoint(p2.x, p2.y, p2.z);
            q1.addNormal(n3.data);
            q1.addPoint(p3.x, p3.y, p3.z);
            q1.addNormal(n4.data);
            q1.addPoint(p4.x, p4.y, p4.z);
            q1.draw();
        }
    glutSwapBuffers();
}

void drawSmooth_loadTexture(){
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    float light[4] = {1,1,1,0};
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION,light);
    glEnable(GL_COLOR_MATERIAL);

    glColor3f(0,1,0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_TEXTURE_2D);
    for(float x = Xmin; x <= Xmax-dx; x+=dx)
        for (float y = Ymin; y <= Ymax-dy; y+=dy){

            Point3d p1(x, y, f(x,y));
            Point3d p2(x+dx, y, f(x+dx, y));
            Point3d p3(x+dx, y+dy, f(x+dx, y+dy));
            Point3d p4(x, y+dy, f(x, y+dy));

            Point3d n1 = computeNormal(p1);
            Point3d n2 = computeNormal(p2);
            Point3d n3 = computeNormal(p3);
            Point3d n4 = computeNormal(p4);

            Quad q1;
            q1.addNormal(n1.data);
            glTexCoord2f(0,0);
            q1.addPoint(p1.x, p1.y, p1.z);
            q1.addNormal(n2.data);
            glTexCoord2f(1,0);
            q1.addPoint(p2.x, p2.y, p2.z);
            q1.addNormal(n3.data);
            glTexCoord2f(1,1);
            q1.addPoint(p3.x, p3.y, p3.z);
            q1.addNormal(n4.data);
            glTexCoord2f(0,1);
            q1.addPoint(p4.x, p4.y, p4.z);
            q1.draw();
        }
    glutSwapBuffers();
}


void loadTexture(const char *filename){
    FILE *inFile;
    char buffer[100000];
    GLubyte *theTexture;
    int width, height, maxVal, pixelSize;
    if(!(inFile = fopen(filename, "rb"))){
        fprintf(stderr, "Can't open %s\n",filename);
        exit(-1);}
    printf("Load %s successfully123\n", filename);
    fgets(buffer,sizeof(buffer), inFile);
    if (buffer[0] != 'P'|| (buffer[1]!='6')) {
        fprintf(stderr,"Not a binary PPM file %s\n", filename);
        exit(-1);
    }
    if (buffer[2] == 'A') {
        pixelSize = 4;
        printf("A\n");
    }else{
        pixelSize = 3;
        printf("not A\n");
    }
    do fgets(buffer, sizeof(buffer), inFile);
    while (buffer[0] == '#');
    sscanf(buffer, "%d %d", &width, &height);
    int memSize = width * height * 4 * sizeof(GLubyte);
    printf("%d\n", memSize);
    theTexture = (GLubyte *)malloc(memSize);

    for(int i = 0; i<memSize; i++){
        if((i%4<3) || pixelSize == 4)
            theTexture[i] = (GLubyte)fgetc(inFile);
        else
            theTexture[i] = (GLubyte)255;
    }
    fclose(inFile);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,theTexture);
    free(theTexture);
}

int main(int argc,char* argv[])							//主程序
{
    glutInit(&argc, argv);								//1. 初始化GLUT toolkit
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);			//2. 通知GLUT创建的窗口是包括RGB framebuffer以及Z-buffer
    glutInitWindowSize(500,500);							//3. 通知GLUT创建的窗口的尺寸
    glutCreateWindow("exp1_smooth_with_texture");					//4. 创建窗口，标题指定
    loadTexture("../texture.ppm");

    glutDisplayFunc(drawSmooth_loadTexture);								//5. 为窗口指定drawing callback
    glutReshapeFunc(viewing);							//6. 为窗口制定resize callback
    glutMainLoop();										//7. 开始事件循环。此后可以接受显示，改变窗口大小等事件

    return 0;
}




