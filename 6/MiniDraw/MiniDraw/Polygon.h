#ifndef POLYGON_H
#define POLYGON_H

#include "Figure.h"
#include <vector>

class Polygon : public Figure{
public:
    Polygon(const QPoint _start_point);
    // ���ڶ�̬����
	void update(const QPoint _end_point);

    // ���ڸı� Polygon ��״̬
    // mode == 0 ���������
    // mode == 1 : ����һ����
    void update(int mode);
    void Draw(QPainter &paint);
	std::vector<QPoint *> getPoint(QPoint & pos, bool all);
protected:
    // QPolygon �̳��� QVector, ������ vector
    // ��ֱ�����ڽӿ� drawPolygon �� drawPolyline
    QPolygon points;
    bool done;
};

#endif // POLYGON_H