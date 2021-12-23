#pragma once

#include <vector>
#include "Image.h"
#include "coord3.h"

using namespace std;

template<class T> class Volume				//A simple 3D volume class, implemented as a stack of images, one per XY-slice
{
public:
  
					Volume() {}
					Volume(int width, int height, int depth);
					Volume(const Volume<T> &vol) 
					{  slices = vol.slices;  }
  void				makeVolume(int width, int height, int depth);
  Image<T>&			operator[](short z) 
					{ return slices[z]; }
  const Image<T>&	operator[](short z) const 
					{ return slices[z]; }
  T&				operator()(short x, short y, short z) 
					{ return slices[z].pixels[y][x]; }
  const T&			operator()(short x, short y, short z) const 
					{ return slices[z].pixels[y][x]; }
  T&				operator()(const coord3& c) 
					{ return slices[c.z].pixels[c.y][c.x]; }
  const T&			operator()(const coord3& c) const 
					{ return slices[c.z].pixels[c.y][c.x]; }
  void				toVector(vector<T> &vec);
  int				getDepth() const 
					{ return slices.size(); }
  int				getWidth() const 
					{ return slices[0].getWidth(); }
  int				getHeight()const  
					{ return slices[0].getHeight(); }
  void				clearVolume(T val);
  void				clearVolume();
  void				getMinMax(T& min, T& max);
  bool				readBinaryField(const char *fname, int pad, int &np, int *&points, float *&normals);
  bool				readVTKField(const char *fname, int pad=0);
  bool				readAVSField(const char *fname, int pad=0);
  void				exportAVSField(const char *fname);
  void				exportVolField(const char *fname,T threshold,byte fg_value,int pad=0);  
  void				exportVTKField(const char *fname);
  
private:

  vector<Image<T > > slices;
};




