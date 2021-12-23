#pragma once

#include "Volume.h"
#include <map>
#include <vector>

class UnstructuredGrid3D;




template <class T> class Isosurface			//��3D�������ݼ������ֵ�棬�洢ΪVolume.
{											//��ֵ����UnstructuredGrid3D (������mesh)����.
public:
				    Isosurface();			
				   ~Isosurface();			
UnstructuredGrid3D* Contour(const Volume<T>& volume,T level,float cellLengthX=1,float cellLengthY=1,float cellLengthZ=1);
											//�������ı���ֵ������ֵ��
protected:

struct IsoVertex							//�����ṹ��: �洢��ֵ��Ķ���
{		
	unsigned int	newID;
	float			x,y,z;
};

struct IsoTriangle
{											
	unsigned int	pointID[3];
};

typedef std::vector<IsoTriangle>		 IsoTriangles;					//���й��ɵ�ֵ�������������	
typedef std::map<unsigned int,IsoVertex> IsoVertices;					//���й��ɵ�ֵ������Ķ��㣬�ö�������hash��Ϊ�������㣩

IsoVertices			isoVertices;										//���ɵ�ֵ��Ķ���ļ��� 
IsoTriangles		isoTriangles;										//���ɵ�ֵ��������εļ���
const Volume<T>*	volume;												//3D��������
T					isoLevel;											//��ֵ���Ӧ֮����ֵ
unsigned int		m_nCellsX, m_nCellsY, m_nCellsZ;					// x��y��z �����cell�ı��
float				m_fCellLengthX, m_fCellLengthY, m_fCellLengthZ;		// x��y��z �����cell�ĳߴ�

unsigned int		GetEdgeID(unsigned int nX,unsigned int nY,unsigned int nZ,unsigned int nEdgeNo);

unsigned int		GetVertexID(unsigned int nX,unsigned int nY,unsigned int nZ);

IsoVertex			CalculateIntersection(unsigned int nX,unsigned int nY,unsigned int nZ,unsigned int nEdgeNo);//�Զ���(nX,nY,nZ)��Ϊ���ıߣ����ֵ��Ľ���
					
IsoVertex			Interpolate(float fX1,float fY1,float fZ1,float fX2,float fY2,float fZ2);//�����������֮��Ĳ�ֵ, ���ص��Ǳ�(1,2)���ֵ��Ľ���
					
void				AddVertex(unsigned int x,unsigned int y,unsigned int z,int edgeNo);//���㲢�ڵ�ֵ���������µĶ���
					
UnstructuredGrid3D* CreateIsoMesh();//�洢����õ��ĵ�ֵ��ΪUnstructuredGrid3D	

void				Clear();
					
static const unsigned int edgeTable[256];
static const unsigned int triTable[256][16];
static const int		  edgeIDTable[12][4];
};


template <class T> inline unsigned int Isosurface<T>::GetVertexID(unsigned int nX,unsigned int nY,unsigned int nZ)
{
	return 3*(nZ*(m_nCellsY + 1)*(m_nCellsX + 1) + nY*(m_nCellsX + 1) + nX);
}


template <class T> inline unsigned int Isosurface<T>::GetEdgeID(unsigned int nX,unsigned int nY,unsigned int nZ,unsigned int nEdgeNo)
{
	if (nEdgeNo>11) return -1;
	const int* eit = edgeIDTable[nEdgeNo];	
	return GetVertexID(nX+eit[0],nY+eit[1],nZ+eit[2])+eit[3];
}

