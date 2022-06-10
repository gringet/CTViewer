#pragma once

#include "MenuBar.h"
#include "InfoBox.h"
#include "QtVTKFourPanesBox.h"
#include "DebugBox.h"
#include "ToolsBox.h"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLayout>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QGroupBox>


class CTViewer : public QWidget
{
Q_OBJECT
public:
    CTViewer(QWidget *parent = Q_NULLPTR);

private:
    InfoBox* infoBox;
    QtVTKFourPanesBox* fourPanesBox;
    DebugBox* debugBox;
    ToolsBox* toolsBox;

private slots:
    void OnOpenCT(QString path);
    void HistogramSlot(void);

};
