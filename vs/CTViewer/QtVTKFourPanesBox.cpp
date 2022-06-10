#include "QtVTKFourPanesBox.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkCallbackCommand.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkPointHandleRepresentation2D.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkCommand.h"
#include "vtkHandleWidget.h"
#include "vtkSeedWidget.h"
#include "vtkSeedRepresentation.h"
#include <vtkSphereHandleRepresentation.h>
#include <vtkSphereWidget2.h>
#include "vtkSphereRepresentation.h"
#include <vtkSphereHandleRepresentation.h>
#include "vtkPointHandleRepresentation3D.h"
#include "vtkConstrainedPointHandleRepresentation.h"

#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkVolumeProperty.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkCamera.h"
#include "vtkRayCastImageDisplayHelper.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkVolumeMapper.h"
#include "vtkRayCastImageDisplayHelper.h"
#include "vtkOpenGLRayCastImageDisplayHelper.h"
#include "vtkOpenGLGPUVolumeRayCastMapper.h"

#include "vtkTransformCoordinateSystems.h"
#include "vtkMatrix4x4.h"
#include "vtkBoundedPlanePointPlacer.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkImageData.h"
#include "vtkResliceCursorWidget.h"
#include "vtkAbstractTransform.h"
#include "vtkViewport.h"
#include "vtkResliceCursor.h"
#include "vtkImageActor.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkPointWidget.h"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QDebug>




class vtkWindowLevelCallback : public vtkCommand
{
public:
    static vtkWindowLevelCallback* New()
    {
        return new vtkWindowLevelCallback;
    }

    void Execute(vtkObject* caller, unsigned long event, void* callData) override
    {
		vtkInteractorStyleImage* interactor = dynamic_cast<vtkInteractorStyleImage*>(caller);
		if (!interactor)
			return;

		for (int i = 0; i < 3; ++i)
		{
			// Search for the image whose LevelWindow has been changed
			if (this->imageViews[i]->GetInteractorStyle() == interactor)
			{
				double window = this->imageViews[i]->GetColorWindow();
				double level = this->imageViews[i]->GetColorLevel();
				for (int ii = 0; ii < 3; ++ii)
				{
					// Change the other image LevelWindow accordingly
					if (!(ii == i))
					{
						this->imageViews[ii]->SetColorWindow(window);
						this->imageViews[ii]->SetColorLevel(level);
						this->imageViews[ii]->Render();
					}
				}
				break;
			}
		}
    }

	vtkWindowLevelCallback() {}
	vtkSmartPointer < vtkResliceImageViewer > imageViews[3];
};


class vtkMouseMoveCallback : public vtkCommand
{
public:
	static vtkMouseMoveCallback* New()
	{
		return new vtkMouseMoveCallback;
	}

	void Execute(vtkObject* caller, unsigned long event, void* callData) override
	{
		vtkRenderWindowInteractor* interactor = dynamic_cast<vtkRenderWindowInteractor*>(caller);
		if (!interactor)
			return;

		for (int i = 0; i < 3; ++i)
		{
			// Search for the image whose interactor has been triggered
			if (this->imageViews[i] && this->imageViews[i]->GetInteractor() == interactor)
			{
				int* eventPosition = interactor->GetEventPosition();
				double displayCoordinates[3] = { eventPosition[0], eventPosition[1], 0};

				vtkNew<vtkCoordinate> coordinates;
				coordinates->SetCoordinateSystemToDisplay();
				coordinates->SetValue(displayCoordinates);
				double* CTCoordinates = coordinates->GetComputedWorldValue(imageViews[i]->GetRenderer());

				int sliceAxis;
				switch (i)
				{
				case 0: sliceAxis = 2; break;
				case 1: sliceAxis = 1; break;
				case 2: sliceAxis = 0; break;
				}

				double* pixelSpacing = this->imageReader->GetPixelSpacing();
				CTCoordinates[sliceAxis] = this->imageViews[i]->GetSlice() * pixelSpacing[sliceAxis];

				int indexes[3];
				for (int ii = 0; ii < 3; ++ii)
				{
					indexes[ii] = CTCoordinates[ii] / pixelSpacing[ii];
				}

				if(this->OnCTCoordinates)
					this->OnCTCoordinates(CTCoordinates[0], CTCoordinates[1], CTCoordinates[2]);

				if (this->OnCTIndexes)
					this->OnCTIndexes(indexes[0], indexes[1], indexes[2]);

				//float* imageOrigin = imageReader->GetImagePositionPatient();
				//ctCoordinates[0] = imageOrigin[0] + worldCoordinates[0];
				break;
			}
		}
	}

	vtkSmartPointer < vtkResliceImageViewer > imageViews[3];
	vtkSmartPointer< vtkDICOMImageReader > imageReader;
	std::function<void(double, double, double)> OnCTCoordinates;
	std::function<void(int, int, int)> OnCTIndexes;
};


class vtkLeftButtonPressCallback : public vtkCommand
{
public:
	static vtkLeftButtonPressCallback* New()
	{
		return new vtkLeftButtonPressCallback;
	}

	void Execute(vtkObject* caller, unsigned long event, void* callData) override
	{
		vtkRenderWindowInteractor* interactor = dynamic_cast<vtkRenderWindowInteractor*>(caller);
		if (!interactor)
			return;

		for (int i = 0; i < 3; ++i)
		{
			// Search for the image whose interactor has been triggered
			if (this->imageViews[i] && this->imageViews[i]->GetInteractor() == interactor)
			{
				int* eventPosition = interactor->GetEventPosition();
				auto x = eventPosition[0];
				auto y = eventPosition[1];

				if (this->callback)
				{
					this->callback(i, x, y);
				}
			}
		}
	}

	vtkSmartPointer < vtkResliceImageViewer > imageViews[3];
	std::function<void(int, int, int)> callback;
};


QtVTKFourPanesBox::QtVTKFourPanesBox()
{
	QGridLayout* layout = new QGridLayout();
	this->setLayout(layout);

	// Dicom volume reader
	this->imageReader = vtkSmartPointer< vtkDICOMImageReader >::New();
	this->ctVolume = new CTVolume(this->imageReader->GetOutputPort());
	vtkNew< vtkWindowLevelCallback > windowLevelCallback;
	vtkNew< vtkMouseMoveCallback > mouseMoveCallback;
	vtkNew< vtkLeftButtonPressCallback > leftButtonPressCallback;

	for (auto i = 0; i < 3; ++i)
	{
		// QT Slider
		this->sliders[i] = new QSlider(Qt::Orientation::Horizontal);

		// QT image viewer widget
		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
		this->imageWidgets[i] = new QVTKOpenGLNativeWidget(this);
		this->imageWidgets[i]->SetRenderWindow(renderWindow);

		// Dicom volume reslicer
		this->imageReslice[i] = vtkSmartPointer < vtkImageReslice >::New();
		this->imageReslice[i]->SetInterpolationModeToLinear();
		this->imageReslice[i]->SetInputConnection(this->imageReader->GetOutputPort());

		// Dicom image view
		this->imageViews[i] = vtkSmartPointer <vtkResliceImageViewer>::New();
		this->imageViews[i]->SetRenderWindow(renderWindow);
		this->imageViews[i]->SetupInteractor(renderWindow->GetInteractor());
		this->imageViews[i]->SetSliceOrientation(2 - i);
		this->imageViews[i]->SetInputConnection(this->imageReslice[i]->GetOutputPort());

		// Set windowLevelCallback for the image view interactor
		windowLevelCallback->imageViews[i] = this->imageViews[i];
		mouseMoveCallback->imageViews[i] = this->imageViews[i];
		leftButtonPressCallback->imageViews[i] = this->imageViews[i];
		leftButtonPressCallback->callback = [&](int view, int x, int y) { this->AddTool(view, x, y); };
		this->imageViews[i]->GetInteractorStyle()->AddObserver(vtkCommand::WindowLevelEvent, windowLevelCallback);
		this->imageViews[i]->GetInteractor()->AddObserver(vtkCommand::MouseMoveEvent, mouseMoveCallback);
		this->imageViews[i]->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent, leftButtonPressCallback);

		// Set a slot on the slider to change slice
		connect(this->sliders[i], SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
	}
	this->SetToFourPanesView();
	
	mouseMoveCallback->imageReader = this->imageReader;
	mouseMoveCallback->OnCTCoordinates = [&](double x, double y, double z) { emit OnCTCoordinatesSignal(x, y, z); };
	mouseMoveCallback->OnCTIndexes = [&](int x, int y, int z) { emit OnCTIndexesSignal(x, y, z); };

	this->reset();
	this->OpenCT("D:/PelvicDataset/1");
}


void QtVTKFourPanesBox::ViewChangeSlot(ViewType viewType)
{
	switch (viewType)
	{
	case ViewType::FourPanes:
		this->SetToFourPanesView();
		break;
	case ViewType::ThreePanes:
		this->SetToThreePanesView();
		break;
	case ViewType::Volume:
		this->SetToVolumeView();
		break;
	}
}


void QtVTKFourPanesBox::SetToFourPanesView(void)
{
	this->ResetLayout();
	QGridLayout* layout = (QGridLayout*)this->layout();
	// TOP LEFT
	layout->addWidget(this->sliders[0], 0, 0);
	layout->addWidget(this->imageWidgets[0], 1, 0);
	//TOP RIGHT
	layout->addWidget(this->ctVolume, 1, 1);
	// BOTTOM RIGHT
	layout->addWidget(this->sliders[1], 2, 1);
	layout->addWidget(this->imageWidgets[1], 3, 1);
	// BOTTOM LEFT
	layout->addWidget(this->sliders[2], 2, 0);
	layout->addWidget(this->imageWidgets[2], 3, 0);

	for (int i = 0; i < 3; ++i)
	{
		this->sliders[i]->setHidden(false);
		this->imageWidgets[i]->setHidden(false);
	}
	this->ctVolume->setHidden(false);
}


void QtVTKFourPanesBox::SetToThreePanesView(void)
{
	this->ResetLayout();
	QGridLayout* layout = (QGridLayout*)this->layout();
	for (int i = 0; i < 3; ++i)
	{
		layout->addWidget(this->sliders[i], 0, i);
		layout->addWidget(this->imageWidgets[i], 1, i);
		this->sliders[i]->setHidden(false);
		this->imageWidgets[i]->setHidden(false);
	}
}


void QtVTKFourPanesBox::SetToVolumeView(void)
{
	this->ResetLayout();
	QGridLayout* layout = (QGridLayout*)this->layout();
	layout->addWidget(this->ctVolume, 0, 0);
	this->ctVolume->setHidden(false);
}


void QtVTKFourPanesBox::ResetLayout(void)
{
	QGridLayout* layout = (QGridLayout*)this->layout();
	for (int i = layout->count(); i > 0; --i)
	{
		layout->removeItem(this->layout()->itemAt(i));
	}
	for (int i = 0; i < 3; ++i)
	{
		this->sliders[i]->setHidden(true);
		this->imageWidgets[i]->setHidden(true);
	}
	this->ctVolume->setHidden(true);
}


void QtVTKFourPanesBox::OpenCT(QString path)
{
	this->imageReader->SetDirectoryName(path.toStdString().c_str());
	this->imageReader->Update();
	// Check if CT was properly read
	if (!this->imageReader->GetErrorCode())
	{
		this->SetSliders();
		for (int i = 0; i < 3; ++i)
		{
			this->imageViews[i]->GetRenderer()->ResetCamera();
			this->imageViews[i]->Render();
		}
		emit patientNameChangedSignal(QString(this->imageReader->GetPatientName()));
	}
	else
	{
		this->reset();
	}
}



void QtVTKFourPanesBox::reset(void)
{
	for (int i = 0; i < 3; ++i)
	{
		this->sliders[i]->setRange(0, 0);
		this->sliders[i]->setValue(0);
	}
}


void QtVTKFourPanesBox::SetSliders(void)
{
	for (int i = 0; i < 3; ++i)
	{
		int sliceMin = this->imageViews[i]->GetSliceMin();
		int sliceMax = this->imageViews[i]->GetSliceMax();
		int currentSlice = (sliceMin + sliceMax) / 2;
		this->imageViews[i]->SetSlice(currentSlice);
		this->sliders[i]->setRange(sliceMin, sliceMax);
		this->sliders[i]->setValue(currentSlice);
	}
}


void QtVTKFourPanesBox::sliderChanged(int value)
{
	for (int i = 0; i < 3; ++i)
	{
		if (this->sliders[i]->isSliderDown())
		{
			this->imageViews[i]->SetSlice(value);
			break;
		}
	}
}


void QtVTKFourPanesBox::AddToolSlot(ToolType toolType)
{
	this->toolAdded = toolType;
}


void QtVTKFourPanesBox::AddTool(int view, int xPosition, int yPosition)
{
	bool render = false;
	switch (this->toolAdded)
	{
	case ToolType::None:
		return;
	case ToolType::Point:
		this->AddPointToViews(view, xPosition, yPosition);
		render = true;
		break;
	case ToolType::Ruler:
		this->AddRulerToView(view);
		render = true;
		break;
	}
	if (render)
		for (auto i = 0; i < 3; ++i)
			this->imageViews[i]->Render();
	this->toolAdded = ToolType::None;
}


void QtVTKFourPanesBox::AddRulerToView(int view)
{
	vtkNew< vtkDistanceWidget > widget;
	widget->SetInteractor(this->imageViews[view]->GetInteractor());

	vtkNew< vtkPointHandleRepresentation2D > handleRep;
	vtkNew< vtkDistanceRepresentation2D > distanceRep;

	distanceRep->SetHandleRepresentation(handleRep);
	widget->SetRepresentation(distanceRep);
	distanceRep->InstantiateHandleRepresentation();
	distanceRep->GetPoint1Representation()->SetPointPlacer(this->imageViews[view]->GetPointPlacer());
	distanceRep->GetPoint2Representation()->SetPointPlacer(this->imageViews[view]->GetPointPlacer());

	this->imageViews[view]->GetMeasurements()->AddItem(widget);
	widget->CreateDefaultRepresentation();
	widget->EnabledOn();
	this->widgets.emplace_back(widget);
}


void QtVTKFourPanesBox::AddPointToViews(int view, int xPosition, int yPosition)
{
	vtkNew< vtkHandleWidget > widget;
	widget->SetInteractor(this->imageViews[view]->GetInteractor());
	vtkNew< vtkConstrainedPointHandleRepresentation  > handleRep;

	widget->SetRepresentation(handleRep);
	widget->CreateDefaultRepresentation();
	widget->EnabledOn();

	double position[] = { (double)xPosition, (double)yPosition };
	handleRep->SetDisplayPosition(position);
	this->widgets.emplace_back(widget);
}


void QtVTKFourPanesBox::DeleteAllToolsSlot(void)
{
	bool render = false;
	for (auto widget : this->widgets)
	{
		widget->SetEnabled(0);
		widget = nullptr;
		render = true;
	}
	this->widgets.clear();
	if (render)
		for (auto i = 0; i < 3; ++i)
			this->imageViews[i]->Render();
}