#include "CTVolume.h"


#include "vtkGenericOpenGLRenderWindow.h"

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include "vtkRayCastImageDisplayHelper.h"
#include <vtkOpenGLGPUVolumeRayCastMapper.h>

CTVolume::CTVolume(vtkAlgorithmOutput* input)
{
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	this->SetRenderWindow(renderWindow);

	vtkNew<vtkRenderer> renderer;
	vtkNew<vtkOpenGLGPUVolumeRayCastMapper> volumeMapper;
	vtkNew<vtkVolume> volume;
	
	renderer->SetBackground(0.0, 0.0, 0.0);
	renderWindow->AddRenderer(renderer);
	volumeMapper->SetBlendModeToComposite();
	volumeMapper->SetInputConnection(input);

	vtkNew<vtkVolumeProperty> volumeProperty;
	vtkNew<vtkPiecewiseFunction> gradientOpacity;
	int value = 200;
	gradientOpacity->AddPoint(value, 0.0);
	gradientOpacity->AddPoint(value + 1, 1.0);
	volumeProperty->SetGradientOpacity(gradientOpacity);

	vtkNew<vtkPiecewiseFunction> scalarOpacity;
	value = 300;
	scalarOpacity->AddPoint(value, 0.0);
	scalarOpacity->AddPoint(value + 1, 1.0);
	volumeProperty->SetScalarOpacity(scalarOpacity);
	
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	renderer->AddVolume(volume);
}
