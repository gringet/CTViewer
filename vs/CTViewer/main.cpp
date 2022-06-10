#include "CTViewer.h"

#include <QSurfaceFormat>
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    //QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
    QApplication app(argc, argv);
    QFile file("StyleSheet.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    app.setStyleSheet(styleSheet);

    CTViewer w;
    w.show();
    return app.exec();
}

//#include <vtkSmartPointer.h>
//#include <vtkImageData.h>
//#include <vtkDICOMImageReader.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderer.h>
//#include <vtkInteractorStyleTrackballCamera.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkSmartVolumeMapper.h>
//#include <vtkVolumeProperty.h>
//#include <vtkPiecewiseFunction.h>
//#include <vtkColorTransferFunction.h>
//#include "vtkRayCastImageDisplayHelper.h"
//
//
//#include <vtkOpenGLGPUVolumeRayCastMapper.h>
//
//const struct Hounsfield {
//    int air = -1000;
//    int lungs = -500;
//    int fat = -75;
//    int water = 0;
//    int blood = 40;
//    int muscle = 30;
//    int grayMatter = 40;
//    int whiteMatter = 25;
//    int liver = 50;
//    int softTissues = 200;
//    int boneSpongious = 700;
//    int boneDense = 3000;
//};
//
//int main(int argc, char* argv[]) {
//
//    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
//    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
//    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//    vtkSmartPointer<vtkInteractorStyleTrackballCamera> interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//
//    vtkSmartPointer<vtkOpenGLGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkOpenGLGPUVolumeRayCastMapper>::New();
//
//
//    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
//    vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
//    vtkSmartPointer<vtkPiecewiseFunction> scalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
//    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
//    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
//
//    reader->SetDirectoryName("C:/Users/JeremieGringet/Desktop/STATIC_1_0_EFOV_VOL_0005_out");
//    reader->Update();
//    imageData->ShallowCopy(reader->GetOutput());
//
//    renderer->SetBackground(0.0, 0.0, 0.0);
//
//    renderWindow->AddRenderer(renderer);
//    renderWindow->SetSize(500, 500);
//
//    renderWindowInteractor->SetInteractorStyle(interactorStyle);
//    renderWindowInteractor->SetRenderWindow(renderWindow);
//
//    volumeMapper->SetBlendModeToComposite();
//    //volumeMapper->SetRequestedRenderModeToGPU();
//    volumeMapper->SetInputData(imageData);
//
//    volumeProperty->ShadeOn();
//    volumeProperty->SetInterpolationTypeToLinear();
//
//    volumeProperty->SetAmbient(1.0);
//    volumeProperty->SetDiffuse(0.9);
//    volumeProperty->SetSpecular(0.2);
//    volumeProperty->SetSpecularPower(10.0);
//
//    gradientOpacity->AddPoint(0.0, 0.0);
//    gradientOpacity->AddPoint(2000.0, 1.0);
//    volumeProperty->SetGradientOpacity(gradientOpacity);
//
//    scalarOpacity->AddPoint(0.0, 0.0);
//    scalarOpacity->AddPoint(1.0, 1.0);
//    scalarOpacity->AddPoint(700.0, 1.0);
//    scalarOpacity->AddPoint(701.0, 0.0);
//    volumeProperty->SetScalarOpacity(scalarOpacity);
//
//    //color->AddRGBPoint(-750.0, 0.08, 0.05, 0.03);
//    //color->AddRGBPoint(-350.0, 0.39, 0.25, 0.16);
//    //color->AddRGBPoint(-200.0, 0.80, 0.80, 0.80);
//    //color->AddRGBPoint(2750.0, 0.70, 0.70, 0.70);
//    //color->AddRGBPoint(3000.0, 0.35, 0.35, 0.35);
//    //volumeProperty->SetColor(color);
//
//    volume->SetMapper(volumeMapper);
//    volume->SetProperty(volumeProperty);
//    renderer->AddVolume(volume);
//    //renderer->ResetCamera();
//
//    renderWindow->Render();
//    renderWindowInteractor->Start();
//
//    return EXIT_SUCCESS;
//}


