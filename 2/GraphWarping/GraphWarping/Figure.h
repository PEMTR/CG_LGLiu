#ifndef FIGURE_H
#define FIGURE_H

#include <qpainter.h>
#include <qpoint.h>

class Figure{
public:
    virtual ~Figure(){};
    virtual void Draw(QPainter &paint)=0;

    // �ṩ��̬���µĽӿ�
    virtual void update(const QPoint _end_point){};

    // �ṩ�� Polygon �Ľӿ�
    virtual void update(int mode){};
};

#endif //FIGURE_H