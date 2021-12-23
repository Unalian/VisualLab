#pragma once


struct coord3											//Models the (integer) coordinates of a voxel in a 3D volume
{
	short int x, y, z;

	bool operator==(const coord3 &a) const 
	{
	  return (a.x==x) && (a.y==y) && (a.z==z);
	}
	
	bool operator<(const coord3 &a) const				//lexicografic compare (for inserting coord3's in STL containers)
	{
	  if(z<a.z) return true;
	  if(z>a.z) return false;
	  if(y<a.y) return true;
	  if(y>a.y) return false;
	  if(x<a.x) return true;
	  return false;
	}

    coord3() {}
	coord3(short xx, short yy, short zz) : x(xx), y(yy), z(zz) {};
};




