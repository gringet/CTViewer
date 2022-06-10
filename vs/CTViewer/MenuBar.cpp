#include "MenuBar.h"

MenuBar::MenuBar(
    QWidget* parent,
    std::list<QWidget *> widgetsList
) : QMenuBar(parent)
{
    // FileMenu
    QMenu* fileMenu = this->addMenu("File");
    QAction* openCT = fileMenu->addAction("Open CT");
    openCT->setShortcut(QKeySequence("Ctrl+o"));
    connect(openCT, &QAction::triggered, this, [&]() {this->OnOpenCT(); });

    // WindowMenu
    QMenu* windowMenu = this->addMenu("Window");
    QAction* action;
    for (QWidget* widget: widgetsList)
    {
        if (!widget)
            continue;
        action = windowMenu->addAction(widget->objectName());
        connect(
            action, &QAction::triggered, this,
            [widget]() {
                bool show = !(widget->isVisible());
                widget->setVisible(show);
            }
        );
    }

    // ViewMenu
    QMenu* viewMenu = this->addMenu("View");
    action = viewMenu->addAction("Four Panes");
    connect(action, &QAction::triggered, this, [&]() { emit ViewChangeSignal(ViewType::FourPanes); });
    action = viewMenu->addAction("Three Panes");
    connect(action, &QAction::triggered, this, [&]() { emit ViewChangeSignal(ViewType::ThreePanes); });
    action = viewMenu->addAction("Volume Pane");
    connect(action, &QAction::triggered, this, [&]() { emit ViewChangeSignal(ViewType::Volume); });

    // Tools Menu
    QMenu* toolsMenu = this->addMenu("Tools");
    action = toolsMenu->addAction("Add Point");
    connect(action, &QAction::triggered, this, [&]() { emit AddToolSignal(ToolType::Point); });
    action = toolsMenu->addAction("Add Ruler");
    connect(action, &QAction::triggered, this, [&]() { emit AddToolSignal(ToolType::Ruler); });
    action = toolsMenu->addAction("Delete all");
    connect(action, &QAction::triggered, this, [&]() { emit DeleteAllToolsSignal(); });

    // Stats Menu
    QMenu* statsMenu = this->addMenu("Stats");
    action = statsMenu->addAction("Histogram");
    connect(action, &QAction::triggered, this, [&]() { emit HistogramSignal(); });
}


void MenuBar::OnOpenCT(void)
{
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
    QString fileName = fileDialog->getExistingDirectory();
    emit OnOpenCTSignal(fileName);
}

