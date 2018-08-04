#ifndef MINSRF_H
#define MINSRF_H

#include "HE_mesh/Mesh3D.h"
#include "ND_Point.h"
#include <vector>
#include <cmath>
#include "HE_mesh\Mesh3D.h"
#include "triangle.h"
#include <Dense>
#include <Sparse>
#define SMALLVALUE  10E-6
#define PI 3.1415926
using namespace Eigen;
typedef SparseMatrix<float> SpMat; // ����һ�������ȵ�˫����ϡ���������
typedef Triplet<float> T; //��Ԫ�飨�У��У�ֵ��

class MinSrfLocal
{
public:
	static void Minimize(Mesh3D * mesh);
};

class MinSrfGlobal
{
public:
	static void Minimize(Mesh3D * mesh);
};

class MinSrfCurve
{
public:
	static void Minimize(std::vector<point> & curve, Mesh3D * mesh);
};

void genTriMesh(std::vector<point2> & V, std::vector<std::vector<int>> & faces);
#endif// MINSRF_H