#include "Histogram.h"

#include <vtkIdTypeArray.h>
#include <vtkArrayIterator.h>

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtGui/QIntValidator>



Histogram::Histogram(QWidget* parent, vtkSmartPointer < vtkDICOMImageReader > imageReader) : QWidget(parent)
{
	QVBoxLayout* layout = new QVBoxLayout();
	this->setLayout(layout);

	// Chart View
	this->chart = new QChart();
	QChartView* chartView = new QChartView(this->chart);
	layout->addWidget(chartView);

	// Parameters
	QHBoxLayout* parametersLayout = new QHBoxLayout();
	auto minimumValue = new QLabel("Minimum value:");
	auto maximumValue = new QLabel("Maximum value:");
	auto nBins = new QLabel("N bins:");

	this->minimumValue = new QLineEdit();
	this->minimumValue->setValidator(new QIntValidator());
	this->minimumValue->setText(std::to_string(this->MIN_HOUNSFIELD).c_str());
	this->maximumValue = new QLineEdit();
	this->maximumValue->setText(std::to_string(this->MAX_HOUNSFIELD).c_str());
	this->maximumValue->setValidator(new QIntValidator());
	this->nBins = new QLineEdit();
	this->nBins->setText("100");
	this->nBins->setValidator(new QIntValidator());

	for (auto widget : { this->minimumValue, this->maximumValue, this->nBins })
	{
		widget = new QLineEdit();
		widget->setValidator(new QIntValidator());
	}

	parametersLayout->addWidget(minimumValue);
	parametersLayout->addWidget(this->minimumValue);
	parametersLayout->addWidget(maximumValue);
	parametersLayout->addWidget(this->maximumValue);
	parametersLayout->addWidget(nBins);
	parametersLayout->addWidget(this->nBins);

	auto validateButton = new QPushButton("Apply");
	connect(validateButton, SIGNAL(clicked()), this, SLOT(OnValidateParameters(void)));
	parametersLayout->addWidget(validateButton);
	layout->addLayout(parametersLayout);

	this->histogram = vtkSmartPointer< vtkImageHistogram >::New();
	this->histogram->SetInputConnection(imageReader->GetOutputPort());
	this->OnValidateParameters();
}


void Histogram::OnValidateParameters(void)
{
	auto minimumValue = this->minimumValue->text().toLong();
	auto maximumValue = this->maximumValue->text().toLong();
	auto nBins = this->nBins->text().toLong();
	if (minimumValue >= maximumValue || nBins <= 1)
		return;

	auto binSpacing = (maximumValue - minimumValue) / nBins;

	this->histogram->SetBinOrigin(minimumValue);
	this->histogram->SetBinSpacing(binSpacing);
	this->histogram->SetNumberOfBins(nBins);
	this->histogram->Update();

	QBarSet* set0 = new QBarSet("");

	auto values = this->histogram->GetHistogram();
	for (auto value = values->Begin()+1; value < values->End()-1; ++value)
	{
		*set0 << *value;
	}

	QBarSeries* series = new QBarSeries();
	series->append(set0);
	this->chart->removeAllSeries();
	this->chart->addSeries(series);

	QValueAxis* axisX = new QValueAxis();
	axisX->setMin(minimumValue);
	axisX->setMax(maximumValue);
	axisX->setTickCount(20);
	axisX->applyNiceNumbers();
	this->chart->setAxisX(axisX);
}