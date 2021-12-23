#pragma once

#include "Renderer.h"



class ColormapRenderer : public Renderer
{
public:

enum DRAW_STYLE
{
	DRAW_POINTS   =0,					//��㣬��ɫ��ͼ���ݵ�ı���ֵΪ����ɫ
	DRAW_C0_CELLS,						//��cells,��ɫ��ͼ���ݶ���ı���ֵΪ��Ԫ��ɫ����C0�����£�cell�ڲ�����ı�����ֵ���Ǹ�������ı���ֵ�ľ�ֵ
	DRAW_C1_CELLS,						//��cells,��ɫ��ͼ���ݶ���ı���ֵΪ��Ԫ��ɫ����C1�����£�cell�ڲ�����ı�����ֵ����˫���Բ�ֵȷ��
	DRAW_C1_TEX_CELLS					//��cells,��ɫ��ͼ���ݶ���ı���ֵΪ��Ԫ��ɫ��ʹ��1D�����˫���Բ�ֵ
};	


enum COLORMAP_TYPE
{
    COLORMAP_GRAYSCALE =0,				//�Ҷ�ͼ��id
	COLORMAP_RAINBOW,					//�ʺ�ɫ��ͼ��id
	COLORMAP_SATURATION					//����ͼ��id
};	

				ColormapRenderer(): draw_style(DRAW_POINTS), colormap_type(COLORMAP_GRAYSCALE) 
				{  createTextures(); }

void			setDrawingStyle(DRAW_STYLE s)
				{ draw_style = s; }

void			setColormapType(COLORMAP_TYPE c)
				{ colormap_type = c; }
				
				
void			draw(Grid&);

protected:

DRAW_STYLE		draw_style;
COLORMAP_TYPE	colormap_type;
void			mapToColor(float v,float& R,float& G,float& B);
void			createTextures();
unsigned int	textureID[3];

private:

void			drawPoints(Grid& g);
void			drawC0Cells(Grid& g);
void			drawC1Cells(Grid& g);
void			drawC1TexCells(Grid& g);
};



						

