#pragma once

#include "Renderer.h"



class ColormapRenderer : public Renderer
{
public:

enum DRAW_STYLE
{
	DRAW_POINTS   =0,					//绘点，用色彩图依据点的标量值为点着色
	DRAW_C0_CELLS,						//绘cells,用色彩图依据顶点的标量值为单元着色。在C0条件下，cell内部各点的标量数值就是各个顶点的标量值的均值
	DRAW_C1_CELLS,						//绘cells,用色彩图依据顶点的标量值为单元着色。在C1条件下，cell内部各点的标量数值采用双线性插值确定
	DRAW_C1_TEX_CELLS					//绘cells,用色彩图依据顶点的标量值为单元着色，使用1D纹理的双线性插值
};	


enum COLORMAP_TYPE
{
    COLORMAP_GRAYSCALE =0,				//灰度图的id
	COLORMAP_RAINBOW,					//彩虹色彩图的id
	COLORMAP_SATURATION					//辐射图的id
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



						

