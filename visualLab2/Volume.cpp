#include "include/Volume.h"
#include <string.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include <stdint.h>


using namespace std;



static inline int is_big_endian()	//ȷ�����õ��Ǵ����ǰ����С����ǰ��ϵͳ
{
    union 
	{
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}


float FloatToBigEndian(float f)							//����ֵת��Ϊ��˸�ʽ
{														//(��С��ϵͳ������ʱ��Ҫ��ת��)
   if (is_big_endian()) return f;

   union
   {
      float f;
      byte b[4];
   } dat1, dat2;

   dat1.f = f;
   dat2.b[0] = dat1.b[3];
   dat2.b[1] = dat1.b[2];
   dat2.b[2] = dat1.b[1];
   dat2.b[3] = dat1.b[0];
   return dat2.f;
}


inline float ReverseFloat(const float inFloat)
{
   float retVal;
   char *floatToConvert = (char*) &inFloat;
   char *returnFloat    = (char*) &retVal;

   // swap the bytes into a temporary buffer
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];

   return retVal;
}





template< class T >
Volume<T>::Volume(int width, int height, int depth) : slices(depth) 
{
	for(register int i=0;i<depth;i++) 
	  slices[i] = Image<T>(width, height);

}



template< class T >
void Volume<T>::makeVolume(int width, int height, int depth) 
{
	if(depth!=(int)slices.size()) 
	{
	  slices.erase(slices.begin(), slices.end());
	  for(register int i=0;i<depth;i++) 
		 slices.push_back(Image<T>(width, height));
	}    
}



template< class T >
void Volume<T>::getMinMax(T &min, T &max)
{
  int width=getWidth(), height=getHeight(), depth=getDepth();
  register int i, j;
  register T gval;

  min=slices[0][0][0]; max=min;
  
  for(i=0;i<depth;i++)
    for(j=0;j<width*height;j++) {
      gval=slices[i][0][j];
      if(gval<min) min=gval;
      if(gval>max) max=gval;
    }
}


template< class T >
void Volume<T>::toVector(vector<T> &vec)
{
  int width=getWidth(), height=getHeight(), depth=getDepth();
  register int i, j, k, len=0;

  vec.resize(width*height*depth);
  
  for(i=0;i<depth;i++)
    for(j=0;j<height;j++)
      for(k=0;k<width;k++)
	vec[len++]=slices[i][j][k];
}


template< class T >
bool Volume<T>::readAVSField(const char *fname, int pad)
{
  FILE *pfile=fopen(fname, "r");
  if (!pfile)
  {
	return false;
  }	
  
  char buffer[255],valtype[128];
  int dx=0, dy=0, dz=0, dummy, width;

  while(!feof(pfile)) 
  {
    fgets(buffer, 128, pfile);
    if(buffer[0]=='#') continue;								//Skip comments
    sscanf(buffer,"ndim=%d\n", &dx);	assert(dx==3);
    fscanf(pfile, "dim1=%d\n", &dx);	assert(dx>0);
    fscanf(pfile, "dim2=%d\n", &dy);	assert(dy>0);
    fscanf(pfile, "dim3=%d\n", &dz);	assert(dz>0);
    width = dx;

    makeVolume(width+2*pad, dy+2*pad, dz+2*pad);				//Allocate volume, pad it for safety with a small border
    fscanf(pfile, "nspace=%d\n", &dummy);
    fscanf(pfile, "veclen=%d\n", &dummy);
    fscanf(pfile, "data=%s\n", valtype);
    
	fgets(buffer, 128, pfile);									//Skip the field, min_ext, and max_ext specifications
	fgets(buffer, 128, pfile);
	fgets(buffer, 128, pfile);     

	fread(buffer,sizeof(byte),2,pfile);							//Skip the two form-feed characters following the header


	if(!strcmp(valtype,"byte"))									//Read a byte volume, convert it to a T volume:
	{
		byte* buffer = new byte[dx*dy*sizeof(byte)];
		for(int i=0;i<dz;++i) 
		{
		  int read = fread(buffer,sizeof(byte),dx*dy,pfile);		
		  assert(read==dx*dy);
		  const byte* buf = buffer;
		  Image<T>& slice = slices[i+pad];
		  for(int k=0;k<dy;++k)
		  {
			T* row = slice[k+pad]+pad;
			for(T* rend = row+width;row!=rend;) *row++ = (T)(*buf++);
		  }
		}
		delete[] buffer;
    }
	else 
	if(!strcmp(valtype,"float"))								//Read a float volume, convert it to a T volume:
	{
		bool platform_big_endian = is_big_endian();				//Determine platform's endianness

		float* buffer = new float[dx*dy*sizeof(float)];
		for(int i=0;i<dz;++i) 
		{
		  int read = fread(buffer,sizeof(float),dx*dy,pfile);
		  assert(read==dx*dy);
		  const float* buf = buffer;
		  Image<T>& slice = slices[i+pad];
		  for(int k=0;k<dy;++k)
		  {
			T* row = slice[k+pad]+pad;
			for(T* rend = row+width;row!=rend;) 
			{
			  float val = *buf++;
			  if (!platform_big_endian) val = ReverseFloat(val);
			  *row++ = (T)(val);
			}  
		  }
		}
		delete[] buffer;
    }
	else 
	{
	    cout<<"Data type is "<<valtype<<", can only read bytes and floats."<<endl;
		return false;
	}
	
    return true;
  }

  return true;
}

template< class T >
bool Volume<T>::readVTKField(const char *fname, int pad)
{
  FILE *pfile=fopen(fname, "r");
  if (!pfile) return false;
  
  char buffer[128];
  int dx=0, dy=0, dz=0, dummy1, dummy2, dummy3;

  while(!feof(pfile)) 
  {
    fgets(buffer, 128, pfile);
    if(buffer[0]=='#') continue;
    break;
  }
  
  bool ascii = true;
  if(strncmp(buffer, "ASCII", 5)) ascii = false;
  
  fgets(buffer, 128, pfile);
  if(strncmp(buffer, "DATASET STRUCTURED_POINTS", 25)) 
  {
    printf("readVTKField: can load only VTK structured points.\n");
    return false;
  }        
  fgets(buffer, 128, pfile);
  sscanf(buffer, "DIMENSIONS %d %d %d\n", &dx, &dy, &dz);
  assert(dx>0 && dy>0 && dz>0);

  fgets(buffer, 128, pfile);
  sscanf(buffer, "ORIGIN %d %d %d\n", &dummy1, &dummy2, &dummy3);
    
  fgets(buffer, 128, pfile);
  sscanf(buffer, "SPACING %d %d %d\n", &dummy1, &dummy2, &dummy3);
    
  fgets(buffer, 128, pfile);
  char points_or_cells[128];
  sscanf(buffer, "%s %d\n", points_or_cells, &dummy1);
  if (!strcmp(points_or_cells,"CELL_DATA"))
  {  --dx; --dy; --dz;  }
  
  fgets(buffer, 128, pfile);
  char voxel_type[32];
  sscanf(buffer, "%*s %*s %s", voxel_type);

  fgets(buffer, 128, pfile);

  makeVolume(dx+2*pad, dy+2*pad, dz+2*pad);
  
  if (ascii)
  {
	  for(int i=0;i<dz;i++) 
	  {
		Image<T> &slice = this->slices[i+pad];

		for(int k=0;k<dy;k++)
		  for(int j=0;j<dx;j++) 
		  {
			float value;
			fscanf(pfile,"%f",&value);
			slice[k+pad][j+pad]=(T)value;
		  }
	  }  
  }
  else //binary
  {
	  if (strcmp(voxel_type,"unsigned_char") && strcmp(voxel_type,"char"))
	  {
		printf("Voxel type is %s, file is binary, can only read (unsigned) chars.\n",voxel_type);
		return false;
	  }	
	  
	  Image<byte> buf(dx, dy);

	  for(int i=0;i<dz;i++) 
	  {
		int read=fread(buf[0], sizeof(byte), dx*dy, pfile);
		assert(read==dx*dy);

		Image<T> &slice = this->slices[i+pad];

		for(int k=0;k<dy;k++)
		  for(int j=0;j<dx;j++) 
		  {
			int val = buf[k][j];
			if(val) val = (val<<1) - 1;
			slice[k+pad][j+pad]=(T)val;
		  }
	  }	  
  }


  return true;
}

template< class T >
bool Volume<T>::readBinaryField(const char *fname, int pad, int &np, int *&points, float *&normals)
{
  FILE *pfile=fopen(fname, "rb");
  if (!pfile)
  {
	return false;
  }	
  
  char buffer[128];
  int dx=0, dy=0, dz=0, dummy1, dummy2, dummy3;
  
  unsigned char compressed = fgetc(pfile);

  assert(!compressed);

  uint64_t shape[3];
  fread(&shape, 3*sizeof(uint64_t), 1, pfile);

  dz = (int)shape[0];
  dy = (int)shape[1];
  dx = (int)shape[2];

  assert(dx>0 && dy>0 && dz>0);

  makeVolume(dx+2*pad, dy+2*pad, dz+2*pad);
  
  Image<byte> buf(dx, dy);

  for(int i=0;i<dz;i++) {
    int read=fread(buf[0], sizeof(byte), dx*dy, pfile);
    assert(read==dx*dy);

    Image<T> &slice = this->slices[i+pad];

    for(int k=0;k<dy;k++)
      for(int j=0;j<dx;j++) {
	int val = buf[k][j];
	if(val) val = 255;
	slice[k+pad][j+pad]=(T)val;
      }
  }

  np = 0;
  fread(&np, sizeof(int), 1, pfile);
  
  points = new int[3*np];
  normals = new float[3*np];

  fread(points, 3*sizeof(int), np, pfile);
  fread(normals, 3*sizeof(float), np, pfile);

  fclose(pfile);

  return true;
}


template< class T >
void Volume<T>::exportAVSField(const char *fname)
{
  FILE *pfile=fopen(fname, "w");

  fprintf(pfile, "# AVS field file %s\n#\n", fname);
  fprintf(pfile, "%s", "ndim=3\n");
  fprintf(pfile, "dim1=%d\n", getWidth());
  fprintf(pfile, "dim2=%d\n", getHeight());
  fprintf(pfile, "dim3=%d\n", getDepth());
  fprintf(pfile, "%s", "nspace=3\n");
  fprintf(pfile, "%s", "veclen=1\n");
  fprintf(pfile, "%s", "data=float\n");
  fprintf(pfile, "%s", "field=uniform\n");
  fprintf(pfile, "min_ext=%f %f %f\n", 0.0f, 0.0f, 0.0f);
  fprintf(pfile, "max_ext=%f %f %f\n", 
	  (float)getWidth(), (float)getHeight(), (float)getDepth());
  
  fputc(0x0c, pfile);
  fputc(0x0c, pfile);

  Image<float> slice(getWidth(), getHeight());
  char *odata = (char*)slice.getPixels();
  
  bool platform_big_endian = is_big_endian();				//Determine platform's endianness


    for (int k=0; k<getDepth(); k++) 
	{
      for (int j=0; j<getHeight(); j++)
		for (int i=0; i<getWidth(); i++) 
		{
	  	  T val = (*this)(i,j,k);
		  float fval = FloatToBigEndian(val);				//Convert the scalar value to a float. Next, convert it to big-endian
		  
	  	  fwrite(&fval, sizeof(float), 1, pfile);			//format (as this is required by the binary VTK file specification)
		}
	}	


  /* 
  for(int i=0;i<getDepth();i++) 
  {
    char* idata = (char*)slices[i].getPixels();

    if (!platform_big_endian)								//If platform is little endian, swap data while writing,
		for(int j=0;j<getWidth()*getHeight();j++)			//since AVS's binary format is big endian by default
		{
		  odata[j*4+0] = idata[j*4+3];
		  odata[j*4+1] = idata[j*4+2];
		  odata[j*4+2] = idata[j*4+1];
		  odata[j*4+3] = idata[j*4+0];
		}
	else													//If platform is big endian, write data directly
		for(int j=0;j<getWidth()*getHeight();j++)  
		{
		  odata[j*4+0] = idata[j*4+0];
		  odata[j*4+1] = idata[j*4+1];
		  odata[j*4+2] = idata[j*4+2];
		  odata[j*4+3] = idata[j*4+3];
		}

    fwrite(slice.getPixels(), sizeof(float), slice.getSize(), pfile);
    
  }*/
  
  fclose(pfile);
}

template< class T >
void Volume<T>::exportVolField(const char *fname,T threshold,byte fg_value,int pad)
{
  FILE *pfile=fopen(fname, "w");

  fprintf(pfile, "X: %d\n", getWidth()-2*pad);
  fprintf(pfile, "Y: %d\n", getHeight()-2*pad);
  fprintf(pfile, "Z: %d\n", getDepth()-2*pad);
  fprintf(pfile, "Version: 2\n");
  fprintf(pfile, "Voxel-Size: 1\n");
  fprintf(pfile, "Alpha-Color: 0\n");
  fprintf(pfile, "Int-Endian: 0123\n");
  fprintf(pfile, "Voxel-Endian: 0\n");
  fprintf(pfile, "Res-X: 1.000000\n");
  fprintf(pfile, "Res-Y: 1.000000\n");
  fprintf(pfile, "Res-Z: 1.000000\n");
  fputc(0x2E, pfile);
  fputc(0x0A, pfile);
  
  Image<byte> slice(getWidth()-2*pad, getHeight()-2*pad);
  for(int i=pad;i<getDepth()-pad;i++) 
  {
    for(int j=pad;j<getHeight()-pad;j++) 
	{
      for(int k=pad;k<getWidth()-pad;k++) 
	  {
	    T val =  (slices[i][j][k]>=threshold)? fg_value:0;
		slice[j-pad][k-pad] = (byte)val;
      }
    }
    fwrite(slice.getPixels(), sizeof(byte), slice.getSize(), pfile);
  }
  
  fclose(pfile);
} 


template< class T >
void Volume<T>::exportVTKField(const char *filename)
{
	FILE* f = fopen(filename,"wb");
	
	char buf[1024];

	strcpy(buf,"# vtk DataFile Version 3.0\n");
	fwrite(buf,sizeof(char),strlen(buf),f);

	strcpy(buf,"#generated by IMA3D\n");
	fwrite(buf,sizeof(char),strlen(buf),f);

	strcpy(buf,"BINARY\n");
	fwrite(buf,sizeof(char),strlen(buf),f);

	strcpy(buf,"DATASET STRUCTURED_POINTS\n");
	fwrite(buf,sizeof(char),strlen(buf),f);

	sprintf(buf,"DIMENSIONS %d %d %d\n",getWidth()+1,getHeight()+1,getDepth()+1);	//Needed to add +1 if we want cell-data
	fwrite(buf,sizeof(char),strlen(buf),f);
	
	strcpy(buf,"ORIGIN 0 0 0\n");
	fwrite(buf,sizeof(char),strlen(buf),f);
	
	strcpy(buf,"SPACING 1 1 1\n");
	fwrite(buf,sizeof(char),strlen(buf),f);

	int size = getDepth()*getHeight()*getWidth();
	sprintf(buf,"CELL_DATA %d\n",size);							//Replace this by POINT_DATA if you want point data
	fwrite(buf,sizeof(char),strlen(buf),f);
	
	strcpy(buf,"SCALARS voxel_data float\n");					//Save data as floats always. This allows us a simple way to handle
	fwrite(buf,sizeof(char),strlen(buf),f);						//different scalar types, e.g. chars, ints, floats, and similar.

	strcpy(buf,"LOOKUP_TABLE default\n");
	fwrite(buf,sizeof(char),strlen(buf),f);
		
    for (int k=0; k<getDepth(); k++) 
	{
      for (int j=0; j<getHeight(); j++)
		for (int i=0; i<getWidth(); i++) 
		{
	  	  T val = (*this)(i,j,k);
		  float fval = FloatToBigEndian(val);					//Convert the scalar value to a float. Next, convert it to big-endian
	  	  fwrite(&fval, sizeof(float), 1, f);					//format (as this is required by the binary VTK file specification)
		}
	}	

	fclose(f);
}




template< class T >
void Volume<T>::clearVolume(T val) 
{
    for(register int i=0;i<(int)slices.size();++i) 
      slices[i].clearImage(val);
}


template< class T >
void Volume<T>::clearVolume()
{
    for(register int i=0;i<(int)slices.size();++i) 
      slices[i].clearImage();
}
  

 

template class Volume<float>;      
template class Volume<byte>;      
template class Volume<char>;
template class Volume<int>;

