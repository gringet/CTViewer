#pragma once
#include "CTVolume.h"
#include "MenuBar.h"

#include "vtkSmartPointer.h"
#include "QVTKOpenGLNativeWidget.h"
#include "vtkResliceImageViewer.h"
#include "vtkImageReslice.h"
#include "vtkDICOMImageReader.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorObserver.h"
#include "vtkDistanceWidget.h"
#include "vtkVolume.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkImageThreshold.h"
#include "vtkAbstractWidget.h"

#include "ThresholdingBox.h"

#include <QtWidgets/QGroupBox>
#include <QWheelEvent>
#include <QSlider>


class QtVTKFourPanesBox : public QGroupBox
{
Q_OBJECT
public:
	// Constructor/Destructor
	QtVTKFourPanesBox();
	~QtVTKFourPanesBox() override {};

	void OpenCT(QString path);
	vtkSmartPointer < vtkDICOMImageReader > GetImageReader(void)
	{
		return this->imageReader;
	}

public slots:
	virtual void sliderChanged(int value);
	virtual void ViewChangeSlot(ViewType viewType);
	virtual void AddToolSlot(ToolType toolType);
	virtual void DeleteAllToolsSlot(void);

signals:
	void patientNameChangedSignal(QString name);
	void OnCTCoordinatesSignal(double x, double y, double z);
	void OnCTIndexesSignal(int x, int y, int z);

private:
	void reset(void);
	void SetSliders(void);
	void ResetLayout(void);
	void SetToFourPanesView(void);
	void SetToThreePanesView(void);
	void SetToVolumeView(void);
	void AddTool(int view, int xPosition, int yPosition);
	void AddPointToViews(int view, int xPosition, int yPosition);
	void AddRulerToView(int view);

	QSlider* sliders[3];
	CTVolume* ctVolume;
	QVTKOpenGLNativeWidget *imageWidgets[3];
	vtkSmartPointer < vtkImageReslice > imageReslice[3];
	vtkSmartPointer < vtkResliceImageViewer > imageViews[3];
	vtkSmartPointer < vtkVolume > volumeView;
	vtkSmartPointer < vtkDICOMImageReader > imageReader;
	std::list< vtkSmartPointer < vtkAbstractWidget > > widgets;

	ToolType toolAdded = ToolType::None;
};

