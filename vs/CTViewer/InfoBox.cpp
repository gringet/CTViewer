#include "InfoBox.h"


InfoBox::InfoBox()
{
	//this->setTitle("Info");
	QGridLayout* layout = new QGridLayout();
	this->setLayout(layout);

	int row = 0;

	// Patient info
	this->patientName = new QLabel("...");
	layout->addWidget(new QLabel("Name"), row, 0, Qt::AlignTop);
	layout->addWidget(this->patientName, row, 1, Qt::AlignTop);
	layout->setRowStretch(row, 1);
	++row;

	// Indexes and coordinates labels.
	int i = 0;
	for (QString coordName : {"X:", "Y:", "Z:"})
	{
		layout->addWidget(new QLabel(coordName), row, 0);
		layout->setRowStretch(row, 0);
		this->indexLabel[i] = new QLabel("0");
		layout->addWidget(this->indexLabel[i], row, 1);
		this->coordLabel[i] = new QLabel("0mm");
		layout->addWidget(this->coordLabel[i], row, 2);
		++row;
		++i;
	}
}


void InfoBox::setPixelIndexSlot(int x, int y, int z)
{
	int i = 0;
	for (int v : {x, y, z})
	{
		if (v >= 0)
			this->indexLabel[i]->setText(QString::number(v));
		++i;
	}
}


void InfoBox::setCTCoordinatesSlot(double x, double y, double z)
{
	int i = 0;
	for (double v : {x, y, z})
	{
		if (v >= 0)
			this->coordLabel[i]->setText(QString::number(v, 'f', 1));
		++i;
	}
}


void InfoBox::setPatientNameSlot(QString name)
{
	this->patientName->setText(name);
}
