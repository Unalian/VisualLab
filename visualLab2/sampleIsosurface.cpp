#include <OpenGL/gl.h>
#include <OpenGL/opengl.h>
#include <GLUT/glut.h>
#include "include/Volume.h"
#include "include/Isosurface.h"
#include "include/UnstructuredGrid3D.h"							//3D �޽ṹ������
#include "include/MeshRenderer.h"								//3D�޽ṹ���������Ⱦ����ʵ����ƽ����Ⱦ��ƽ����Ⱦ
#include "include/zpr.h"
#include <iostream>



Isosurface<float>	isoEngine;							//�ö�����ӱ��������������ȡ��ֵ�棬�� Volume<T> �洢
MeshRenderer		renderer;							//�ö���������ȡ�ĵ�ֵ��
Volume<float>*		volume			= 0;				//3D ������ݣ��洢�������ݻ���������
UnstructuredGrid3D*	grid			= 0;				//3D ���񣬴洢��ȡ���ĵ�ֵ��

float				isoValue		= 0;				//����ֵ�����ݴ�ֵ�������Ӧ�ĵ�ֵ��
float				valMin,valMax;						//����������еı��������ֵ����Сֵ
float				isoDelta;							//��/����ֵ���Ӧ�ı������ݵĲ���
float				max_depth		= 0;
int					drawing_style	= MeshRenderer::DRAW_C1_CELLS;  //����C1����cell�������˫���Բ�ֵ
float				volSize			= 0;

void computeIsosurface(UnstructuredGrid3D*& grid)		//���ݸ�����isoValue�����ֵ��. 
{														//�洢��'grid'.
	delete grid;										//1. ����ǰ��ɾ����ǰ��grid���κ���Կ��Ǵ洢ÿһ�εĽ���Լ�С�ٴ���ʾʱ�Ŀ���������ÿ���ػ�
	grid = isoEngine.Contour(*volume,isoValue);			//2. �����ֵ�沢�洢�� 'grid'

	grid->computeFaceNormals();							//3. �����ֵ����漰���㷨��
	grid->computeVertexNormals();						

	cout<<"����ֵ���Ӧ�ı���ֵΪ: "<<isoValue<<" �� ["<<valMin<<","<<valMax<<"] ��Χ��"<<endl;
}

static void mouseclick(int button, int state, int x, int y)
{
	int keys = glutGetModifiers();						//����¼���ȡ��id:	
	if (keys & GLUT_ACTIVE_CTRL)						// 
		button = GLUT_MIDDLE_BUTTON;					// �����ס�϶�������ת�ӵ�
	if (keys & GLUT_ACTIVE_SHIFT)						// �м�/���֣������+Control����ס�϶���������
		button = GLUT_RIGHT_BUTTON;						// �Ҽ��������+Shift����ס�϶�����ƽ���ӵ㣬���Ͼ��Լ�ʵ��

	zprMouse(button,state,x,y);							//ʹ��ZPR�⴦���ӵ�. �ÿ�ͨ������¼����� OpenGL ��modelview �������ı�۲�� 
														
}

static void mousemotion(int x, int y)
{
	zprMotion(x,y);
	glutPostRedisplay();
}

static void viewing(int width, int height)					//GLUT ���ڸı�ʱ�Ļص�
{
	glutReshapeWindow(width,height);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,double(width)/height,10.0f,max_depth);
	glViewport(0,0,width,height);
}

static void keyboard(unsigned char ch,int,int)				//GLUT �����¼��ص�
{
	switch(ch)
	{
	case ' ':												//�ı����ر���Ļ���ģʽ
		{
		  drawing_style = (++drawing_style) % 4;
		  renderer.setDrawingStyle((MeshRenderer::DRAW_STYLE)drawing_style);


		  break;
		}
	case 'r':												//�����ӵ�
	case 'R':
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		zprInit(0,0,-(1.7f-0.5f)*volSize);
		break;	
	case '+':												//���ӵ�ֵ�����ֵ	
	case '=':												//���¼���õ�ֵ��
		isoValue += isoDelta;
		if (isoValue>valMax) isoValue=valMax;
		computeIsosurface(grid);
		break;
	case '-':
	case '_':
		isoValue -= isoDelta;
		if (isoValue<valMin) isoValue=valMin;
		computeIsosurface(grid);
		break;
	}	
	
	glutPostRedisplay();	
}		



static void display()		//GLUT ��ʾ�ص�����
{
   const int width = volume->getWidth(), height = volume->getHeight(), depth = volume->getDepth();

   glMatrixMode(GL_MODELVIEW);	
   glPushMatrix();
   glTranslatef(-width/2,-height/2,-1.7*depth);	//ʹ�ô���ʾ���ݼ���ǰ����ƽ������ļ�ǰ����ȷ����ʾ
   renderer.draw(*grid);
   glPopMatrix();
}



int main(int argc,char* argv[])								
{
	
	//û����������ݼ��Ļ���ȱʡ��ͷ��ɨ�����ݼ�
	const char* filename = (argc>1)? argv[1]:"../DATA/head.dat.vtk";
	
	glutInit(&argc, argv);	//1. ��ʼ�� GLUT toolkit
	//2. ֪ͨGLUT�������ڣ��ô��ڰ���RGB framebuffer�Լ�Z-buffer
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(500,500);//3. ֪ͨGLUT�����Ĵ��ڵĳߴ�
	glutCreateWindow("lab2");	// ������������

	volume = new Volume<float>;

	const int pad = 2;	//4. ����������ݵ�ʱ����ÿ���������฽��0�������������
	bool ok;												
   
   	if(strstr(filename, ".fld"))//5. ��������ļ��ж������ݣ�Ŀǰֻ֧��fld��vtk��ʽ���ļ�����
	  ok = volume->readAVSField(filename,pad);
	else if (strstr(filename, ".vtk"))
	  ok = volume->readVTKField(filename,pad);
	else 
	{
	  cout<<"Unknown file format"<<endl;
	  return -1;
	}
	if (!ok)
	{
	  cout<<"Cannot open file: "<<filename<<endl;
	  return -1;
	}

	volume->getMinMax(valMin,valMax);//6. �������ֵ�ķ�Χ	
	isoValue = (valMax+valMin)/2;//��ֵ���ڳ�ʼ����ֵ�棬�Լ�ȷ��step-size	
	isoDelta = (valMax-valMin)/40;//ÿ�ε����仯������
																
	computeIsosurface(grid);//7. �����ֵ�棬����洢��'grid'
	
	//8. ��ʼ���۲����Ⱦ�Ĳ���	
	volSize = (volume->getWidth()+volume->getHeight()+volume->getDepth())/3.0; 
	zprInit(0,0,-(1.7f-0.5f)*volSize);
	max_depth = 1.5*sqrt((double)(volume->getWidth()*volume->getWidth() + volume->getHeight()*volume->getHeight() + volume->getDepth()*volume->getDepth()));
	renderer.setDrawingStyle((MeshRenderer::DRAW_STYLE)drawing_style);
	   
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseclick);
	glutMotionFunc(mousemotion);
	glutReshapeFunc(viewing);
	glutDisplayFunc(display);
	
	glutMainLoop();

	return 0;
}




