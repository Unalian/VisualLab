#pragma once

#include <math.h>

struct Vector
{
 float u,v,w;
 
 Vector& operator+=(const Vector& a)
 {
	u += a.u;
	v += a.v;
	w += a.w;
	return *this;
 }
 
 Vector& operator-=(const Vector& a)
 {
	u -= a.u;
	v -= a.v;
	w -= a.w;
	return *this;
 }
 
 float norm() const
 {  return sqrtf(u*u + v*v + w*w); }

 float norm2() const
 {  return u*u + v*v + w*w; }
};




