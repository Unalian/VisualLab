#pragma once

#include <vector>

using namespace std;

class ScalarAttributes
{
public:
				ScalarAttributes(int size):values(size) { }//���캯��ȷ��vector���ݽṹ�Ĵ�С

void			setC0Scalar(int i,float v)
				{  values[i] = v;  }

float			getC0Scalar(int i)
				{  return values[i];  }	

float			getMin();//��ȡ����ֵ����С��

float			getMax();//��ȡ����ֵ�������
				
protected:

vector<float>	values;//����ֵ

};
