#pragma once

#include "Grid.h"
#include "ScalarAttributes.h"


class UniformGrid : public Grid //均匀网格的实现类
{
public:			
					UniformGrid(int N1, int N2, float m1, float m2, float M1, float M2)
					: scalars(N1*N2), 
					  N1(N1),N2(N2),
					  m1(m1),m2(m2),
					  d1((M1-m1)/(N1-1)),d2((M2-m2)/(N2-1))
					  { }
					//从上到下，分别为：
					//本grid中标量个数即采样点个数
					//本grid中标量个数即采样点个数
					//指定grid的两个坐标方向上采样点数目
					//指定grid的两个坐标方向上最小坐标
					//指定两个坐标方向上cell的尺寸

		           ~UniformGrid();	

int					numPoints()
					{ return N1*N2; } //采样点个数

int					numCells() 
					{ return (N1-1)*(N2-1); } //cell个数

void				getPoint(int i,float* p) 
					{ p[0] = m1 + (i%N1)*d1; p[1] = m2 + (i/N1)*d2; }

int					getCell(int i,int* c); 

int					getDimension1()
					{ return N1; } 

int					getDimension2()
					{ return N2; } 
			
int					findCell(float*);

ScalarAttributes&	pointScalars()
					{ return scalars; }
			
protected:

ScalarAttributes scalars;
int				 N1,N2;			//沿着x轴和y轴的采样点的个数 
float			 m1,m2;			//x方向和y方向的坐标值的最小者

private: 

float		d1,d2;				//本grid中Cell的尺寸，注意是均匀网格，因此cell的大小是一样的

};




