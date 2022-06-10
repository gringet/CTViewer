#pragma once

#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

class ThresholdingBox : public QWidget
{
Q_OBJECT
public:
	ThresholdingBox(QWidget* parent = Q_NULLPTR);

public slots:
	virtual void sliderChanged(int value);

signals:
	void thresholdChangedSignal(int low, int high);

private:
	QSlider* lowSlider;
	QSlider* highSlider;
	const int MIN_HOUNSFIELD = -1000;
	const int MAX_HOUNSFIELD = 3071;
};

