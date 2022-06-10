#include "CTViewer.h"


#include "Histogram.h"


CTViewer::CTViewer(QWidget *parent): QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    // Splitter to separate toolsBox/imagesBox from debugBox
    QSplitter* vSplitter = new QSplitter(Qt::Orientation::Vertical);
    vSplitter->setOpaqueResize(false);
    vSplitter->setHandleWidth(8);
    mainLayout->addWidget(vSplitter);

    // Splitter to separate infoBox / toolsBox from imagesBox
    QSplitter* hSplitter = new QSplitter(Qt::Orientation::Horizontal);
    hSplitter->setOpaqueResize(false);
    hSplitter->setHandleWidth(8);
    vSplitter->addWidget(hSplitter);
    vSplitter->setStretchFactor(0, 1);

    QWidget* infoToolsWidget = new QWidget();
    QVBoxLayout* infoToolsLayout = new QVBoxLayout();
    infoToolsLayout->setContentsMargins(0, 0, 0, 0);
    infoToolsWidget->setLayout(infoToolsLayout);
    hSplitter->addWidget(infoToolsWidget);
    hSplitter->setStretchFactor(0, 0);

    // Info Box
    this->infoBox = new InfoBox();
    this->infoBox->setObjectName("InfoBox");
    infoToolsLayout->addWidget(this->infoBox);

    // Tools Box
    this->toolsBox = new ToolsBox();
    this->toolsBox->setObjectName("ToolsBox");
    infoToolsLayout->addWidget(this->toolsBox);

    // ImagesBox
    this->fourPanesBox = new QtVTKFourPanesBox();
    this->fourPanesBox->setObjectName("imagesBox");
    hSplitter->addWidget(fourPanesBox);
    hSplitter->setStretchFactor(1, 1);

    // DebugBox
    this->debugBox = new DebugBox();
    this->debugBox->setObjectName("debugBox");
    vSplitter->addWidget(this->debugBox);
    vSplitter->setStretchFactor(1, 0);

    // MenuBar. Keeps at the end since it needs the QWidgets pointers.
    std::list<QWidget*> widgets = { this->infoBox, this->fourPanesBox, this->debugBox };
    MenuBar* menuBar = new MenuBar(this, widgets);
    mainLayout->setMenuBar(menuBar);

    connect(this->fourPanesBox, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(this->fourPanesBox, SIGNAL(patientNameChangedSignal(QString)), this->infoBox, SLOT(setPatientNameSlot(QString)));
    connect(menuBar, SIGNAL(OnOpenCTSignal(QString)), this, SLOT(OnOpenCT(QString)));
    connect(menuBar, SIGNAL(ViewChangeSignal(ViewType)), this->fourPanesBox, SLOT(ViewChangeSlot(ViewType)));
    connect(menuBar, SIGNAL(HistogramSignal(void)), this, SLOT(HistogramSlot(void)));
    connect(
        this->fourPanesBox, &QtVTKFourPanesBox::OnCTCoordinatesSignal,
        this->infoBox, &InfoBox::setCTCoordinatesSlot
    );
    connect(
        this->fourPanesBox, &QtVTKFourPanesBox::OnCTIndexesSignal,
        this->infoBox, &InfoBox::setPixelIndexSlot
    );
    connect(menuBar, SIGNAL(AddToolSignal(ToolType)), this->fourPanesBox, SLOT(AddToolSlot(ToolType)));
    connect(menuBar, SIGNAL(DeleteAllToolsSignal(void)), this->fourPanesBox, SLOT(DeleteAllToolsSlot(void)));
    this->debugBox->append("CTViewer started.");
}


void CTViewer::HistogramSlot(void)
{
    Histogram* hist = new Histogram(this, this->fourPanesBox->GetImageReader());
    hist->setWindowFlag(Qt::Window, true);
    hist->show();
}


void CTViewer::OnOpenCT(QString path)
{
    this->fourPanesBox->OpenCT(path.toStdString().c_str());
    this->debugBox->append("Open file \"" + path + "\"");
}
