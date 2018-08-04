#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Figure.h"

class Ellipse : public Figure{
public:
	Ellipse(const QPoint _start_point, const QPoint _end_point);
	void Draw(QPainter &paint);

    // ���ڶ�̬����
	void update(const QPoint _end_point);

private:
	QPoint start_point, end_point;
};


#endif //ELLIPSE_H