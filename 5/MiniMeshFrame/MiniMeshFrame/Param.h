#pragma once
#include <vector>
#include <cmath>
#include "HE_mesh\Mesh3D.h"
#include <Dense>
#include <Sparse>
#define SMALLVALUE  10E-6
#define PI 3.1415926
using namespace Eigen;
typedef SparseMatrix<float> SpMat; // ����һ�������ȵ�˫����ϡ���������
typedef Triplet<float> T; //��Ԫ�飨�У��У�ֵ��

class Param
{
public:
	virtual ~Param(){};
	virtual void Parameterize(Mesh3D * ptr_mesh) = 0;
};

