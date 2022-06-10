#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLayout>
#include <QtCharts/QChartView>

#include "vtkSmartPointer.h"
#include <vtkImageHistogram.h>
#include "vtkDICOMImageReader.h"


using namespace QtCharts;


class Histogram : public QWidget
{
Q_OBJECT
public:
	Histogram(QWidget* parent, vtkSmartPointer < vtkDICOMImageReader > imageReader);

public slots:
	void OnValidateParameters(void);

signals:

private:
	const int MIN_HOUNSFIELD = -1000;
	const int MAX_HOUNSFIELD = 3071;
	QLineEdit* minimumValue;
	QLineEdit* maximumValue;
	QLineEdit* nBins;
	vtkSmartPointer< vtkImageHistogram > histogram;
	QChart* chart;
};

