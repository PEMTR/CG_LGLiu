#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QRect>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <qfiledialog.h>
#include <iostream>
#include <vector>
#include "Interpolation.h"
#include "IDW.h"
#include "RBF.h"
#include "Line.h"
#include <Eigen/Dense>
#include <cmath>
#include <qstatusbar.h>
using namespace Eigen;

enum MODE{
	IDLE,//�ȴ�
	GET,//��ȡ���ݵ�
	DRAW,//��������
};

class ImageWidget : public QWidget{
	Q_OBJECT

public:
	ImageWidget();
	~ImageWidget();

	// event
	void mousePressEvent(QMouseEvent * mouseEvent);
	void mouseMoveEvent(QMouseEvent * mouseEvent);
	void mouseReleaseEvent(QMouseEvent * mouseEvent);
    void paintEvent(QPaintEvent *);

	// File IO
	void Open();												// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// warp
	void Get();
	void WarpIDW();
	void WarpRBF();
	// ��հ�
	void fix(QImage * newImage, MatrixXd *flag);

	// ��getʱ�ɳ������ݵ㣬�ڷ�getʱ�ɳ���ͼ����Σ�idw��rbf������
	void Undo();
	

private:
	// ��lineVec�����г�Աת����QPoint������p��q��
	void lineVecTo(std::vector<QPoint *> *p, std::vector<QPoint *> *q);
	void warp(Interpolation * interpolation);
	// �����ж�����λ���Ƿ���ͼ����
	bool insideImg(QPoint p);

	QImage *currentImage;
	Line *currentLine;
	std::vector<Line *> lineVec;
	std::vector<QImage *> imageVec;
	QString filename;
	int mode;
};

#endif // IMAGEWIDGET_H
