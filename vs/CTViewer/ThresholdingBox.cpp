#include "ThresholdingBox.h"


ThresholdingBox::ThresholdingBox(QWidget* parent) : QWidget(parent)
{
	QVBoxLayout* mainLayout = new QVBoxLayout();
	this->setLayout(mainLayout);
	this->lowSlider = new QSlider(Qt::Orientation::Horizontal);
	this->lowSlider->setRange(this->MIN_HOUNSFIELD, this->MAX_HOUNSFIELD);
	this->highSlider = new QSlider(Qt::Orientation::Horizontal);
	this->highSlider->setRange(this->MIN_HOUNSFIELD, this->MAX_HOUNSFIELD);

	mainLayout->addWidget(this->lowSlider);
	mainLayout->addWidget(this->highSlider);

	connect(this->lowSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
	connect(this->highSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
}


void ThresholdingBox::sliderChanged(int value)
{
	if (this->lowSlider->isSliderDown())
	{
		if (this->highSlider->value() < value)
		{
			this->highSlider->setValue(value);
		}
	}
	else
	{
		if (this->lowSlider->value() > value)
		{
			this->lowSlider->setValue(value);
		}
	}
	emit thresholdChangedSignal(this->lowSlider->value(), this->highSlider->value());
}

