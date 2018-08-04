#ifndef LINE_H
#define LINE_H

#include <qpainter.h>
#include <qpoint.h>
#include <qpen.h>

class Line{
public:
    Line(const QPoint _start_point, const QPoint _end_point);
    void Draw(QPainter &paint);

    // ���ڶ�̬����
    void update(const QPoint _end_point);
	QPoint & getStartPoint();
	QPoint & getEndPoint();

private:
    // һ��ʼ�����Լ������ struct point�������� QT �� QPoint
    // ͬ����֮��ʹ���� QPolygon�� QPainterPath ��
    QPoint start_point, end_point;
	QPen linePen, pointPen;
};

#endif //LINE_H