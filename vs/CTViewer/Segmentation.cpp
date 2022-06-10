#include "Segmentation.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMeanImageFilter.h"
#include "itkGDCMImageIO.h"


Segmentation::Segmentation(void)
{
	//using InputPixelType = signed short;
	//using OutputPixelType = signed short;
	//
	//using InputImageType = itk::Image<InputPixelType, 2>;
	//using OutputImageType = itk::Image<OutputPixelType, 2>;
	//using ImageIOType = itk::GDCMImageIO;
	//
	//auto gdcmImageIO = ImageIOType::New();
	//auto reader = itk::ImageFileReader<InputImageType>::New();
	//auto writer = itk::ImageFileWriter<OutputImageType>::New();
	//auto filter = itk::MeanImageFilter<InputImageType, OutputImageType>::New();
	//
	//reader->SetFileName("C:\\Users\\JeremieGringet\\Desktop\\in.DCM");
	//writer->SetFileName("C:\\Users\\JeremieGringet\\Desktop\\out1234.DCM");
	//
	//reader->SetImageIO(gdcmImageIO);
	//writer->UseInputMetaDataDictionaryOff();
	//writer->SetImageIO(gdcmImageIO);
	//
	//InputImageType::SizeType indexRadius;
	//indexRadius[0] = 10; // radius along x
	//indexRadius[1] = 10; // radius along y
	//filter->SetRadius(indexRadius);
	//
	//filter->SetInput(reader->GetOutput());
	//writer->SetInput(filter->GetOutput());
	//
	//try
	//{
	//    reader->Update();
	//    writer->Update();
	//}
	//catch (const itk::ExceptionObject& e)
	//{
	//    std::cerr << "exception in file reader " << std::endl;
	//    std::cerr << e << std::endl;
	//}
}