#pragma once

#include <QtWidgets/QMenuBar>
#include <QFileDialog>
#include <list>


enum ViewType {
	FourPanes,
	ThreePanes,
	Volume
};


enum ToolType {
	None,
	Point,
	Ruler
};


class MenuBar: public QMenuBar
{
Q_OBJECT
public:
	MenuBar(QWidget* parent, std::list<QWidget *>);

signals:
	void OnOpenCTSignal(QString path);
	void ViewChangeSignal(ViewType viewType);
	void AddToolSignal(ToolType toolType);
	void DeleteAllToolsSignal(void);
	void HistogramSignal(void);

private:
	void OnOpenCT(void);
};

