#pragma once

#include "QVTKOpenGLNativeWidget.h"
#include "vtkAlgorithmOutput.h"
#include "vtkImageData.h"

class CTVolume: public QVTKOpenGLNativeWidget
{
public:
	CTVolume(vtkAlgorithmOutput* input);
};

