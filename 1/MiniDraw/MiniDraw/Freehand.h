#ifndef FREEHAND_H
#define FREEHAND_H

#include <QPainterPath>
#include "Figure.h"

class Freehand : public Figure{
public:
    Freehand(const QPoint _start_point);
    void Draw(QPainter &paint);

    // ���ڶ�̬����
    void update(const QPoint _end_point);

private:
    QPainterPath path;
};

#endif // FREEHAND_H