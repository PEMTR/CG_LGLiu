#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
 
#include <QWidget>
#include <QImage>
#include <QRect>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QtWidgets\qtoolbar.h>
#include <qfiledialog.h>
#include <iostream>
#include <vector>
#include "Interpolation.h"
#include "IDW.h"
#include "RBF.h"
#include "Line.h"
#include "Mesh.h"
#include "TypeCvt.h"
#include <cmath>
#include <qstatusbar.h>
#include <malloc.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

#include <Eigen/Dense>
using namespace Eigen;



#define FILETOOLNUM 3
#define WARPTOOLNUM 8
#define TOOLNUM ((FILETOOLNUM)+(WARPTOOLNUM))
#define PREFIX (std::string(":/GraphWarping/Resources/images/"))

// �����¼����������� GraphWarping �� tool �� icon
#define SW(tool, status) (parent()->event(&QEvent(eventType[((tool)<<1)|(status)])))
// status
#define INACTIVATE 0
#define ACTIVATE 1

namespace TOOL{
	enum {
		OPEN,
		SAVE,
		SAVEAS,
		GET,
		FEATURE,
		IDW,
		RBF,
		RECT_MESH,
		TRI_RAND_MESH,
		TRI_FEATURE_MESH,
		UNDO,
	};
}

enum MODE{
	IDLE,//�ȴ�
	GET,//��ȡ���ݵ�
	DRAW,//��������
};

// FP �� ����ָ��
class ImageWidget;//������ǰ
typedef void (ImageWidget::* FP)();

class ImageWidget : public QWidget{
	Q_OBJECT

public:
	ImageWidget(QEvent::Type * _eventType);
	~ImageWidget();

	// event
	void mousePressEvent(QMouseEvent * mouseEvent);
	void mouseMoveEvent(QMouseEvent * mouseEvent);
	void mouseReleaseEvent(QMouseEvent * mouseEvent);
    void paintEvent(QPaintEvent *);

	// File IO
	void Open();
	void Save();
	void SaveAs();

	// warp
	void Get();
	void Feature();
	void WarpIDW();
	void WarpRBF();
	void WarpRectMesh();
	void WarpTriRandMesh();
	void WarpTriFeatureMesh();
	void Undo();// ��getʱ�ɳ������ݵ㣬�ڷ�getʱ�ɳ���ͼ����εĲ���

	// ��հ�
	static void fix(QImage * img, MatrixXi & flag);

	// ���ظ���ĺ���ָ��
	static FP method(int id);

private:
	// ��lineVec�����г�Աת����QPoint������p��q��
	void lineVecTo(std::vector<ND_Point> & _pVec, std::vector<ND_Point> & fpVec);

	// �����ж�����λ���Ƿ���ͼ����
	std::vector<ND_Point> allImgP();

	// ��ȡͼ������Ͻǵ����꣬���ͼ�񲻴��ڣ��򷵻�(-1, -1)
	QPoint imgIdx();

	// ����Icon
	void updateIcon();

	QImage * curImg;
	Line * curLine;
	std::vector<Line *> lineVec;
	std::vector<QImage *> imageVec;
	QString filename;
	int mode;
	
	// feature points of curImg
	std::vector<cv::KeyPoint> keypoints;
	//featureState==0 : û����ʾ������
	//featureState==1 : ��ʾ��������
	int featureState;

	// �Զ���� eventType
	// eventType[i] > QEvent::User, i = 0, 1, ..., 2*TOOLNUM-1
	QEvent::Type eventType[2 * TOOLNUM];
};

#endif // IMAGEWIDGET_H
