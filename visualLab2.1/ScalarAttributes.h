#pragma once

#include <vector>

using namespace std;

class ScalarAttributes
{
public:
				ScalarAttributes(int size):values(size) { }//构造函数确定vector数据结构的大小

void			setC0Scalar(int i,float v)
				{  values[i] = v;  }

float			getC0Scalar(int i)
				{  return values[i];  }	

float			getMin();//获取标量值中最小者

float			getMax();//获取标量值中最大者
				
protected:

vector<float>	values;//标量值

};
