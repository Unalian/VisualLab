#pragma once

typedef unsigned char byte;



template<class T> class Image				//A simple class that represents a 2D image.
{
 public:

  T **pixels;

 public:
			Image();
			Image(int w, int h);
			Image(const Image<T> &);
  virtual  ~Image();
  int		getWidth() const { return width; }
  int		getHeight() const { return height; }
  int		getSize() { return width*height; }
  void		makeImage(int w, int h);
  T*		getPixels() { return pixels[0]; }
  void		setPixels(int w, int h, T *data);  
  T*		operator[](short i) { return pixels[i]; }
  const T*	operator[](short i) const { return pixels[i]; }
  T&		operator()(short i, short j) {  return pixels[0][i*width+j];  }
  const T&  operator()(short i, short j) const {  return pixels[0][i*width+j];  }
  Image<T>& operator=(const Image< T > &img); 
  int		operator==(const Image< T > &img);
  int		operator!=(const Image< T > &img);
  Image<T>& operator+=(const T val);
  Image<T>& operator-=(const T val);
  Image<T>& operator*=(const T val);
  Image<T>& operator+=(const Image< T > & img);
  Image<T>& operator*=(const Image< T > & img);
  Image<T>& operator-=(const Image< T > & img);
  void      clearImage(const T val);
  void      clearImage();

protected: 
  
  short		width, height; 
  void		destroyImage();
};









