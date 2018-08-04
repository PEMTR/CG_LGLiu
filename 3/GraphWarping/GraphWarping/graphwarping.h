#ifndef GRAPHWARPING_H
#define GRAPHWARPING_H

#include "ui_graphwarping.h"
#include <QtWidgets\QMainWindow>
#include <QtWidgets\qaction.h>
#include <QtWidgets\qmenu.h>
#include <QtWidgets\qmenubar.h>
#include <QtWidgets\qtoolbar.h>
#include <qline.h>
#include "imagewidget.h"
#include <vector>
#include <string>

class GraphWarping : public QMainWindow{
	Q_OBJECT

public:
	GraphWarping(QWidget * parent = 0);
	~GraphWarping();
	void activate(int id);
	void inactivate(int id);
	// ���ڴ� imagewidget ���չ��ڸı� icon ���ź�
	bool event(QEvent * event);

private:
	void init();

	Ui::GraphWarpingClass ui;
	QMenu *fileMenu, *warpMenu;
	QToolBar *fileToolBar, *warpToolBar;
	std::vector<QIcon *> icons;
	std::vector<QAction *> actions;
	std::string toolName[TOOLNUM];
	// �Զ���� eventType
	// eventType[i] > QEvent::User, i = 0, 1, ..., 2*TOOLNUM-1
	QEvent::Type eventType[2 * TOOLNUM];
	int iconState[TOOLNUM];
	ImageWidget * imagewidget;
};

#endif // GRAPHWARPING_H
