#ifndef RBF_H
#define RBF_H

#include "interpolation.h"
#include <cmath>
#include <Eigen/Dense>
using namespace Eigen;

class RBF : public Interpolation{
public:
	RBF(std::vector<QPoint *> *p, std::vector<QPoint *> *q);
	~RBF();

	QPoint map(QPoint p);

private:
	std::vector<QPoint *> pVec, qVec;

	// RBF��ص�����ϵ��, RBF_A = [a, alpha]
	MatrixXd *RBF_A;
	double r, u;
};

#endif // RBF_H

