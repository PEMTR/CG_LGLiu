#include "viewwidget.h"
#include <QMouseEvent>
#include <qpainter.h>

ViewWidget::ViewWidget(QWidget *parent)
    : QWidget(parent){
    ui.setupUi(this);
    draw_status = false;
    current_figure = NULL;
    figure_type = kDefault;
}

ViewWidget::~ViewWidget(){
    for(int i=0; i<figureVec.size(); i++){
        if(figureVec[i]){
            delete figureVec[i];
            figureVec[i] = NULL;
        }
    }

    // ÿ�ΰ� current_figure push to figureVec �󶼽�������Ϊ NULL
    // �������� delete ������� delete ���ᴦ��ͬһָ��
    if(current_figure != NULL)
        delete current_figure;
}

void ViewWidget::mousePressEvent(QMouseEvent *mouseEvent){
	if (Qt::LeftButton == mouseEvent->button()){
		draw_status = true;
		start_point = end_point = mouseEvent->pos();

        switch (figure_type){
	    case kDefault:
            // ���ڳ�ʼ״̬
            draw_status = false;
		    break;
	    case kLine:
		    current_figure = new Line(start_point, end_point);
		    break;
        case kRectangle:
		    current_figure = new Rect(start_point, end_point);
		    break;
	    case kEllipse:
		    current_figure = new Ellipse(start_point, end_point);
		    break;
        case kPolygon:
            if( current_figure == NULL ){
		        current_figure = new Polygon(start_point);
                // �������׷��
                // ��������Ƿ��м����£������������ mouseMoveEvent
                setMouseTracking(true);
            }
            break;
        case kFreehand:
            current_figure = new Freehand(start_point);
            break;
	    default:
		    break;
	    }
	}
}

void ViewWidget::mouseMoveEvent(QMouseEvent *mouseEvent){
	if (draw_status){
		end_point = mouseEvent->pos();
        current_figure->update(end_point);
    }
}

void ViewWidget::mouseReleaseEvent(QMouseEvent *mouseEvent){	
    if(figure_type != kPolygon){
        draw_status = false;
        if(current_figure != NULL){
            figureVec.push_back(current_figure);
	        current_figure = NULL;
        }
    }else{// figure_type == kPolygon
        if(Qt::LeftButton == mouseEvent->button())
            current_figure->update(1);// ����һ����
        if(Qt::RightButton == mouseEvent->button()){
            draw_status = false;
            current_figure->update(0);// �������
            figureVec.push_back(current_figure);
            current_figure = NULL;
            setMouseTracking(false);
        }
    }
}

void ViewWidget::paintEvent(QPaintEvent *)
{
	QPainter painter;
	painter.begin(this);

    for(int i=0; i < figureVec.size(); i++)
        figureVec[i]->Draw(painter);
    if(current_figure != NULL)
        current_figure->Draw(painter);

	painter.end();
	update();
}

void ViewWidget::set_figure_type_to_line(){
	if(draw_status == false)
        figure_type = kLine;
}

void ViewWidget::set_figure_type_to_ellipse(){
    if(draw_status == false)
	    figure_type = kEllipse;
}

void ViewWidget::set_figure_type_to_rectangle(){
	if(draw_status == false)
        figure_type = kRectangle;
}

void ViewWidget::set_figure_type_to_polygon(){
	if(draw_status == false)
        figure_type = kPolygon;
}

void ViewWidget::set_figure_type_to_freehand(){
    if(draw_status == false)
        figure_type = kFreehand;
}

void ViewWidget::undo(){
	if( figureVec.size() > 0 ){
        delete figureVec.back();
        figureVec.pop_back();
    }
}