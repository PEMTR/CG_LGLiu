#include "minidraw.h"
#include <QtWidgets\qmenubar.h>
#include <string>
#include <QtWidgets\qmessagebox.h>
#include <QToolBar>
#include<QMessageBox>

#define TOOLNUM 6

MiniDraw::MiniDraw(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    init();
}

MiniDraw::~MiniDraw()
{
    for(int i=0; i<actionVec.size(); i++)
        delete actionVec[i];
    delete view_widget;
}

void MiniDraw::init(){
    // ��Ӳ˵� Figure
    menu = menuBar()->addMenu(tr("Figure"));

    // ��ԭ�е� toolBar �� mainToolBar ����Ϊ Figure Tool
    // �̳��е��������£�
    // toolBar = addToolBar(tr("Figure Tool"));
    // ��ᵼ�²����ڶ��� toolBar������
    toolBar = findChild<QToolBar*>(tr("mainToolBar"));
    toolBar->setWindowTitle(tr("Figure Tool"));

    // ���ɰ���
    std::string toolName[TOOLNUM] = {"Line", "Rectangle",
        "Ellipse", "Polygon", "Freehand", "Undo"};
    std::string statusTips[TOOLNUM] = {"Select line tool.", "Select rectangle tool.",
        "Select ellipse tool.", "Select polygon tool.", "Select freehand tool.", "Delect lastest firgue."};
    for(int i=0; i<TOOLNUM; i++){
        actionVec.push_back(new QAction(
            QIcon(tr((std::string(":MiniDraw/images/")+toolName[i]+std::string(".bmp")).c_str())),
            tr(toolName[i].c_str()), this));
        toolBar->addAction(actionVec[i]);
        menu->addAction(actionVec[i]);
        actionVec[i]->setStatusTip(tr(statusTips[i].c_str()));
    }

    // �źŲ�
    view_widget = new ViewWidget();
    setCentralWidget(view_widget);
    connect(actionVec[0], &QAction::triggered, view_widget, &ViewWidget::set_figure_type_to_line);
    connect(actionVec[1], &QAction::triggered, view_widget, &ViewWidget::set_figure_type_to_rectangle);
    connect(actionVec[2], &QAction::triggered, view_widget, &ViewWidget::set_figure_type_to_ellipse);
    connect(actionVec[3], &QAction::triggered, view_widget, &ViewWidget::set_figure_type_to_polygon);
    connect(actionVec[4], &QAction::triggered, view_widget, &ViewWidget::set_figure_type_to_freehand);
    connect(actionVec[5], &QAction::triggered, view_widget, &ViewWidget::undo);
}
