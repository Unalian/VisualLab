#include "include/Image.h"
#include "include/Vector.h"
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>




template< class T > Image< T >::Image()
{
  width = height = 0; 
  pixels = 0;
}

template< class T > Image< T >::Image(int w, int h)
{
    pixels = (T **)calloc(h, sizeof(T *));
    assert(pixels);
    pixels[0] = (T *)calloc(w*h, sizeof(T));
    assert(pixels[0]);

    for(int i=1;i<h;i++)
       pixels[i] = pixels[i-1] + w;

  width = w; height = h;
}

template< class T > Image< T >::Image(const Image<T>& img)
{
  width  = img.width;
  height = img.height;
  pixels = 0;
  if (img.width*img.height != 0) 
      setPixels(img.width, img.height, img.pixels[0]);
}

template< class T > void Image< T >::destroyImage() 
{
  if(pixels != 0) 
  {
    free(pixels[0]);
    free(pixels);
    pixels = 0;
    width=height=0;
  }
}

template< class T > Image< T >::~Image()
{ 
  destroyImage();
}

template< class T > void Image< T >::makeImage(int w, int h)
{
    destroyImage(); 

    pixels = (T **)calloc(h, sizeof(T*));
    assert(pixels);
    pixels[0] = (T*)calloc(w*h, sizeof(T));
    assert(pixels[0]);


    for(int i=1;i<h;i++)
      pixels[i] = pixels[i-1] + w;

    width = w; height = h;
}

template< class T > void Image< T >::setPixels(int w, int h, T* data)
{
    makeImage(w, h);

    assert(data);
    memcpy(pixels[0], data, getSize()*sizeof(T));
}


template< class T > Image< T >& Image< T >::operator=(const Image< T >& img)
{
  if(this != &img) 
  {
    width = img.width;
    height = img.height;
    if (img.width*img.height != 0)
      setPixels(img.width, img.height, img.pixels[0]);
    else
      pixels = 0;
  }
  return *this;
}

template< class T > int Image< T >::operator==(const Image< T >&img)
{
  if (width==img.width && height==img.height)
    return (!memcmp(pixels[0], img.pixels[0], getSize()*sizeof(T)));
  else
    return (width==img.width && height==img.height); 
}

template< class T > int Image< T >::operator!=(const Image< T >&img)
{
  if (width==img.width && height==img.height)
    return (memcmp(pixels[0], img.pixels[0], getSize()*sizeof(T)));
  else
    return (width!=img.width && height!=img.height); 
}

template< class T > Image< T > &Image< T >::operator+=(const T val)
{
  for (int i=0;i<getSize();i++) 
    pixels[0][i] += val;
  return (*this);
}

template< class T > Image< T > &Image< T >::operator-=(const T val)
{
  for (int i=0;i<getSize();i++) 
    pixels[0][i] -= val;
  return (*this);
}

template< class T > Image< T > &Image< T >::operator+=(const Image< T >&img)
{
  if(width!=img.width || height!=img.height)
    printf("Error: The two images have different sizes");
  else
  {
    for(int i=0;i<getSize();i++)
      pixels[0][i] += img.pixels[0][i];
    return *this;
  }
}

template< class T > Image< T > &Image< T >::operator-=(const Image< T >&img)
{
  if(width!=img.width || height!=img.height)
  {
    printf("Error: The two images have different sizes");
  }
  else
  {
    for(int i=0;i<getSize();i++)
      pixels[0][i] -= img.pixels[0][i];
    return *this;
  }
}

template< class T > void Image< T >::clearImage() 
{ 
  memset(pixels[0],0,sizeof(T)*getSize());
}

template< class T > void Image< T >::clearImage(const T val) 
{ 
  for (int i=0,sz=getSize(); i<sz; ++i)
    pixels[0][i] = val;
}


template class Image< char >;
template class Image< byte >;
template class Image< short >;
template class Image< int >;
template class Image< float >;
template class Image< double >;
template class Image< Vector >;

