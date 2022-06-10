#pragma once

#include <QtWidgets/QGroupBox>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>


class InfoBox: public QGroupBox
{
public:
	InfoBox();

public slots:
	void setPatientNameSlot(QString name);
	void setCTCoordinatesSlot(double x, double y, double z);
	void setPixelIndexSlot(int x, int y, int z);

private:
	QLabel* patientName;
	QLabel* indexLabel[3];
	QLabel* coordLabel[3];
};

